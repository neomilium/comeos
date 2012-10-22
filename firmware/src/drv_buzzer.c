#include "drv_buzzer.h"

#include "drv_pwm1.h"
#include "scheduler.h"

typedef enum {
	BUZZER_DISABLED,
	BUZZER_ENABLED
} buzzer_mode_t;

static volatile buzzer_mode_t _buzzer_mode;
static volatile uint8_t _buzzer_remaining_pass;

void _buzzer_process(void);

void
buzzer_init(void)
{
	pwm1_set_B(0);
	scheduler_add_hook_fct( _buzzer_process );
}

void 
buzzer_beep(void)
{
	_buzzer_mode = BUZZER_ENABLED;
	_buzzer_remaining_pass += 5;
	pwm1_set_B(1800/4);
}

void
_buzzer_process(void)
{
	switch(_buzzer_mode) {
		case BUZZER_ENABLED:
			_buzzer_remaining_pass--;
			if ( _buzzer_remaining_pass == 0 ) {
				_buzzer_mode = BUZZER_DISABLED;
				pwm1_set_B(0);
			}
		break;
		case BUZZER_DISABLED:
			// nothing to do
		break;
	}
}
