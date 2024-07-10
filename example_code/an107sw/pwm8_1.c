//-----------------------------------------------------------------------------
// PWM8_1.c
//-----------------------------------------------------------------------------
//
// AUTH: BW
//
// Target: C8051F000, F001, F002, F005, F006, F010, F011, or F012
// Tool chain: KEIL C51
//
// Description:
//						Example source code for implementing 8-bit PWM.
//						The PCA is configured in 8-bit PWM mode using 
//						SYSCLK/4 as its time base.  <PWM> holds the number of 
//						PCA cycles for the output waveform to remain low per 256-
//						count period.  The waveform is high for (256 - PWM) cycles.
//						The duty cycle of the output is equal to (256 - PWM) / 256.
//
//						Because the 8-bit PWM is handled completely in hardware,
//						no CPU cycles are expended in maintaining a fixed duty
//						cycle.  Altering the duty cycle requires a single 8-bit 
//						write to the high byte of the module's compare register,
//						PCA0CP0H, in this example.
//
//						Achievable duty cycle ranges are 0.38% (PCA0CP0H = 0xff) 
//						to 100% (PCA0CP0H = 0x00).
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f000.h>					// SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define PWM				0x80				// Number of PCA clocks for waveform
												// to be low
												// duty cycle = (256 - PWM) / 256
												// Note: this is an 8-bit value

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void main (void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) {

	WDTCN = 0xde;							// Disable watchdog timer
	WDTCN = 0xad;

	OSCICN = 0x07;							// set SYSCLK to 16MHz, internal osc.

	XBR0 = 0x08;							// enable CEX0 at P0.0
	XBR2 = 0x40;							// enable crossbar and weak pull-ups

	PRT0CF = 0x01;							// set P0.0 output state to push-pull
	PRT1CF = 0x20;							// set P1.6 output to push-pull (LED)

	
	// configure the PCA
	PCA0MD = 0x02;							// disable CF interrupt
												// PCA time base = SYSCLK / 4
	PCA0CPL0 = PWM;						// initialize PCA PWM value			
	PCA0CPH0 = PWM;
	PCA0CPM0 = 0x42;						// CCM0 in 8-bit PWM mode
	PCA0CN = 0x40;							// enable PCA counter

	while (1) {
		PCON |= 0x01;						// set IDLE mode
	}
}