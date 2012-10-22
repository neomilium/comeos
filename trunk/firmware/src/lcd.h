#ifndef __LCD_H__
#define __LCD_H__

#include <avr/pgmspace.h>
#include "drv_bf4.h"

#define lcd_init() bf4_init()
#define lcd_clear() bf4_clear()
#define lcd_display_char( X ) bf4_display_char( X )
#define lcd_display_string( X ) bf4_display_string( X )
#define lcd_gotoxy( X, Y ) bf4_gotoxy( X, Y )
#define lcd_finish_line() bf4_finish_line()

#define LCD_MODE_NORMAL		0
#define LCD_MODE_INVERTED	1

#define LCD_SIZE_NORMAL		_BF4_SIZE_NORMAL
#define LCD_SIZE_BIG		_BF4_SIZE_BIG

#define lcd_set_mode(mode) bf4_set_mode(mode)
#define lcd_set_size( size ) bf4_set_size( size )

#define lcd_display_line( X ) bf4_display_string( X ); bf4_finish_line()

#define LCD_SYMBOL_PLAY		0
#define LCD_SYMBOL_STOP		1
#define LCD_SYMBOL_PAUSE	2
#define LCD_SYMBOL_BATTERY	3

#define LCD_SYMBOL_LEVEL	(LCD_SYMBOL_BATTERY + 6)

#define lcd_display_symbol( X ) bf4_display_symbol( X )

void		lcd_display_number(int16_t number);
void		lcd_display_hex(const uint8_t hex);
void		lcd_display_bcd(const uint8_t bcd);
/* void lcd_display_string( const char *string); */

#endif
