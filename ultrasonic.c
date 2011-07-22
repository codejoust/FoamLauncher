// File Name: Ultrasonic.h
// Date:  July 16, 2008
// Purpose: Interface with Ping from Parallax
//  to show ultrasonic sensor and input capture
// Header File
// Author: Jianjian Song
// Modified by: Iain Nash && McCall Saltzman
// Hardware && Debugging: Sarah Healy

#include <pic.h>

//define all Ultrasonic hardware interface pins
#define TriggerAndEchoPin RC2 //input caputure CCP1
#define PinDirection TRISC2 //needs to be switched from input to output, etc.

#define WarningLight RE2

#define WarningLightDirection TRISE2
#define FlashingLED RC4 //constantly flashing LED
#define FlashingLED_DIRECTION TRISC4=0


// Timing setup for ultrasonic
#define TenMicroSecond 50 //312 usecond
#define TenMilliSecond 200 //

// Ultrasonic Calculation Constants
#define Frequency 20 //MHz
#define SpeedOfSound 35 //centimeters per milliseccond at 25 degrees Celsius
#define Timer1ClockPeriod 0.4 //usec 4/Frequency


unsigned long int GetUltrasonicDistance(void);
void DelayTime(unsigned int count);
void SetupUltrasonic(void);


/****************************
** Pin RC2/CCP1, set as input
** capture 16-bit Timer1 count in CCPR1H:CCPR1L
** Control register CCP1CON
** Four modes
** CCP1M3-0==0B1000: every falling edge
** CCP1M3-0==0B1001: every rising edge
** CCP1M3-0==0B1010: every 4th rising edge
** CCP1M3-0==0B1011: every 16th rising edge
** Flag CCP1IF of pin 0 in PIR1. It must be cleared in software.
*******************************/
void SetupUltrasonic(void) {
  T1CKPS1 = 0;
  T1CKPS0 = 1;
  TMR1CS = 0; // Internal clock (Fosc/4)
  TMR1ON = 1; // Turn on Timer 1
  TRISE = 0;
  TRISC2 = 0;
}

void DelayTime(unsigned int count) {
 unsigned int i, j;
 for(i=0;i<count;i++){for(j=0;j<count;j++){}}
}

unsigned long int GetUltrasonicDistance(void){
  unsigned int range, first_edge, second_edge;
  unsigned long int distance, count;
  PinDirection = 0; // Output to start sending ultrasonic sound 
  TRISC2 = 0;
  TriggerAndEchoPin = 0;
  DelayTime(TenMicroSecond);
  TriggerAndEchoPin = 1; // Generate a pulse
  DelayTime(TenMicroSecond); // Minimum pulse width
  TriggerAndEchoPin = 0; // Turn off the pulse to start sending sound
  PinDirection = 1; // Switch to input to receive echo
  TRISC2 = 1;
  CCP1IF = 0;
  CCP1CON = 0b00000101; // Input capture on every rising edge
  // Wait for rising edge
  while(CCP1IF==0){}
  first_edge = CCPR1H;
  first_edge = first_edge<<8;
  first_edge = first_edge + CCPR1L;
  CCP1IF = 0;
  CCP1CON = 0b00000100; //Input capture on every falling edge
  //DelayTime(TenMicroSecond); /minimum pulse width
  while(CCP1IF==0){} //waiting for falling edge
  second_edge = CCPR1H;
  second_edge = second_edge<<8;
  second_edge = second_edge+CCPR1L;
  CCP1IF = 0;
  if(first_edge < second_edge)
    count = second_edge-first_edge;
   else {
    count = -first_edge;
    count = count+second_edge;
   }
   distance = count * Timer1ClockPeriod;
   distance = distance>>1;
   distance = distance * SpeedOfSound;
   distance = distance>>10; //in centimeters
   return distance;
}
