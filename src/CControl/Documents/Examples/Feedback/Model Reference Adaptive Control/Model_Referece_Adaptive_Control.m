% This is a simple adaptive PI-controller, but it goes under the namelengthmax
% Model Reference Adaptive Control
% I just have removed the reference model and set it to 1. Still a model.

function [u] = Model_Referece_Adaptive_Control()
  
   % Initial parameters
   r = 51; % Set point
   y = 7; % Current output
   u = 0; % Our input 
   K1 = 3.1; % Current I gain
   K2 = 4.1; % Current P gain
   LEARNING = 0.0001;
   
   e = y-r;
   K1 = K1 + r*e; % I-gain in practical view
   K2 = K2 + y*e; % P-gain in practical view
   u = -LEARNING*r*(K1) - LEARNING*y*(K2);
end
