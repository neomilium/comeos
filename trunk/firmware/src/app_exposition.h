#ifndef _APP_EXPOSITION_H_
#define _APP_EXPOSITION_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_exposition_init()	application_t app_exposition = { &_app_exposition_init, &_app_exposition_event_handler, &_app_exposition_display_preview, 0 }

void		_app_exposition_init(void *data);
void		_app_exposition_event_handler(const keyboard_event_t event);
void		_app_exposition_display_preview(void);
uint16_t	exposition_get_value(void);

#endif
