#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <util/delay.h>

void pwm1_init(void);
void pwm1_set_A(const uint16_t ocr);
void pwm1_set_B(const uint16_t ocr);

#endif //


