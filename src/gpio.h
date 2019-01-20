#ifndef _GPIO_H_
#define _GPIO_H_

#include <hsf.h>

#define GPIO_BUZZER            (HFGPIO_F_USER_DEFINE+0)
#define GPIO_LED               (HFGPIO_F_USER_DEFINE+0)
#define GPIO_RELAY             (HFGPIO_F_USER_DEFINE+1)
#define GPIO_SWITCH            (HFGPIO_F_USER_DEFINE+2)


void USER_FUNC gpio_init(void);
void USER_FUNC gpio_set_relay(uint8_t action, uint8_t publish);
int USER_FUNC gpio_get_state(int fid);
void USER_FUNC gpio_set_led(uint8_t st);

#endif
