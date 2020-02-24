/* HFeasy

Copyright (c) 2019 Luis Alves

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "hfeasy.h"

#define CONFIG_MAX_TIMERS	10

struct hfeasy_config {
	/* config version */
	uint8_t		ver;
	
	/* mqtt server */
	char      mqtt_server_hostname[50];
	uint16_t  mqtt_server_port;
	char      mqtt_server_user[30];
	char      mqtt_server_pass[30];
	char      mqtt_sub_topic[20];
	char      mqtt_pub_topic[20];
	uint8_t   mqtt_qos;
	char      mqtt_on_value[10];
	char      mqtt_off_value[10];
	
	/* module */
	char      module_name[30];
	char      http_auth;

	/* timers */
	time_t    countdown[2];
	
	struct hfeasy_timer timers[CONFIG_MAX_TIMERS];
	
	//time_t    timer_on[CONFIG_MAX_TIMERS], timer_off[CONFIG_MAX_TIMERS];
	
	uint32_t  log_ptr;
};

struct hfeasy_state {
	uint8_t has_ip;
	uint8_t mqtt_ready;
	uint32_t reset_reason;
	
	uint8_t	relay_state;
	uint8_t relay_modifier;
	
	char mac_addr[6];
	char mac_addr_s[6*2+1];
	
	time_t	countdown[2];
	
	struct hfeasy_config cfg;
};

void USER_FUNC config_init(void);
void USER_FUNC config_save(void);
void USER_FUNC reboot(void);

struct hfeasy_state* USER_FUNC config_get_state(void);


#endif
