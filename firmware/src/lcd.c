#include "lcd.h"

void
lcd_display_number(int16_t number)
{
	if (number < 0) {
		lcd_display_char('-');
		number = -number;
	}
	int16_t		modulo = 10000;
	char		display = 0;	/* Don't ignore 0 after first
					 * displayed digits */
	while (modulo != 1) {
		if (display || (number / modulo)) {
			lcd_display_char((number / modulo) + '0');
			display = 1;
		}
		number %= modulo;
		modulo /= 10;
	}
	lcd_display_char((number) + '0');
}

void
lcd_display_hex(const uint8_t hex)
{
	unsigned char	lsb = hex & 0x0F;
	unsigned char	msb = (hex & 0xF0) >> 4;

	lcd_display_char('0');
	lcd_display_char('x');

	if (msb < 0x0A)
		lcd_display_char(msb + '0');
	else
		lcd_display_char((msb - 0x0A) + 'A');

	if (lsb < 0x0A)
		lcd_display_char(lsb + '0');
	else
		lcd_display_char((lsb - 0x0A) + 'A');
}

void
lcd_display_bcd(const uint8_t bcd)
{
	unsigned char	lsb = bcd & 0x0F;
	unsigned char	msb = (bcd & 0xF0) >> 4;

	lcd_display_char(msb + '0');
	lcd_display_char(lsb + '0');
}

