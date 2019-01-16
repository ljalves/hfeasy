#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "hfeasy.h"

struct hfeasy_config {
	/* mqtt server */
	uint32_t	mqtt_server_ip;
	uint16_t	mqtt_server_port;
	
};

struct hfeasy_runtime_config {
	uint8_t has_ip;
	uint8_t mqtt_ready;
};

void USER_FUNC config_init(void);
struct hfeasy_runtime_config* USER_FUNC config_get_rtcfg(void);


#endif
