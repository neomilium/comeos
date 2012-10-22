#include "comeos.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bit_field.h"

#include "lcd.h"
#include "drv_battery.h"
#include "drv_keyboard.h"
#include "drv_pwm1.h"
#include "drv_light.h"
#include "drv_buzzer.h"

#include "scheduler.h"
#include "windowmanager.h"
#include "menus.h"

/* Applications */
#include "app_mainmenu.h"
#include "app_exposition.h"
#include "app_state.h"
#include "app_shots.h"
#include "app_interval.h"
#include "app_sound.h"
#include "app_light.h"

void comeos_process(void);

int
main(void)
{
	cli();
	DDRB = 0x3F;
	PORTB = 0x00;
	DDRD = 0x03;


	scheduler_init();
	keyboard_init();
	lcd_init();
	battery_init();
	pwm1_init();
	light_init();
	buzzer_init();

	app_exposition_init();
	app_state_init();
	app_shots_init();
	app_interval_init();
	app_sound_init();
	app_light_init();
	app_mainmenu_init();

	comeos_stop();
	scheduler_hook_high_priority_fct(comeos_process);

	windowmanager_init();

	windowmanager_launch(&app_mainmenu);
// 	windowmanager_screensaver_disable();

	scheduler_add_hook_fct(windowmanager_process_events);
	scheduler_resume();

	set_sleep_mode(SLEEP_MODE_IDLE);

	sei();			/* Enable interrupts */

	for(;;) {
		sleep_mode();
	}
	return 1;
}

typedef enum {
	COMEOS_STEP_EXPOSITION,
	COMEOS_STEP_INTERVAL
} comeos_step_t;
static comeos_step_t _comeos_step = COMEOS_STEP_EXPOSITION;

#define SHUNT_RIGHT	GET_BIT(PORTD).bit0
#define SHUNT_LEFT	GET_BIT(PORTD).bit1
#define shot_start()	SHUNT_LEFT = 1
#define shot_stop()	SHUNT_LEFT = 0

void
comeos_start(void)
{
	if( comeos_mode == COMEOS_MODE_DISABLED ) {
		comeos_shots_count = 0;
	}
	comeos_exposition_remaining_time = exposition_get_value();
	shot_start();
	_comeos_step = COMEOS_STEP_EXPOSITION;
	comeos_mode = COMEOS_MODE_ENABLED;
}

void
comeos_stop(void)
{
	shot_stop();
	comeos_exposition_remaining_time = 0;
	comeos_shots_count = 0;
	comeos_interval_remaining_time = 0;
	comeos_mode = COMEOS_MODE_DISABLED;
}

void
comeos_standby(void)
{
	shot_stop();
	comeos_mode = COMEOS_MODE_STANDBY;
}

void
comeos_process(void)
{
	switch(comeos_mode) {
		case COMEOS_MODE_ENABLED:
			switch(_comeos_step) {
				case COMEOS_STEP_EXPOSITION:
					if(comeos_exposition_remaining_time == 0) {
						shot_stop();
						if( app_sound_get_value() == SOUND_ON ) buzzer_beep();
						comeos_shots_count++;
						comeos_interval_remaining_time = interval_get_value();
						_comeos_step = COMEOS_STEP_INTERVAL;
						if(comeos_shots_count >= shots_get_value()) {
							comeos_mode = COMEOS_MODE_DISABLED;
							if( app_sound_get_value() == SOUND_ON ) buzzer_beep();
							if( app_sound_get_value() == SOUND_ON ) buzzer_beep();
						}
					} else {
						comeos_exposition_remaining_time--;
					}
				break;
				case COMEOS_STEP_INTERVAL:
					if(comeos_interval_remaining_time == 0) {
						if(comeos_shots_count < shots_get_value()) {
							comeos_exposition_remaining_time = exposition_get_value();
							shot_start();
							_comeos_step = COMEOS_STEP_EXPOSITION;
						} else {
							comeos_mode = COMEOS_MODE_DISABLED;
						}
					} else {
						comeos_interval_remaining_time--;
					}
				break;
			}
		break;
		case COMEOS_MODE_DISABLED:
			//nothing to do...
		break;
		case COMEOS_MODE_STANDBY:
		break;
	}
}


