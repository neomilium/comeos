#ifndef __BATTERY_H_DRIVER__
#define __BATTERY_H_DRIVER__

#include <stdint.h>

void battery_init(void);
uint8_t battery_get_status(void);

#endif
