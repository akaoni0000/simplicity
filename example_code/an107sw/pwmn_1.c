//-----------------------------------------------------------------------------
// PWMn_1.c
//-----------------------------------------------------------------------------
//
// AUTH: BW
//
// Target: C8051F000, F001, F002, F005, F006, F010, F011, or F012
// Tool chain: KEIL C51
//
// Description:
//						Example source code for implementing an n-bit PWM.
//						The PCA is configured in high speed output mode using 
//						SYSCLK/4 as its time base.  <PWM_HIGH> holds the number of 
//						PCA cycles for the output waveform to remain high.
//						<PWM_LOW> holds the number of PCA cycles for the output
//						waveform to remain low.  The duty cycle of the output
//						is equal to PWM_HIGH / (PWM_HIGH + PWM_LOW).
//
//						Due to interrupt service times, there are minimum and
//						maximum values for PWM_HIGH and PWM_LOW, and therefore 
//						the duty cycle, depending on interrupt service times.
//						Regardless ofthe efficiency of the compiler, duty 
//						cycles between	1% and 99% should be very easy to achieve.
//
//						With the eval version of the Keil compiler, the minimum
//						high and low counts are 20 PCA cycles each (max frequency
//						is about 100kHz w/ 16MHz internal SYSCLK).  This assumes
//						no other interrupts being serviced, and PCA time base is
//						SYSCLK / 4.
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f000.h>								// SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define PWM_START		0x8000						// starting value for the 
															// PWM_HIGH time and PWM_LOW time
sbit		PWM_OUT = P0^0;							// define PWM output port pin

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void main (void);
void PCA_ISR (void);									// PCA Interrupt Service Routine

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
unsigned PWM_HIGH = PWM_START;					// Number of PCA clocks for
															// waveform to be high
unsigned PWM_LOW = ~PWM_START;					// Number of PCA clocks for 
															// waveform to be low
															// duty cycle = 
															// PWM_HIGH / (PWM_HIGH + PWM_LOW)

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) {

	WDTCN = 0xde;										// Disable watchdog timer
	WDTCN = 0xad;

	OSCICN = 0x07;										// set SYSCLK to 16MHz,
															// internal osc.

	XBR0 = 0x08;										// enable CEX0 at P0.0
	XBR2 = 0x40;										// enable crossbar and weak 
															// pull-ups

	PRT0CF = 0x01;										// set P0.0 output mode to 
															// push-pull
	PRT1CF = 0x20;										// set P1.6 output to 
															// push-pull (LED)

	
	// configure the PCA
	PCA0MD = 0x02;										// disable CF interrupt
															// PCA time base = SYSCLK / 4
	PCA0CPL0 = (0xff & PWM_HIGH);					// initialize PCA compare value
	PCA0CPH0 = (0xff & (PWM_HIGH >> 8));
	PCA0CPM0 = 0x4d;									// CCM0 in High Speed output mode

	EIE1 |= 0x08;										// enable PCA interrupt

	EA = 1;												// Enable global interrupts

	PCA0CN = 0x40;										// enable PCA counter

	while (1) {
		PCON |= 0x01;									// set IDLE mode
	}
}

//-----------------------------------------------------------------------------
// PCA_ISR
//-----------------------------------------------------------------------------
//
// This ISR is called when the PCA CCM0 obtains a match
// PWM_OUT is the CEX0 port pin that holds the state of the current edge:
//  1 = rising edge; 0 = falling edge
// On the rising edge, the compare registers are updated to trigger for the
// next falling edge.
// On the falling edge, the compare registers are updated to trigger for the
// next rising edge.
//
void PCA_ISR (void) interrupt 9 
{
	unsigned temp;										// holding value for 16-bit math

	if (CCF0) {
		CCF0 = 0;										// clear compare indicator
		if (PWM_OUT) {									// process rising edge

			// update compare match for next falling edge
			temp = (PCA0CPH0 << 8) | PCA0CPL0;	// get current compare value
			temp += PWM_HIGH;							// add appropriate offset

			PCA0CPL0 = (0xff & temp);				// replace compare value
			PCA0CPH0 = (0xff & (temp >> 8));

		} else {											// process falling edge

			// update compare match for next rising edge
			temp = (PCA0CPH0 << 8) | PCA0CPL0;	// get current compare value
			temp += PWM_LOW;							// add appropriate offset

			PCA0CPL0 = (0xff & temp);				// replace compare value
			PCA0CPH0 = (0xff & (temp >> 8));

		}
	} else if (CCF1) {								// handle other PCA interrupt
		CCF1 = 0;										// sources
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
