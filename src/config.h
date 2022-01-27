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
	char      mqtt_topic[30];
	char      mqtt_full_topic[50];
	uint8_t   mqtt_qos;
	char      mqtt_on_value[10];
	char      mqtt_off_value[10];
	
	/* module */
	char      friendly_name[50];
	char      http_auth;
	uint8_t   wifi_led;

	/* gpio */
	int				device;
	int				gpio_config[11];
	
	/* timers */
	time_t    countdown[2];
	
	struct hfeasy_timer timers[CONFIG_MAX_TIMERS];
	
	//time_t    timer_on[CONFIG_MAX_TIMERS], timer_off[CONFIG_MAX_TIMERS];
	
	uint8_t   pwron_state;
	
	
	uint32_t  log_ptr;
};

struct hfeasy_state {
	uint8_t has_ip;
	uint8_t mqtt_ready;
	uint32_t reset_reason;
	
	uint8_t	relay_state;
	uint8_t relay_modifier;

	uint8_t dimmer_level;
	
	char mac_addr[6];
	char mac_addr_s[6*2+1];
	
	time_t	countdown[2];
	
	uint16_t func_state;
	
	struct hfeasy_config cfg;
	
	char module_name[30];
};


#define FUNC_DIMMER    0x0001
#define FUNC_RELAY     0x0002
#define FUNC_BTN_PUSH  0x0004
#define FUNC_BTN_TOG   0x0008
#define FUNC_BTN_UP    0x0010
#define FUNC_BTN_DN    0x0020
#define FUNC_BUZZER    0x0040
#define FUNC_I2C       0x0080
#define FUNC_LED       0x0100



enum {
	DEVICE_CUSTOM = 0,
#if defined(__LPXX00__)
	DEVICE_MODULE,
	DEVICE_PLUG,
	DEVICE_USDIMMER,
	DEVICE_USWALLSW,
	DEVICE_GHOMA,
#elif defined(__LPXX30__)
	DEVICE_PLUG,
#endif
	DEVICE_END
};

void USER_FUNC config_init(void);
void USER_FUNC config_save(void);
void USER_FUNC reboot(void);

struct hfeasy_state* USER_FUNC config_get_state(void);

void log_write(char *s);
int log_printf(const char *fmt, ...);


#endif
