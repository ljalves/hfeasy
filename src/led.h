#ifndef _LED_H_
#define _LED_H_

#include <hsf.h>

#define LED_OFF              0
#define LED_ON               1
#define LED_TOGGLE           2

enum {
	LED_CONFIG_OFF   = 0,
	LED_CONFIG_MQTT  = 1,
	LED_CONFIG_HTTP  = 2,
	LED_CONFIG_ALL   = 3,
	LED_CONFIG_RELAY = 4,
	LED_CONFIG_TOPIC = 5,
	LED_CONFIG_FIND  = 6,
	LED_CONFIG_WIFI  = 7,
	LED_CONFIG_END
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
void USER_FUNC led_deinit(void);
void USER_FUNC led_ctrl(uint8_t led, char *a);
void USER_FUNC leds_ctrl_if(int c, char *a_true, char *a_false);

#endif
