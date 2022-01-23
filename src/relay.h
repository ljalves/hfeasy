#ifndef _RELAY_H_
#define _RELAY_H_

#include <hsf.h>


#define RELAY_OFF              0
#define RELAY_ON               1
#define RELAY_TOGGLE           2


#define RELAY_SRC_HTTP         0
#define RELAY_SRC_MQTT         1
#define RELAY_SRC_TIMER        2
#define RELAY_SRC_SWITCH       3
#define RELAY_SRC_SWITCH_UP    4
#define RELAY_SRC_SWITCH_DN    5
#define RELAY_SRC_POWERON      6


void USER_FUNC relay_init(void);
void USER_FUNC relay_deinit(void);
void USER_FUNC relay_set(uint8_t action, uint8_t source);
void USER_FUNC relay_publish_state(void);

#endif
