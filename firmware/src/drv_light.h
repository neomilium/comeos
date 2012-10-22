#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdint.h>

void light_init(void);
void light_suspend(void);
void light_set_level(const uint8_t level);

#define light_resume() light_init()

#endif
