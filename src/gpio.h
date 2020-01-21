#ifndef _GPIO_H_
#define _GPIO_H_

#include <hsf.h>

#define GPIO_BUZZER            (HFGPIO_F_USER_DEFINE+0)
#define GPIO_LED               (HFGPIO_F_USER_DEFINE+0)
#define GPIO_RELAY             (HFGPIO_F_USER_DEFINE+1)
#define GPIO_SWITCH            (HFGPIO_F_USER_DEFINE+2)
#define GPIO_SWITCH_UP         (HFGPIO_F_USER_DEFINE+3)
#define GPIO_SWITCH_DN         (HFGPIO_F_USER_DEFINE+4)
#define GPIO_ZERO_DET          (HFGPIO_F_USER_DEFINE+5)
#define GPIO_LED1         		 (HFGPIO_F_USER_DEFINE+6)
#define GPIO_LED2         		 (HFGPIO_F_USER_DEFINE+7)
#define GPIO_LED3         		 (HFGPIO_F_USER_DEFINE+8)
#define GPIO_LED4         		 (HFGPIO_F_USER_DEFINE+9)
#define GPIO_LED5         		 (HFGPIO_F_USER_DEFINE+10)
#define GPIO_LED6         		 (HFGPIO_F_USER_DEFINE+11)
#define GPIO_LED7         		 (HFGPIO_F_USER_DEFINE+12)


#define RELAY_OFF              0
#define RELAY_ON               1
#define RELAY_TOGGLE           2

#define RELAY_SRC_HTTP         0
#define RELAY_SRC_MQTT         1
#define RELAY_SRC_TIMER        2
#define RELAY_SRC_SWITCH       3
#define RELAY_SRC_SWITCH_UP    4
#define RELAY_SRC_SWITCH_DN    5


void USER_FUNC gpio_init(void);
void USER_FUNC gpio_set_dimmer(uint8_t lvl, uint8_t publish, uint8_t source);
void USER_FUNC gpio_set_relay(uint8_t action, uint8_t publish, uint8_t source);
int USER_FUNC gpio_get_state(int fid);
void USER_FUNC gpio_set_led(uint8_t st);

#endif
