//-----------------------------------------------------------------------------
// F2xx_Osc_Init.c
//-----------------------------------------------------------------------------
// Copyright 2003 Cygnal Integrated Products, Inc.
//
// AUTH: FB
// DATE: 27 DEC 02
//
// This program shows an example of configuring the internal
// and external oscillators.
//
// Target: C8051F2xx
// Tool chain: KEIL C51 6.03 / KEIL EVAL C51
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f200.h>                 // SFR declarations

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F2xx
//-----------------------------------------------------------------------------

sfr16 DP       = 0x82;                 // data pointer
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window
sfr16 RCAP2    = 0xca;                 // Timer2 capture/reload
sfr16 T2       = 0xcc;                 // Timer2

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_IntOsc_Init (void);
void SYSCLK_Crystal_Init (void);
void SYSCLK_C_RC_Init (void);
void SYSCLK_CMOS_Init (void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) {

   WDTCN = 0xde;                       // disable watchdog timer
   WDTCN = 0xad;

   // select one of the following functions to initialze the system clock
   SYSCLK_IntOsc_Init ();
// SYSCLK_Crystal_Init ();
// SYSCLK_C_RC_Init ();
// SYSCLK_CMOS_Init ();

   while (1);
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// SYSCLK_IntOsc_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency.
//
void SYSCLK_IntOsc_Init (void)
{
   OSCICN = 0x87;                      // Set internal oscillator to
                                       // maximum frequency and enable missing
                                       // clock detector
}

//-----------------------------------------------------------------------------
// SYSCLK_Crystal_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use an 22.1184 MHz crystal
// as its clock source. Assumes a 22.1184 MHz crystal and associated loading
// capacitors are connected at XTAL1 and XTAL2.
//
void SYSCLK_Crystal_Init (void)
{
   int i;                              // delay counter

   OSCXCN = 0x67;                      // start external oscillator with
                                       // 22.1184MHz crystal

   for (i=0; i < 256; i++) ;           // XTLVLD blanking interval (>1ms)

   while (!(OSCXCN & 0x80)) ;          // Wait for crystal osc. to settle

   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
}

//-----------------------------------------------------------------------------
// SYSCLK_C_RC_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use an external RC network
// or a single capacitor as its clock source. Assumes an RC network is
// connected to XTAL1 or a single capacitor is connected to XTAL1 and
// XTAL2.
//
void SYSCLK_C_RC_Init (void)
{
   OSCXCN = 0x47;                      // start external oscillator in
                                       // C/RC mode with XFCN = 7

   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
}

//-----------------------------------------------------------------------------
// SYSCLK_CMOS_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to the external oscillator in
// CMOS clock mode. Assumes a CMOS clock generator is connected to XTAL1.
//
void SYSCLK_CMOS_Init (void)
{
   OSCXCN = 0x20;                      // start external oscillator in
                                       // CMOS clock mode.

   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
}
