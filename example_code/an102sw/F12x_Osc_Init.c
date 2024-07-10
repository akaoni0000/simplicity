//-----------------------------------------------------------------------------
// F12x_Osc_Init.c
//-----------------------------------------------------------------------------
// Copyright 2003 Cygnal Integrated Products, Inc.
//
// AUTH: FB
// DATE: 27 DEC 02
//
// This program shows an example of configuring the internal
// and external oscillators.
//
// Target: C8051F12x
// Tool chain: KEIL C51 6.03 / KEIL EVAL C51
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f120.h>                 // SFR declarations

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------

sfr16 DP       = 0x82;                 // data pointer
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window
sfr16 RCAP2    = 0xca;                 // Timer2 capture/reload
sfr16 RCAP3    = 0xca;                 // Timer3 capture/reload
sfr16 RCAP4    = 0xca;                 // Timer4 capture/reload
sfr16 TMR2     = 0xcc;                 // Timer2
sfr16 TMR3     = 0xcc;                 // Timer3
sfr16 TMR4     = 0xcc;                 // Timer4
sfr16 DAC0     = 0xd2;                 // DAC0 data
sfr16 DAC1     = 0xd2;                 // DAC1 data
sfr16 PCA0CP5  = 0xe1;                 // PCA0 Module 5 capture
sfr16 PCA0CP2  = 0xe9;                 // PCA0 Module 2 capture
sfr16 PCA0CP3  = 0xeb;                 // PCA0 Module 3 capture
sfr16 PCA0CP4  = 0xed;                 // PCA0 Module 4 capture
sfr16 PCA0     = 0xf9;                 // PCA0 counter
sfr16 PCA0CP0  = 0xfb;                 // PCA0 Module 0 capture
sfr16 PCA0CP1  = 0xfd;                 // PCA0 Module 1 capture

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

sbit LED = P1^6;                       // LED='1' means ON
sbit SW1 = P3^7;                       // SW1='0' means switch pressed

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
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Set SFR Page

   OSCICN = 0x83;                      // Set internal oscillator to
                                       // maximum frequency

   CLKSEL = 0x00;                      // Select internal oscillator as
                                       // SYSCLK source

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
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
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Set SFR Page

   OSCXCN = 0x67;                      // start external oscillator with
                                       // 22.1184MHz crystal (XFCN = 7)

   for (i=0; i < 256; i++) ;           // XTLVLD blanking interval (>1ms)

   while (!(OSCXCN & 0x80)) ;          // Wait for crystal osc. to settle


   SFRPAGE = LEGACY_PAGE;
   RSTSRC = 0x04;                      // enable missing clock detector

   SFRPAGE = CONFIG_PAGE;
   CLKSEL = 0x01;                      // select external oscillator as SYSCLK
                                       // source

   OSCICN = 0x00;                      // disable internal oscillator


   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
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
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Set SFR Page

   OSCXCN = 0x47;                      // start external oscillator in
                                       // C/RC mode with XFCN = 7
   SFRPAGE = LEGACY_PAGE;
   RSTSRC = 0x04;                      // enable missing clock detector

   SFRPAGE = CONFIG_PAGE;
   CLKSEL = 0x01;                      // select external oscillator as SYSCLK
                                       // source

   OSCICN = 0x00;                      // disable internal oscillator

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
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
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;              // Set SFR Page

   OSCXCN = 0x20;                      // start external oscillator in
                                       // CMOS clock mode.

   SFRPAGE = LEGACY_PAGE;
   RSTSRC = 0x04;                      // enable missing clock detector

   SFRPAGE = CONFIG_PAGE;
   CLKSEL = 0x01;                      // select external oscillator as SYSCLK
                                       // source

   OSCICN = 0x00;                      // disable internal oscillator

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}
