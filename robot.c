/** 
*** File: robot.c
*** Purpose: The main SDI robot.c program
*** Author(s): McCall Saltzman and Iain Nash
*** Project: Catapult 2011 ECE24 - SDI
*/

#include <pic.h>
// Header file for constants / prototypes.
#include "robot.h"

void main(void){
  unsigned long int distance;
  unsigned int spinup_count = 0, reshot_count = 0;
  SetupPorts();
  SetupUltrasonic();
  SetupServo();
  while(1){
    // Rotate Motor
	MOTOR_PLEN = MotorMove;
    while(1){
	  PingLight = 1;
      distance = GetUltrasonicDistance();
      DelayTime(60);
      PingLight = 0;
      if (spinup_count < 2) LaunchMotor = spinup_count = MotorLight = 0;
	  if (spinup_count > 0) spinup_count--;
      if (distance < DistanceThreshold) break;
    }
    MOTOR_PLEN = MotorStop;
    DelayTime(50);
   	WarningLight = 0;
    if (LaunchMotor == 0){
    	SoftStartLaunchMotor(); }
    spinup_count = 100;
    WarningLight = 1;
    SERVO1_PLEN = TriggerPulse;
	DelayTime(400);
    SERVO1_PLEN = ReleasePulse;
    reshot_count = 0;
    while (distance < DistanceThreshold){
	    PingLight = 1;
	    DelayTime(50);
		distance = GetUltrasonicDistance();
		PingLight = 0;
		reshot_count++;
		if (reshot_count > 100) break;
    }
    WarningLight = 0;
  }
  while(1){}
}

void SetupPorts(){
	// Setup Signature Port
	SigPortDirection = 0; SigPort = 1;
	// Setup Launch Motor
	LaunchMotorDirection = LaunchMotor = 0;
	// Setup Warning Light
    WarningLightDirection = WarningLight = 0;
    // Ping Warning Light
    PingLightDirection = PingLight = 0;
    // Motor Startup Light
    MotorLight = MotorLightDirection = 0;    
}	

// Soft Start Routine for the Motor
void SoftStartLaunchMotor(){
	MotorLight = 1;
	unsigned int i;
	for(i = 0; i < 50; i++){
		if (i % 2){
			LaunchMotor = 1;
		} else {
			LaunchMotor = 0;
		}
		DelayTime((int)(i * 1.2) + 30);
	}
	LaunchMotor = 1;
	DelayTime(80);
}
