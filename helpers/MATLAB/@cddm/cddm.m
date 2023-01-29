classdef cddm < handle
    %
    %CDDM Class for handling circular drift diffusion models
    %
    %  Basic usage:
    %
    %    obj = cddm(seed, driftx, drifty, bound, nondt, ssize)
    %
    %      Creates a cddm object with parameters:
    %        |seed|   for the random number generation
    %        |driftx| for horizontal drift
    %        |drifty| for vertical drift
    %        |bound|  for the radial bound
    %        |nondt|  for the nondecision time
    %        |ssize|  for the sample size 
    %  
    %    obj.simulate()
    %  
    %      Populates the |data| property with data simulated
    %      from the object's parameters.  The data structure
    %      will contain response angles (in radians) and
    %      reaction times (in seconds).
    %
    
    % (c) 2022 joachim vandekerckhove <joachim@uci.edu>
    
    %
    %  Non-supported usage:
    %  
    %    [stats, chains, diagnostics, info] = obj.recover()
    %    
    %      Uses the Trinity package to recover parameters from
    %      data.  Requires Trinity and JAGS.
    %  

    properties  (Hidden)
        outputPath
    end

    properties  (SetAccess = private)

        seed       { isscalar, isinteger }
        ssize      { isscalar }

        trueDriftx { isscalar, isreal }
        trueDrifty { isscalar, isreal }
        trueBound  { isscalar, isreal }
        trueNondt  { isscalar, isreal }

    end

    properties  (Hidden, SetAccess = private)
        estDriftx
        estDrifty
        estBound
        estNondt

        maxRhat

        mcmc
    end

    properties
        data
    end

    properties (Hidden)
        dt               = 0.001
        maxT             = 15
        driftCoefficient = 1

        loDriftx
        loDrifty
        loBound
        loNondt

        hiDriftx
        hiDrifty
        hiBound
        hiNondt

    end

    methods
        function obj = cddm(seed, driftx, drifty, bound, nondt, ssize)

            if nargin
                obj.seed       = seed;

                obj.trueDriftx = driftx;
                obj.trueDrifty = drifty;
                obj.trueBound  = bound;
                obj.trueNondt  = nondt;
                
                obj.ssize      = ssize;
            end

        end

        function simulate(obj)
        
            if isempty(obj.seed)
                error('cddm:simulate:inputsUnset', ...
                   'You need to create this object with 6 input variables.')
            end

            sqDt = sqrt(obj.dt);

            maxIter = round(obj.maxT / obj.dt);
            sigma = obj.driftCoefficient;

            xsteps = randn(obj.ssize, maxIter) * (sigma * sqDt) + obj.trueDriftx * obj.dt;
            ysteps = randn(obj.ssize, maxIter) * (sigma * sqDt) + obj.trueDrifty * obj.dt;

            xstate = cumsum(xsteps, 2);
            ystate = cumsum(ysteps, 2);

            crossLoc = sqrt(xstate.^2 + ystate.^2) > obj.trueBound;
            [~, crossSub] = max(crossLoc, [], 2);
            crossInd = sub2ind(size(xstate), 1:obj.ssize, crossSub');

            obj.data.angle = cart2pol(xstate(crossInd), ystate(crossInd))';
            obj.data.rt    = crossSub * obj.dt + obj.trueNondt;

        end

    end

    methods (Hidden)

        function [stats, chains, diagnostics, information] = recover(obj)

            if ~exist('+trinity')
                error('cddm:recover:noTrinity', ...
                    'Trinity not found.  Get it here: https://github.com/joachimvandekerckhove/trinity')
            end

            proj_id   = sprintf('%s_s%04i', 'cddm', obj.seed);

            dataStruct = struct( ...
                'X' , [obj.data.angle(:) obj.data.rt(:)]');

            guess = obj.ez();

            p = @sprintf;

            model = { ...
                'data {'
                '    tmin = 0.95 * min(X[2,])'
                '    D = dim(X)'
                '    N = D[2]'
                '} '
                ''
                'model{'
                '    # Likelihood'
                '    for (i in 1:N) {'
                '        X[1:2,i] ~ dcddm(driftx, drifty, bound, ter0)'
                '    }'
                '    # Priors'
                '    driftx ~ dnorm(0.0, 1.0)'
                '    drifty ~ dnorm(0.0, 1.0)'
                '    bound  ~ dgamma(4, 1)'
                '    ter0   ~ dexp(1)T(, tmin)'
                '} '
            };

            monitors = { 'driftx'  'drifty', 'bound'  'ter0' };

            generator = @()struct( ...
                'driftx' , guess.driftx , ...
                'drifty' , guess.drifty , ...
                'bound'  , guess.bound * (rand/4+7/8) , ...
                'ter0'   , min(obj.data.rt) * 0.9);

            generator = @()struct( ...
                'bound'  , guess.bound * (rand/4+7/8));

            try
                [ ...
                    obj.mcmc.stats, ...
                    obj.mcmc.chains, ...
                    obj.mcmc.diagnostics, ...
                    obj.mcmc.information] = trinity.callbayes( ...
                    'jags', ...
                    'model'          ,         model , ...
                    'data'           ,    dataStruct , ...
                    'outputname'     ,     'samples' , ...
                    'init'           ,     generator , ...
                    'datafilename'   ,       proj_id , ...
                    'initfilename'   ,       proj_id , ...
                    'scriptfilename' ,       proj_id , ...
                    'logfilename'    ,       proj_id , ...
                    'nchains'        ,             3 , ...
                    'nburnin'        ,           400 , ...
                    'nsamples'       ,           400 , ...
                    'monitorparams'  ,      monitors , ...
                    'thin'           ,            10 , ...
                    'workingdir'     ,  ['/tmp/' proj_id]  , ...
                    'verbosity'      ,             0 , ...
                    'saveoutput'     ,         true  , ...
                    'parallel'       ,      isunix() , ...
                    'modules'        ,      {'dic' 'cddm'}  );

                for f = fieldnames(obj.mcmc.chains)'
                    x = reshape(obj.mcmc.chains.(f{1}),[],1);
                    obj.mcmc.stats.lower.(f{1}) = prctile(x,  2.5);
                    obj.mcmc.stats.upper.(f{1}) = prctile(x, 97.5);
                end

                obj.estDriftx = obj.mcmc.stats.mean.driftx;
                obj.estBound  = obj.mcmc.stats.mean.bound;
                obj.estDrifty = obj.mcmc.stats.mean.drifty;
                obj.estNondt  = obj.mcmc.stats.mean.ter0;

                obj.loDriftx = obj.mcmc.stats.lower.driftx;
                obj.loBound  = obj.mcmc.stats.lower.bound;
                obj.loDrifty = obj.mcmc.stats.lower.drifty;
                obj.loNondt  = obj.mcmc.stats.lower.ter0;

                obj.hiDriftx = obj.mcmc.stats.upper.driftx;
                obj.hiBound  = obj.mcmc.stats.upper.bound;
                obj.hiDrifty = obj.mcmc.stats.upper.drifty;
                obj.hiNondt  = obj.mcmc.stats.upper.ter0;

                obj.maxRhat = max(struct2array(obj.mcmc.diagnostics.Rhat));

            catch me
                disp( getReport( me, 'extended', 'hyperlinks', 'on' ) );
                obj.estDriftx = 0;
                obj.estBound  = 0;
                obj.estDrifty = 0;
                obj.estNondt  = 0;
                obj.maxRhat   = Inf;
            end

        end

        function saveSeed(obj)

            seeds = [obj.seed];

            if any(seeds ~= seeds(1))
                error('Method cddm.saveSeed expects all seeds to be identical.')
            end

            fName = sprintf('%s/s%04i.csv', obj(1).outputPath, obj(1).seed);
            fid = fopen(fName, 'w');

            fprintf(fid, ['%16s,%16s,' ...
                '%16s,%16s,%16s,%16s,' ...
                '%16s,%16s,%16s,%16s,' ...
                '%16s,%16s,%16s,%16s,' ...
                '%16s,%16s,%16s,%16s,' ...
                '%16s,%16s,%16s,%16s,' ...
                '%16s\n'], ...
                'seed'      , 'sampleSize', ...
                'trueDriftx', 'trueDrifty', 'trueBound', 'trueNondt', ...
                'ezDriftx'  , 'ezDrifty'  , 'ezBound'  , 'ezNondt'  , ...
                'estDriftx' , 'estDrifty' , 'estBound' , 'estNondt' , ...
                'loDriftx'  , 'loDrifty'  , 'loBound'  , 'loNondt'  , ...
                'hiDriftx'  , 'hiDrifty'  , 'hiBound'  , 'hiNondt'  , ...
                'maxRhat');

            for i = 1:numel(obj)
                ez = obj(i).ez();
                fprintf(fid, ['%16i,%16i,' ...
                    '%16.6f,%16.6f,%16.6f,%16.6f,' ...
                    '%16.6f,%16.6f,%16.6f,%16.6f,' ...
                    '%16.6f,%16.6f,%16.6f,%16.6f,' ...
                    '%16.6f,%16.6f,%16.6f,%16.6f,' ...
                    '%16.6f,%16.6f,%16.6f,%16.6f,' ...
                    '%16.6f\n'], ...
                    obj(i).seed,obj(i).ssize, ...
                    obj(i).trueDriftx , obj(i).trueDrifty , obj(i).trueBound , obj(i).trueNondt , ...
                    ez.driftx         , ez.drifty         , ez.bound         , ez.ter0          , ...
                    obj(i).estDriftx  , obj(i).estDrifty  , obj(i).estBound  , obj(i).estNondt  , ...
                    obj(i).estDriftx  , obj(i).estDrifty  , obj(i).estBound  , obj(i).estNondt  , ...
                    obj(i).estDriftx  , obj(i).estDrifty  , obj(i).estBound  , obj(i).estNondt  , ...
                    obj(i).maxRhat);
            end

            fclose(fid);
        end

        function report(obj)
            ez = obj.ez;
            di = obj.mcmc.diagnostics.Rhat;
            fprintf('%10s%10s%10s%10s%10s\n'        , ''        , 'driftx'      , 'drifty'      , 'bound'      , 'ndt'        );
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'true'    , obj.trueDriftx, obj.trueDrifty, obj.trueBound, obj.trueNondt);
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'guess'   , ez.driftx     , ez.drifty     , ez.bound     , ez.ter0      );
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'lower'   , obj.loDriftx  , obj.loDrifty  , obj.loBound  , obj.loNondt  );
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'estimate', obj.estDriftx , obj.estDrifty , obj.estBound , obj.estNondt );
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'upper'   , obj.hiDriftx  , obj.hiDrifty  , obj.hiBound  , obj.hiNondt  );
            fprintf('%10s%10.4f%10.4f%10.4f%10.4f\n', 'Rhat'    , di.driftx     , di.drifty     , di.bound     , di.ter0      );
        end

        function out = ez(obj)
            angle = obj.data.angle;
            rt    = obj.data.rt;

            N = length(rt);

            MCA = obj.circmean(angle);
            VCA = 1 - sqrt(sum(cos(angle))^2+sum(sin(angle))^2)/N;
            MRT = mean(rt);
            VRT = var(rt);

            theta0 = MCA;
            R      = 1 - VCA;
            k0     = R*(2-R^2)/(1-R^2);

            ivrat  = besseli(1, k0) / besseli(0, k0);

            k1     = k0 - (ivrat-R) / (1-ivrat^2-ivrat/k0);
            drift  = ((k1^2*R^2+2*k1*R-k1^2)/VRT)^(1/4);
            bound  = k1/drift;
            ter0   = MRT-bound/drift*R;

            driftx = drift * cos(theta0);
            drifty = drift * sin(theta0);

            out = struct( ...
                'driftx' , driftx , ...
                'drifty' , drifty , ...
                'bound'  , bound  , ...
                'ter0'   , ter0     ...
                );

        end

    end

    methods (Static, Hidden)

        function out = circmean(rads)
            out = atan2( ...
                sum(sin(rads(:))) , ...
                sum(cos(rads(:)))   ...
                );
        end

        function compile()

            !rm -rf /tmp/jags-cddm/
            !cp -r jags-cddm/ /tmp/
            !cd /tmp/jags-cddm && autoreconf -fvi > /dev/null && ./configure --prefix=/usr > /dev/null && make > /dev/null && sudo make install
            !sudo cp /usr/lib/JAGS/modules-4/cddm.* /usr/lib/x86_64-linux-gnu/JAGS/modules-4/

        end

        function out = rnd(n)

            if ~nargin
                n = 1000;
            end

            dx = randn;
            dy = randn;
            bo = 1.0 + rand;
            nd = 0.1 + rand/5;

            out = cddm(rand, dx, dy, bo, nd, n);
            out.simulate();
            out.recover();

        end

    end

end

