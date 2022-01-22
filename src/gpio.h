#ifndef _GPIO_H_
#define _GPIO_H_

#include <hsf.h>

enum {
	GPIO_LED_WIFI = HFGPIO_F_USER_DEFINE,
	GPIO_RELAY,
	GPIO_SWITCH_PUSH,
	GPIO_SWITCH_TOGGLE,
	GPIO_SWITCH_UP,
	GPIO_SWITCH_DN,
	GPIO_BUZZER,
	GPIO_I2C_SCL,
	GPIO_I2C_SDA,
};


void USER_FUNC gpio_init(void);
void USER_FUNC gpio_set_dimmer(uint8_t lvl, uint8_t publish, uint8_t source);
int USER_FUNC gpio_get_state(int fid);

int *gpio_pin(int n);

#endif
