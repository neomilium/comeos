#ifndef _APP_LIGHT_H_
#define _APP_LIGHT_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_light_init()	application_t app_light = { &_app_light_init, &_app_light_event_handler, &_app_light_display_preview, 0 }

void		_app_light_init(void *data);
void		_app_light_event_handler(const keyboard_event_t event);
void		_app_light_display_preview(void);

#endif
