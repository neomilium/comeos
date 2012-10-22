#include "app_exposition.h"


#include "lcd.h"
#include "drv_keyboard.h"
#include "windowmanager.h"

static uint16_t _exposition_time=10;

typedef enum {
		HOURS,
		MINUTES,
		SECONDS
} entry_t;

static entry_t _current_entry = HOURS;

void		_app_exposition_display(void);

void
_app_exposition_init(void *data)
{
	_app_exposition_display();
}

void
_app_exposition_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					windowmanager_exit();
					break;
				case KEYBOARD_UP:
					switch (_current_entry){
						case HOURS:
							_exposition_time += 36000;
							break;
						case MINUTES:
							_exposition_time += 600;
							break;
						case SECONDS:
							_exposition_time += 10;
							break;
					}
					_app_exposition_display();
					break;
				case KEYBOARD_DOWN:
					switch (_current_entry){
						case HOURS:
							_exposition_time -= 36000;
							break;
						case MINUTES:
							_exposition_time -= 600;
							break;
						case SECONDS:
							_exposition_time -= 10;
							break;
					}
					_app_exposition_display();

					break;
				case KEYBOARD_RIGHT:
					_current_entry = (_current_entry + 1)%3;
					_app_exposition_display();
					break;
				case KEYBOARD_LEFT:
					_current_entry = (_current_entry + 2)%3;
					_app_exposition_display();
					break;
				default:
					_app_exposition_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_exposition_display(void)
{
	uint16_t exposition_time = _exposition_time;
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);

	if(_current_entry == HOURS) lcd_set_mode(LCD_MODE_INVERTED);
	if(exposition_time/36000 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/36000);
	exposition_time %= 36000;
	lcd_set_mode(LCD_MODE_NORMAL);

	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char(' ');
	lcd_gotoxy(24, 4);
	lcd_display_char(':');
	lcd_set_size(LCD_SIZE_BIG);

	if(_current_entry == MINUTES) lcd_set_mode(LCD_MODE_INVERTED);
	lcd_gotoxy(30, 3);
	if(exposition_time/600 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/600);
	exposition_time %= 600;
	lcd_set_mode(LCD_MODE_NORMAL);

	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char('\'');
	lcd_set_size(LCD_SIZE_BIG);

	if(_current_entry == SECONDS) lcd_set_mode(LCD_MODE_INVERTED);
	if(exposition_time/10 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/10);
	lcd_set_mode(LCD_MODE_NORMAL);

	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char('\'');
	lcd_display_char('\'');
	lcd_gotoxy(54, 4);
	lcd_display_char(' ');
	lcd_gotoxy(84, 4);
	lcd_display_char(' ');
	lcd_display_char(' ');
}

void
_app_exposition_display_preview(void)
{
	uint16_t exposition_time = _exposition_time;
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	if(exposition_time/36000 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/36000);
	exposition_time %= 36000;
	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char(' ');
	lcd_gotoxy(24, 4);
	lcd_display_char(':');
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(30, 3);
	if(exposition_time/600 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/600);
	exposition_time %= 600;
	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char('\'');
	lcd_set_size(LCD_SIZE_BIG);
	if(exposition_time/10 < 10) 	lcd_display_char('0');
	lcd_display_number(exposition_time/10);
	lcd_set_size(LCD_SIZE_NORMAL);
	lcd_display_char('\'');
	lcd_display_char('\'');
	lcd_gotoxy(54, 4);
	lcd_display_char(' ');
	lcd_gotoxy(84, 4);
	lcd_display_char(' ');
	lcd_display_char(' ');
	lcd_set_mode(LCD_MODE_NORMAL);
}

uint16_t exposition_get_value(void)
{
	return _exposition_time;
}

