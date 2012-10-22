#ifndef _APP_STATE_H_
#define _APP_STATE_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_state_init()	application_t app_state = { &_app_state_init, &_app_state_event_handler, &_app_state_display_preview, 0 }

void		_app_state_init(void *data);
void		_app_state_event_handler(const keyboard_event_t event);
void		_app_state_display_preview(void);

#endif
