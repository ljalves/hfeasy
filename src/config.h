#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "hfeasy.h"

struct hfeasy_config {
	/* config version */
	uint8_t		ver;
	
	/* mqtt server */
	uint8_t		mqtt_enabled;
	uint32_t	mqtt_server_ip;
	uint16_t	mqtt_server_port;
	char			mqtt_pub_topic[20];
	char			mqtt_sub_topic[20];
	char			mqtt_on_value[10];
	char			mqtt_off_value[10];
};

struct hfeasy_runtime_config {
	uint8_t has_ip;
	uint8_t mqtt_ready;
};

void USER_FUNC config_init(void);
void USER_FUNC config_save(void);

struct hfeasy_runtime_config* USER_FUNC config_get_rtcfg(void);
struct hfeasy_config* USER_FUNC config_get_cfg(void);


#endif
