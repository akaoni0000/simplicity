//------------------------------------------------------------------------------------
// Crossbar1.c
//------------------------------------------------------------------------------------
// Copyright 2002 Cygnal Integrated Products, Inc. 
//
// AUTH: FB
// DATE: 16 DEC 02
// 
// This example shows how to configure the Crossbar.
//
// Target: C8051F00x, C8051F01x
//
// Tool chain: KEIL Eval 'c'
//

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include <c8051f000.h>                    // SFR declarations


//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
sbit  LED = P1^6;                         // green LED: '1' = ON; '0' = OFF

//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
void PORT_Init(void);

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {

   // disable watchdog timer
   WDTCN = 0xde;
   WDTCN = 0xad;
   
   PORT_Init();                           // set up Crossbar and GPIO

   EA = 1;                                // enable global interrupts
   
   LED = 1;                               // turn on LED 

   while (1) {
      PCON |= 0x01;                       // set IDLE mode
   }
}

//------------------------------------------------------------------------------------
// PORT_Init
//------------------------------------------------------------------------------------
//
// This routine initializes the Crossbar and GPIO pins
// 
// Pinout:  
//    P0.0 - UART TX       
//    P0.1 - UART RX
//    P0.2 - /SYSCLK
//    
//    P1.6 - LED (GPIO)
//
void PORT_Init(void)
{
   XBR0 = 0x04;                           // Enable UART
   XBR1 = 0x80;                           // Enable /SYSCLK
   XBR2 = 0x40;                           // Enable crossbar and weak pull-ups
   
   PRT0CF |= 0x05;                        // Enable UART TX and /SYSCLK as push-pull
                                          // outputs
   PRT1CF |= 0x40;                        // Enable P1.6 (LED) as a push-pull output

}