#ifndef _APP_INTERVAL_H_
#define _APP_INTERVAL_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_interval_init()	application_t app_interval = { &_app_interval_init, &_app_interval_event_handler, &_app_interval_display_preview, 0 }

void		_app_interval_init(void *data);
void		_app_interval_event_handler(const keyboard_event_t event);
void		_app_interval_display_preview(void);
uint16_t	interval_get_value(void);

#endif
