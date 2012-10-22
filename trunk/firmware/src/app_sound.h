#ifndef _APP_SOUND_H_
#define _APP_SOUND_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_sound_init()	application_t app_sound = { &_app_sound_init, &_app_sound_event_handler, &_app_sound_display_preview, 0 }

typedef enum {
		SOUND_ON,
		SOUND_OFF
} app_sound_state_t;


void		_app_sound_init(void *data);
void		_app_sound_event_handler(const keyboard_event_t event);
void		_app_sound_display_preview(void);

app_sound_state_t	app_sound_get_value(void);

#endif
