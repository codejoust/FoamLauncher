/** 
*** File: Robot.h
*** Purpose: Defines / Constants for the robot.c program
*** Author(s): McCall Saltzman and Iain Nash
*** Project: Catapult 2011 ECE24 - SDI
**/

/**
*** Port Assignments:
***   Port C == Ultrasonic (2)
***   Port D == Servos / Fake PWM / Launch Motor (2&4&3)
***   Port E == Warning Led (Ultrasonic) (2)
**/

// Setup Microcontroller
#if defined(_16F877A)
 __CONFIG(HS & WDTDIS & PWRTEN & LVPDIS & UNPROTECT);
#endif

// Make the Motor Control on Servo2
#define MOTOR_PLEN SERVO2_PLEN

// Set + Expose the default PLEN (pulse length) globals
unsigned int SERVO2_PLEN = 35, SERVO1_PLEN = 35;

// From Ultrasonic.c
unsigned long int GetUltrasonicDistance(void);
void DelayTime(unsigned int count);
void SetupUltrasonic(void);

// From Servo.c 
void SetupServo();

// In Main
void SetupPorts();
void SoftStartLaunchMotor();

// Setup range to trigger the discs
#define DistanceThreshold 160

// Pulse width to control rotation of a motor
#define Neutral     35      // 1.5ms 
#define ZeroDegree  25    // 1.1ms or less
#define Degree180   70      // 2.3 ms, 1.75ms or more

// Warning Lights / LEDs
#define WarningLight RE2
#define WarningLightDirection TRISE2

#define MotorLight RE1
#define MotorLightDirection TRISE1

#define PingLight RE0
#define PingLightDirection TRISE0

#define SigPortDirection TRISD1
#define SigPort RD1

// Lauch Motor
#define LaunchMotorDirection TRISD5
#define LaunchMotor RD5

// Pulse width to control rotation of a motor
#define Neutral     35      // ~ 1.5ms 
#define ZeroDegree  25      // ~ 1.1ms or less
#define Degree180   70      // ~ 2.3 ms, 1.75ms or more

#define TriggerPulse 26
#define ReleasePulse 34

// Motor pulse values
#define MotorStop 39
#define MotorMove 60
