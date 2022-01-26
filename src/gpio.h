#ifndef _GPIO_H_
#define _GPIO_H_

#include <hsf.h>

#ifndef HFPIO_PULLUP
#define HFPIO_PULLUP 0
#endif


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


#define GPIO_INV_LED 1
#define GPIO_INV_RELAY 2


void USER_FUNC hfeasy_gpio_init(void);
void USER_FUNC gpio_deinit(void);
int USER_FUNC gpio_get_state(int fid);
int *gpio_pin(int n);

#endif
