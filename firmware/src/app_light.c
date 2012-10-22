#include "app_light.h"


#include "lcd.h"

#include "drv_keyboard.h"
#include "drv_light.h"

#include "windowmanager.h"

static int8_t _light_level = 0;

void		_app_light_display(void);

void
_app_light_init(void *data)
{
	_app_light_display();
}

void
_app_light_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					windowmanager_exit();
					break;
				case KEYBOARD_RIGHT:
					_light_level++;
					if(_light_level > 7) _light_level=7;
					light_set_level(_light_level);
					_app_light_display();
					break;
				case KEYBOARD_LEFT:
					_light_level--;
					if(_light_level < 0) _light_level=0;
					light_set_level(_light_level);
					_app_light_display();
					break;
				default:
					_app_light_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_light_display(void)
{
	lcd_set_mode(LCD_MODE_INVERTED);
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	for(int8_t i = 0; i <= _light_level; i++) {
		lcd_display_symbol(LCD_SYMBOL_LEVEL + i);
	}
	lcd_finish_line();
	lcd_set_size(LCD_MODE_NORMAL);
	lcd_set_mode(LCD_MODE_NORMAL);
}

void
_app_light_display_preview(void)
{
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	for(int8_t i = 0; i <= _light_level; i++) {
		lcd_display_symbol(LCD_SYMBOL_LEVEL + i);
	}
	lcd_finish_line();
	lcd_set_size(LCD_MODE_NORMAL);
}
