#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "hfeasy.h"

struct hfeasy_config {
	/* config version */
	uint8_t		ver;
	
	/* mqtt server */
	char	mqtt_server_hostname[50];
	uint16_t	mqtt_server_port;
	char			mqtt_pub_topic[20];
	char			mqtt_sub_topic[20];
	char			mqtt_on_value[10];
	char			mqtt_off_value[10];
};

struct hfeasy_state {
	uint8_t has_ip;
	uint8_t mqtt_ready;
	
	uint8_t	relay_state;
	
	char mac_addr[6];
	char mac_addr_s[6*2+1];
	
	struct hfeasy_config cfg;
};

void USER_FUNC config_init(void);
void USER_FUNC config_save(void);
void USER_FUNC reboot(void);

struct hfeasy_state* USER_FUNC config_get_state(void);


#endif
