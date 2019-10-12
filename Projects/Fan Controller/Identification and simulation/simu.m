# LQI
[L, Li] = lqi(sysd, eye(10+1), 1);

# LQE
K = lqe(sysd, eye(10), 1);


# Parameters
r = 25;
y = 0;
u = 0;
x = zeros(size(sysd.A, 1), 1);
xn = 0;
Y = [];
U = [];
X = [];

# Simulation time
t = linspace(0, 131, 1001);

# Do simulation now
for i = 1:1001
  # Control law - LQGI
  u = Li/(1-0.1)*r - (L*x - Li*xn);

  if(u < 0)
    u = 0;
  end
  if(u > 100)
    u = 100;
  end
  U(i) = u;
  
  if(r > y)
    xn = xn + r - y;
  end
  
  
  # Model
  y = sysd.C*x + sysd.D*u; % Real-life measurement
  Y(i) = y;
  
  # Kalman 
  x = sysd.A*x - K*sysd.C*x + sysd.B*u + K*y;
 
end
u
y
xn
plot(t, Y, t, U);
