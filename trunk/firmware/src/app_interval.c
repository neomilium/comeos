#include "app_interval.h"


#include "lcd.h"
#include "drv_keyboard.h"
#include "windowmanager.h"

static uint8_t _interval=15;

void		_app_interval_display(void);

void
_app_interval_init(void *data)
{
	_app_interval_display();
}

void
_app_interval_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					windowmanager_exit();
					break;
				case KEYBOARD_UP:
					_interval++;
					if(_interval > 50) _interval=50;
					_app_interval_display();
					break;
				case KEYBOARD_DOWN:
					_interval--;
					if(_interval < 1) _interval=1;
					_app_interval_display();	
					break;
				default:
					_app_interval_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_interval_display(void)
{
	lcd_set_mode(LCD_MODE_INVERTED);
	_app_interval_display_preview();
	lcd_set_mode(LCD_MODE_NORMAL);

}

void
_app_interval_display_preview(void)
{
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	lcd_display_char(' ');
	lcd_display_char(' ');
	lcd_display_number(_interval/10);
	lcd_display_char('.');
	lcd_display_number(_interval%10);
	lcd_display_char('s');
	lcd_finish_line();
	lcd_set_size(LCD_SIZE_NORMAL);
}

uint16_t 
interval_get_value(void)
{
	return _interval;
}

