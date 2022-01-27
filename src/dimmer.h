#ifndef _DIMMER_H_
#define _DIMMER_H_

#include <hsf.h>

#define DIMMER_OFF      0x00
#define DIMMER_ON       0xff
#define DIMMER_TOGGLE   0xfe

#define DIMMER_MAX_LEVEL 0x80

void USER_FUNC dimmer_publish_state(void);
void USER_FUNC dimmer_set(uint8_t lvl, uint8_t source);

void USER_FUNC dimmer_init(void);
void USER_FUNC dimmer_deinit(void);

#endif
