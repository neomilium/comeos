#ifndef __COMEOS_H__
#define __COMEOS_H__

#include <stdint.h>

typedef enum {
	COMEOS_MODE_ENABLED,
	COMEOS_MODE_DISABLED,
	COMEOS_MODE_STANDBY
} comeos_mode_t;
volatile comeos_mode_t comeos_mode;

volatile uint16_t comeos_exposition_remaining_time;
volatile uint16_t comeos_interval_remaining_time;
volatile uint16_t comeos_shots_count;

void comeos_start(void);
void comeos_stop(void);
void comeos_standby(void);



#endif /* __COMEOS_H__ */
