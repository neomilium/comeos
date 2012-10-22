#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include <stdint.h>

#include "application.h"

#include "drv_keyboard.h"

void		windowmanager_init(void);
void		windowmanager_process_events(void);
void		windowmanager_launch(application_t * app);
void		windowmanager_exit(void);
void		windowmanager_screensaver_disable(void);
void		windowmanager_screensaver_enable(void);
void		windowmanager_one_bar_progress(void);

#endif
