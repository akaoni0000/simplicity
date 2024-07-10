#include <SI_EFM8SB1_Register_Enums.h>
#include <c8051f000.h> // SFR declarations

//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
sbit LED = P1^2; // Green LED: '1' = ON; '0' = OFF
sbit LED0 = P0^5;

//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
void PORT_Init(void);

//------------------------------------------------------------------------------------
// MAIN Routinemki
//------------------------------------------------------------------------------------
void main (void) {

PORT_Init(); // Set up Crossbar and GPIO

while (1) {
// Infinite loop to keep the program running
LED = 1; // Turn on LED
LED0 = 1;
}
}

//------------------------------------------------------------------------------------
// PORT_Init
//------------------------------------------------------------------------------------
//
// This routine initializes the Crossbar and GPIO pins
//
// Pinout:
// P1.6 - LED (GPIO)
//
void PORT_Init(void)
{
XBR2 = 0x40; // Enable crossbar and weak pull-ups // Enable P1.6 (LED) as a push-pull output
}