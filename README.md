# CControl

CControl is a library written in C only. The library have the tools and features for using advanced control techniques in a simple way. Instead of using big computers and lots of theory, I have made this library so you can use the following controllers very simple.

```
* Model Reference Adaptive Control(MRAC) 
* Autotuning Predictive Control(APC) 
* Generalized Predictive Control(GPC)
* Model Predictive Control(MPC)
* Adaptive Model Predictive Control(AMPC)
* Linear Quadratic Integral Control(LQI)
* Kalman Filter(KF)
* Recursive Least Square identification(RLS)
```

All these controller can be implemented onto an microcontroller such as STM32, Arduino, PIC, AVR etc because all are made in portable C code. 

Just download the project and open it with e.g Eclipse IDE. The project have working examples. I will upload some projects here. To start with CControl? Please look at the ReadMe.pdf in src -> CControl -> Documents folder.

### What controller should I select?

The goal is to break down the huge theory in academic control theory an apply them to practical system where nothing is perfect/ideal and nonlinearities are daily standard.

#### Model Reference Adaptive Control(MRAC) 
Use this controller if you are in need of a PI-controller that needs continuously self tuning on P and I. Very simple controller.

#### Autotuning Predictive Control(APC) 
Use this controleller if you are in need of a LQI controller that finds the control law and integral law by it self and you only need to find these laws only once at start up. 

#### Generalized Predictive Control(GPC)
Use this controller is you need to compute continuously the best optimal inputs for the system for every iteration. This controller using Gaussian Elimination with Tikhonov regularization to find its input.

#### Model Predictive Control(MPC)
Use this controller if you have a fix model and you need to continuously the best optimal inputs for the system for every iteration. But instead, you make sure that your inputs cannot result that your output will overshoot over your reference and the inputs cannot be negative. This controller using simplex method with Tikhonov regularization to find its input.

#### Adaptive Model Predictive Control(AMPC)
Same as MPC, but this using Recursive Least Square identification to identify a transfer function and after that creates a discrete state space model. Use this controller if you are in a nonlinear system or only need autotuning inside the controller.

#### Linear Quadratic Integral Control(LQI)
Use this controller if you need multiple inputs and mutiple outputs. This controllers requries offline tuning. But it strench is that what you tune in in your simulation, it going to act very like that in the controller. That's make LQI a very safe controller.

# Fan Controller with Linear Quadratic Integral Control

I have created a controller for a fan. The controller works as it read the temperature sensor and it compare the temperature sensor with the potentiometer, which is the reference set point. If the error is large between the temperature sensor and the potentiometer, then the fan is going to turn on high, or low, depending on if the error is negative or positive. 

The goal with this system is that I'm going to implement this on a heat source. The fan is going to blow warm air onto a object and the object is holding the temperature sensor. If the fan is blowing to much warm air, then the controller is going to turn down the speed of the fan so the temperature of the object meets the reference

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/System.jpg)

I have been using Matavecontrol and Mataveid to estimate and creating the kalman gain matrix and the LQR control law with integral action. I'm the author of both Matavecontrol and Mataveid.

Identification process. Here I have using the Observer Kalman Filter Identification and Eigen System Realization methods.

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/Identification%20and%20simulation/Identification.png)

Simulation process:

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/Identification%20and%20simulation/Simulation.png)


