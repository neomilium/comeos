#ifndef _APP_SHOTS_H_
#define _APP_SHOTS_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_shots_init()	application_t app_shots = { &_app_shots_init, &_app_shots_event_handler, &_app_shots_display_preview, 0 }

void		_app_shots_init(void *data);
void		_app_shots_event_handler(const keyboard_event_t event);
void		_app_shots_display_preview(void);
uint16_t	shots_get_value(void);
uint16_t	shots_current_value(uint8_t increment);

#endif
