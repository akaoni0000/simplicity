//-----------------------------------------------------------------------------
// PWM16_1.c
//-----------------------------------------------------------------------------
//
// AUTH: BW
//
// Target: C8051F000, F001, F002, F005, F006, F010, F011, or F012
// Tool chain: KEIL C51
//
// Description:
//						Example source code for implementing 16-bit PWM.
//						The PCA is configured in high speed output mode using 
//						SYSCLK/4 as its time base.  <PWM> holds the number of 
//						PCA cycles for the output waveform to remain high.  The 
//						waveform is low for (65536 - PWM) cycles.  The duty 
//						cycle of the output is equal to PWM / 65536.
//
//						Due to interrupt service times, there are minimum and
//						maximum values for PWM, and therefore the duty cycle,
//						depending on interrupt service times.  On the Keil C51
//						compiler (Eval version), the minimum PWM value is 7
//						PCA clocks; the maximum value is 65530.  This equates
//						to a minimum duty cycle of 0.01% and a maximum duty
//						cycle of 99.99%.  This assumes a PCA time base of SYSCLK/4
//						and no other interrupts being serviced.
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f000.h>					// SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define PWM_START		0x4000			// starting value for the PWM high time
sbit		PWM_OUT = P0^0;				// define PWM output port pin

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void main (void);
void PCA_ISR (void);						// PCA Interrupt Service Routine

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
unsigned PWM = PWM_START;				// Number of PCA clocks for waveform
												// to be high
												// duty cycle = PWM / 65536
												// Note: this is a 16-bit value

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
	PCA0CPL0 = (0xff & PWM);			// initialize PCA compare value
	PCA0CPH0 = (0xff & (PWM >> 8));
	PCA0CPM0 = 0x4d;						// CCM0 in High Speed output mode

	EIE1 |= 0x08;							// enable PCA interrupt

	EA = 1;									// Enable global interrupts

	PCA0CN = 0x40;							// enable PCA counter

	while (1) {
		PCON |= 0x01;						// set IDLE mode
	}
}

//-----------------------------------------------------------------------------
// PCA_ISR
//-----------------------------------------------------------------------------
//
// This ISR is called when the PCA CCM0 obtains a match
// PWM_OUT is the CEX0 port pin that holds the state of the current edge:
//  1 = rising edge; 0 = falling edge
// On the rising edge, the compare registers are loaded with PWM_HIGH.
// On the falling edge, the compare registers are loaded with zero.
//
void PCA_ISR (void) interrupt 9 
{
	if (CCF0) {
		CCF0 = 0;							// clear compare indicator
		if (PWM_OUT) {						// process rising edge

			PCA0CPL0 = (0xff & PWM);	// set next match to PWM
			PCA0CPH0 = (0xff & (PWM >> 8));

		} else {								// process falling edge

			PCA0CPL0 = 0;					// set next match to zero
			PCA0CPH0 = 0;

		}
	} else if (CCF1) {					// handle other PCA interrupt sources
		CCF1 = 0;
	} else if (CCF2) {
		CCF2 = 0;
	} else if (CCF3) {
		CCF3 = 0;
	} else if (CCF4) {
		CCF4 = 0;
	} else if (CF) {
		CF = 0;
	}
}
