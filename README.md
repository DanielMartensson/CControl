# CControl

CControl is a library written in C only. The library have the tools and features for using advanced control techniques in a simple way. Instead of using big computers and lots of theory, I have made this library so you can use the following controllers very simple.

```
* Autotuning Predictive Control
* Generalized Predictive Control
* Model Predictive Control
* Linear Quadratic Integral Control
* Gain Scheduling
* Kalman Filter
* Recursive Least Square identification
* Linear Algebra Math
```

All these controller can be implemented onto an microcontroller such as STM32, Arduino, PIC, AVR etc because all is made in portable C code. 

Just download the project and open it with e.g Eclipse IDE. The project have working examples. I will upload some projects here.

## Fan Controller with Linear Quadratic Integral Control

I have created a controller for a fan. The controller works as it read the temperature sensor and it compare the temperature sensor with the potentiometer, which is the reference set point. If the error is large between the temperature sensor and the potentiometer, then the fan is going to turn on high, or low, depending on if the error is negative or positive. 

The goal with this system is that I'm going to implement this on a heat source. The fan is going to blow warm air onto a object and the object is holding the temperature sensor. If the fan is blowing to much warm air, then the controller is going to turn down the speed of the fan so the temperature of the object meets the reference

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/System.jpg)

I have been using Matavecontrol and Mataveid to estimate and creating the kalman gain matrix and the LQR control law with integral action. I'm the author of both Matavecontrol and Mataveid.

Identification process. Here I have using the Observer Kalman Filter Identification and Eigen System Realization methods.

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/Identification%20and%20simulation/Identification.png)

Simulation process:

![a](https://raw.githubusercontent.com/DanielMartensson/CControl/master/Projects/Fan%20Controller/Identification%20and%20simulation/Simulation.png)


