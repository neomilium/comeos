#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "drv_keyboard.h"

#include <stdint.h>

typedef struct {
	void		(*fn_init) (void *);
	void		(*fn_event_handler) (const keyboard_event_t);
	void		(*fn_display_preview) (void);
	void		*user_data;
}		application_t;

#endif
