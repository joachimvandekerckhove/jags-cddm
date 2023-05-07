% Set the sample size for the cddm.rnd function and the number of
% iterations
sampleSize = 200;
nIter      = 3;

% Loop over nIter and call the rnd method of the cddm class, 
% storing the output in the output array
for i = 1:nIter
    output(i) = cddm.rnd(sampleSize);
end

% Define an array of parameter names
parameters = ["Driftx" "Drifty" ...
    "DriftLength" "DriftAngle" ...
    "Bound" "Nondt"];

% Get the number of parameters
nParameters = length(parameters);

% Initialize the xValue and yValue arrays
xValue = zeros(nIter, nParameters);
yValue = zeros(nIter, nParameters);

% Loop over nIter and the parameter names, 
% extracting the true and estimated parameter values and storing them in
% xValue and yValue 
for i = 1:nIter
    for p = 1:nParameters
        xValue(i,p) = output(i).("true"+parameters(p));
        yValue(i,p) = output(i).("est" +parameters(p));
    end
end

%%
clf

% Create a figure with subplots for each parameter
red = [output.maxRhat] > 1.05;
for p = 1:nParameters
    subplot(2, 3, p)
    plot(xValue(:,p), yValue(:,p), '.', ...
        xValue(red,p), yValue(red,p), 'r.')
    axis equal
    grid on
    bisector
    axis tight
    xlabel('True Parameter Value')
    ylabel('Estimated Parameter Value')
    title(parameters(p))
end
