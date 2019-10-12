# Raw data
load h1.mat
u = h1(:, 1);
y = h1(:, 2);

# Filter
[x_filter, y_filter] = moavg2(1:length(u), y, 30);
y = y_filter(1:430) - y_filter(1);
y_filter = [y linspace(y(end), y(end), 100)];
y_filter(1:73) = 0; # Remove bad zeros
u = [linspace(0, 0, 77) linspace(47.8, 47.8, length(y_filter) - 77)];

# Estimation
sysd = okid(u, y_filter, 0.131, 2.75); % Select 10
close all 

# Simulation
u = h1(:, 1);
t = linspace(0, 196, length(u));
figure
lsim(sysd, u', t);
hold on
t = linspace(0, 69, length(y_filter));
plot(t, y_filter);

# Reference gain
sysd = referencegain(sysd);
