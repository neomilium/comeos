#include "drv_pwm1.h"

#include <avr/io.h>

void pwm1_init(void)
{
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);	// Clear OC1A/OC1B on Compare Match (Set output to low level)
	TCCR1B = _BV(WGM13)  | _BV(WGM12)  | _BV(CS10);		// Fast PWM, clkI/O/1 (No prescaling)
	
	ICR1 = 1000;		// TOP = ICR1, freq = 1Mhz/1000 = 1Khz, period = 1ms
}

void pwm1_set_A(const uint16_t ocr)
{
	if( ocr == 0 ) {
		DDRB &= ~(_BV(PB1));
		PORTB &= ~(_BV(PB1));
	} else {
		DDRB |= _BV(PB1);
		PORTB &= ~(_BV(PB1));
	}
	OCR1A = ocr;
}

void pwm1_set_B(const uint16_t ocr)
{
	if( ocr == 0 ) {
		DDRB &= ~(_BV(PB2));
		PORTB &= ~(_BV(PB2));
	} else {
		DDRB |= _BV(PB2);
		PORTB &= ~(_BV(PB2));
	}
	OCR1B = ocr;
}


