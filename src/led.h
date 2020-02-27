#ifndef _LED_H_
#define _LED_H_

#include <hsf.h>

enum {
	LED_CONFIG_OFF  = 0,
	LED_CONFIG_MQTT = 1,
	LED_CONFIG_HTTP = 2,
	LED_CONFIG_ALL  = 3,
	LED_CONFIG_FIND = 4,
};

enum {
	LED_ACTION_END = 0,
	LED_ACTION_ON = 'n',
	LED_ACTION_OFF = 'f',
	LED_ACTION_DELAY100 = '1',
	LED_ACTION_DELAY200 = '2',
	LED_ACTION_DELAY500 = '5',
	LED_ACTION_DELAY1000 = 's',
	LED_ACTION_REPEAT = 'r',
};

void USER_FUNC led_init(void);
void USER_FUNC led_ctrl(char *a);

#endif
