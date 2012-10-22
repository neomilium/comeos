#include "drv_battery.h"

#include <avr/io.h>
#include <avr/interrupt.h>


static volatile uint8_t _battery_value = 4;

ISR (SIG_ADC)
{
// 	_battery_status = (ADCH / 52);
	_battery_value = ADCH;
}

void _battery_start_adc(void)
{
	ADCSRA |= 0xC4;
}

void 
battery_init(void)
{
	ADMUX = 0x60;
	ADCSRA = 0xCF;
	SFIOR = SFIOR & 0x1F;
	_battery_start_adc();
}

uint8_t
battery_get_status(void)
{
	uint8_t _battery_status = (_battery_value - 185) / 8;
	if(_battery_status > 5) {	// wrong value...
		_battery_status = 0;
	}
	_battery_start_adc();
	return _battery_status;	// Warning: we don't send current adc but previously requested !
}
