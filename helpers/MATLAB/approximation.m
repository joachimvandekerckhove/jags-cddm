% Suppose we go up to |maxTerms| terms
maxTerms = 50;

% First zeros of the zero-order Bessel function
j0 = besselzero(0, maxTerms); 

j0_squared       = j0.^2;
j0_over_J1_of_j0 = j0./besselj(1, j0);

% This is the variable factor in the infinite sum
scaledTime = linspace(0.00035, .045, 1000);

% What fraction of the sum is accounted for by the last term?
err = scaledTime;
for i =  1:numel(scaledTime)
    ff = exp(-.5 * j0_squared * scaledTime(i)) .* j0_over_J1_of_j0;
    err(i) = abs(ff(maxTerms) / sum(ff(1:maxTerms)));
end

% The sum oscillates -- find highest value of scaled time with excess error
point = find(err > .001 & ~isinf(err), 1, 'last');
fprintf('Highest scaled time with error > 0.1%%: %g\n', scaledTime(point))

% Graph it
plot(scaledTime, (err), '-', ...
     scaledTime(point), (err(point)), 'ro')
grid on

% The jags module uses negative half decision time to branch
nhdt = -.5 * scaledTime(point)