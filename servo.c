/**
*** PIC16F84 file: servo.c -- servo control program for Operation Catapult
*** Written by: Jianjian Song, Rose-Hulman Institute of Technology
*** Modified by: Iain Nash and McCall Saltzman, Catapult Session I 2011
*** Date: 6-27-2011
*** Purpose: Generate multiple (dual) Servo control square wave with an Timer0 interrupt.
**/

#include <pic.h>

#define Servo1ControlPin RD2
#define Servo2ControlPin RD3
#define Servo1ControlPinDirection TRISD2
#define Servo2ControlPinDirection TRISD3

/* Servo Control Defines */
// Interrupt duration
#define INITIAL_COUNT 200 // 128 microseconds per interrupt at 8MHz
// Pulse period
#define PulseMnPeriod 400   // 11.5ms period

// Globals, req'd for the interrupt to change pulse.
unsigned int PULSE1_PERIOD, PULSE1_WIDTH, SERVO1_PLEN = 25;
unsigned int PULSE2_PERIOD, PULSE2_WIDTH, SERVO2_PLEN = 65;

// Prototype
void Timer_Init();

void SetupServo(){
    unsigned char i, count;
    
    // Servo Control Pin are outputs
    Servo1ControlPinDirection = 0;
    Servo2ControlPinDirection = 0;
	
    // Set Servo Power On
	Servo1ControlPin = 1;
	Servo2ControlPin = 1;
	
    // Initialize the Timer and Interrupts
	Timer_Init();
    
}

//Initialize Timer0 for interrupt to generate servo control signal
void Timer_Init(void) {
    //Timer0 Interrupt initialization
	T0CS = 0; // Set prescaler to Timer   
	// Set interrupt source connected to prescaler
	PSA = PS2 = PS1 = PS0 = 0;
    // Timer0 interrupt
	T0IF = 0; T0IE = 1;
	// interrupt occurs every 10 microseconds
	TMR0 = INITIAL_COUNT;
	// turn on global interrupt flag
	GIE = 1;
}


// Interrupt service routine to generate servo control signal
void interrupt ServoDriver1(void){
    if (T0IF==1){	// Has the timer overflowed?  ~100 - 128ms
    
    // For Servo2
    
        if (PULSE2_PERIOD != 0){ 			// If there is a pulse period
    		PULSE2_PERIOD = PULSE2_PERIOD--;// Deinc the period
		} else {
		  PULSE2_PERIOD = PulseMnPeriod;	// Initialize period
		  PULSE2_WIDTH = SERVO2_PLEN;	 	// Initialize pulse
		  Servo2ControlPin = 1;				// Start pulse
		}
		if (PULSE2_WIDTH != 0){ 			// If width
			PULSE2_WIDTH--;	 				// Deincrement it
        } else {
			Servo2ControlPin=0;				// End Pulse
		}
    
    // For Servo1    

		if (PULSE1_PERIOD != 0){ 			// If there is a pulse period
			PULSE1_PERIOD = PULSE1_PERIOD--;
		} else {
		  PULSE1_PERIOD = PulseMnPeriod;	// Initialize period
		  PULSE1_WIDTH = SERVO1_PLEN;		// Initialize pulse
		  Servo1ControlPin = 1;				// Start pulse
		}
		if (PULSE1_WIDTH != 0){
			PULSE1_WIDTH--;	
		} else {
			Servo1ControlPin=0;				// End pulse	
		}
		TMR0=INITIAL_COUNT;
		T0IF=0;
	}
}
