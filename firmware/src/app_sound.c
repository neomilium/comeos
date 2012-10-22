#include "app_sound.h"


#include "lcd.h"
#include "drv_keyboard.h"
#include "drv_pwm1.h"
#include "windowmanager.h"

static app_sound_state_t _current_state;

void		_app_sound_display(void);

void
_app_sound_init(void *data)
{
	_app_sound_display();
}

void
_app_sound_event_handler(const keyboard_event_t event)
{
	switch (event.code) {
		case KEYBOARD_EVENT_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_OK:
					windowmanager_exit();
					break;
				case KEYBOARD_RIGHT:
					_current_state = (_current_state + 1)%2;
					_app_sound_display();
					break;
				case KEYBOARD_LEFT:
					_current_state = (_current_state - 1)%2;
					_app_sound_display();
					break;
				default:
					_app_sound_display();
					break;
			}
			break;
		default:
			break;
	}
}

void
_app_sound_display(void)
{
	lcd_set_mode(LCD_MODE_INVERTED);
	_app_sound_display_preview();
	lcd_set_mode(LCD_MODE_NORMAL);
}

void
_app_sound_display_preview(void)
{
	lcd_set_size(LCD_SIZE_BIG);
	lcd_gotoxy(0, 3);
	switch (_current_state) {
		case SOUND_ON:
			lcd_display_line(PSTR("   On"));
			break;
		case SOUND_OFF:
			lcd_display_line(PSTR("   Off"));
			break;
	}

	lcd_set_size(LCD_SIZE_NORMAL);

}


app_sound_state_t
app_sound_get_value(void)
{
	return _current_state;
}
