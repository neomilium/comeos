#include "app_shots.h"


#include "lcd.h"
#include "drv_keyboard.h"
#include "windowmanager.h"

static uint16_t _shots = 5;
static uint16_t _entry_shots = 5;

void		_app_shots_display(void);

void
_app_shots_init(void *data)
{
	_app_shots_display();
}

void
_app_shots_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					_shots = _entry_shots;
					windowmanager_exit();
					break;
				case KEYBOARD_UP:
					_entry_shots++;
					if(_entry_shots > 500) _entry_shots=500;
					_app_shots_display();
					break;
				case KEYBOARD_DOWN:
					_entry_shots--;
					if(_entry_shots < 1) _entry_shots=1;
					_app_shots_display();
					break;
				default:
					_app_shots_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_shots_display(void)
{
	lcd_set_mode(LCD_MODE_INVERTED);
	_app_shots_display_preview();
	lcd_set_mode(LCD_MODE_NORMAL);
}

void
_app_shots_display_preview(void)
{
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	lcd_display_char(' ');
	lcd_display_char(' ');
	if(_shots < 100) lcd_display_char(' ');
	lcd_display_number(_entry_shots);
	lcd_finish_line();
	lcd_set_size(LCD_SIZE_NORMAL);
}

uint16_t
shots_get_value(void)
{
	return _shots;
}
