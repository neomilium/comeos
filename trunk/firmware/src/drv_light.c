#include "drv_light.h"

#include "drv_pwm1.h"

static int8_t _light_level = 5;

void
light_init(void)
{
	light_set_level(_light_level);
}

void
light_set_level(const uint8_t level)
{
	_light_level = level;
	pwm1_set_A(144*_light_level); // 144*8 ~= 60% duty cycle.
}

void
light_suspend(void)
{
	pwm1_set_A(0);
}
