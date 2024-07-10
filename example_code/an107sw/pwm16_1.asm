;-----------------------------------------------------------------------------
;	CYGNAL INTEGRATED PRODUCTS, INC.
;
;
; 	FILE NAME  	:	pwm16_1.ASM 
; 	TARGET MCU	:	C8051F000, F001, F002, F005, F006, F010, F011, or F012
; 	DESCRIPTION	:	Example source code for implementing 16-bit PWM.
;						The PCA is configured in high speed output mode using 
;						SYSCLK/4 as its time base.  PWM holds the number of 
;						PCA cycles for the output waveform to remain high.  The 
;						waveform is low for (65536 - PWM) cycles.  The duty 
;						cycle of the output is equal to PWM / 65536.
;
;						Due to interrupt service times, the minimum value for
;						PWM is 7 PCA cycles, and the maximum value is 65529.
;						This equates to a minimum duty cycle of 0.01068% and a
;						maximum duty cycle of 99.9893%.
;
;						If the PCA time base is changed to SYSCLK / 12, the min and
;						max values for PWM change to 3 and 65533 respectively,
;						for min and max duty cycles of 0.0046% and 99.9954%
;						respectively.
;
;						Processing the rising edge interrupt handler takes 18 cycles.
;						Processing the falling edge interrupt handler takes 19 cycles.
;
;						One interrupt handler is called for each edge, and there are
;						2 edges for every 65536 PCA clocks.  Using SYSCLK / 4 as the
;						PCA time base, that means that 37 cycles are consumed for
;						edge maintenance for every (65536 * 4) = 262,144 SYSCLK
;						cycles, not counting vectoring the interrupt.
;						CPU utilization is (37 / 262,144)*100% = 0.0141%
;
;						Using SYSCLK / 12 as the PCA timebase, 37 cycles are 
;						consumed for edge maintenance for every (65536 * 12) = 
;						786,432 SYSCLK cycles.  CPU utilization is (37 / 786,432)
;						= 0.0047%.
;
;						The period of the waveform is 65536 PCA clocks.  Using
;						SYSCLK / 4 as the PCA time base, the period is 262,144 SYSCLK
;						cycles.  Using the default internal oscillator at 2MHz, the
;						period is 131ms (7.6Hz).  Using the 16MHz internal
;						oscillator (as in this example), the period is 16.4us 
;						(61 Hz).
;
;						Using SYSCLK / 12 as the PCA time base, the period is
;						65536 * 12 = 786,432 SYSCLK cycles.  Using the default
;						internal oscillator at 2MHz, the period is 393ms (2.5Hz).
;						Using the 16MHz internal oscillator, the period is 49.2ms
;						(20Hz).
;
;						In this example, the output is routed to P0.0, which is
;						also labeled 'PWM_OUT'.
;
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; EQUATES
;-----------------------------------------------------------------------------

$MOD8F000

PWM		EQU	32768							; Number of PCA clocks for waveform
													;  to be high
													; duty cycle = PWM / 65536
													; max = 65529 (99.9893% duty cycle)
													; min = 7 (0.01068% duty cycle)
													; Note: this is a 16-bit constant

PWM_OUT	EQU	P0.0							; define PWM output port pin

;-----------------------------------------------------------------------------
; RESET AND INTERRUPT VECTOR TABLE
;-----------------------------------------------------------------------------

CSEG
		org	00h					
		ljmp	Main

		org	04bh
		ljmp	PCA_ISR							; PCA Interrupt Service Routine

;-----------------------------------------------------------------------------
; MAIN PROGRAM CODE
;-----------------------------------------------------------------------------
		org	0b3h								; start at end of interrupt handler
													; space
Main:
		; Disable watchdog timer
		mov	WDTCN, #0deh		
		mov	WDTCN, #0adh

		;	Enable the Internal Oscillator at 16 MHz
		mov	OSCICN, #07h

		; Enable the Crossbar, weak pull-ups enabled
		mov	XBR0, #08h						; route CEX0 to P0.0
		mov	XBR2, #40h		
		
		orl	PRT0CF, #01h					; Configure P0.0

		; Configure the PCA
		mov	PCA0MD, #02h					; disable cf interrupt, 
													; PCA time base = SYSCLK/4
		mov	PCA0CPL0, #LOW(PWM)			; initialize the PCA compare value 
		mov	PCA0CPH0, #HIGH(PWM)			
		mov	PCA0CPM0, #4dh					; CCM0 in High Speed output mode
		
		; Enable interrupts
		orl	EIE1,	#08h						; Enable PCA interrupt
		setb	EA									; Enable global interrupts

		mov	PCA0CN, #40h					; enable PCA counter

		jmp	$

;-----------------------------------------------------------------------------
;	CCF0 Interrupt Vector
;
;
; This ISR is called when the PCA CCM0 obtains a match
; PWM_OUT is the CEX0 port pin that holds the state of the current edge:
;  1 = rising edge; 0 = falling edge
; On the rising edge, the compare registers are loaded with PWM_HIGH.
; On the falling edge, the compare registers are loaded with zero.

PCA_ISR:
		jbc	CCF0, CCF0_HNDL				; handle CCF0 comparison
		jbc	CCF1, PCA_STUB					; stub routines
		jbc	CCF2, PCA_STUB
		jbc	CCF3, PCA_STUB
		jbc	CCF4, PCA_STUB
		jbc	CF,   PCA_STUB

PCA_STUB:
PCA_ISR_END:
		reti

CCF0_HNDL:
		jnb	PWM_OUT, CCF0_FALL
													; handle rising edge	
CCF0_RISE:
		mov	PCA0CPL0, #LOW(PWM)
		mov	PCA0CPH0, #HIGH(PWM)

		reti
		
CCF0_FALL:										; handle falling edge
		mov	PCA0CPL0, #00
		mov	PCA0CPH0, #00

		reti

; rising edge takes 4+3+11 = 18 cycles
; falling edge takes 4+4+11 = 19 cycles

;-----------------------------------------------------------------------------
; END
;-----------------------------------------------------------------------------

END
