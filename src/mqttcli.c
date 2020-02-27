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

#include "hfeasy.h"
#include "mqttlib.h"


#define MQTT_TX_BUF_SIZE	1024
#define MQTT_RX_BUF_SIZE	1024

#define MQTT_SYNC_DELAY		200 /* ms */
#define MQTT_PING_PERIOD	30  /* sec */
#define MQTT_PING_COUNT		(MQTT_PING_PERIOD * (1000 / MQTT_SYNC_DELAY))


enum {
	MQTTCLI_STATE_IDLE = 0,
	MQTTCLI_STATE_INIT,
	MQTTCLI_STATE_CONNECT,
	MQTTCLI_STATE_RUN,
	MQTTCLI_STATE_DISCONNECT,
	MQTTCLI_STATE_FREERX,
	MQTTCLI_STATE_FREETX,
	MQTTCLI_STATE_RESET
};

static struct mqtt_client mqttcli;

static uint8_t *txbuf; /* should be large enough to hold multiple whole mqtt messages */
static uint8_t *rxbuf; /* should be large enough any whole mqtt message expected to be received */


void publish_callback(void** unused, struct mqtt_response_publish *published) 
{
	struct hfeasy_state *state = config_get_state();
	struct hfeasy_config *cfg = &state->cfg;

	char *topic_name = (char*) hfmem_malloc(published->topic_name_size + 1);
	char *msg = (char*) hfmem_malloc(published->application_message_size + 1);
	
	memcpy(topic_name, published->topic_name, published->topic_name_size);
	topic_name[published->topic_name_size] = '\0';
	memcpy(msg, published->application_message, published->application_message_size);
	msg[published->application_message_size] = '\0';
	u_printf("Received publish('%s'): %s\n", topic_name, msg);
	
	if (strcmp(topic_name, cfg->mqtt_sub_topic) == 0) {
		if (state->cfg.wifi_led & LED_CONFIG_MQTT)
			led_ctrl("n1f"); /* got data = 1 blink */

#if defined(__HFEASY_DIMMER__)
		int lvl = atoi(msg);
		gpio_set_dimmer(lvl, 0, RELAY_SRC_MQTT);
#else
		if (strcmp(cfg->mqtt_on_value, msg) == 0) {
			if (state->relay_state != 1)
				gpio_set_relay(1, 0, RELAY_SRC_MQTT);
		} else if (strcmp(cfg->mqtt_off_value, msg) == 0) {
			if (state->relay_state != 0)
				gpio_set_relay(0, 0, RELAY_SRC_MQTT);
		}
#endif
	}
	hfmem_free(topic_name);
	hfmem_free(msg);
}


int USER_FUNC mqttcli_connect(void)
{
	struct hfeasy_state *state = config_get_state();
	struct sockaddr_in addr;
	ip_addr_t server_addr;
	int fd, tmp;
	
	if (hfnet_gethostbyname(state->cfg.mqtt_server_hostname, &server_addr) != HF_SUCCESS)
		return -1;
	
	if (inet_aton(state->cfg.mqtt_server_hostname, &server_addr) == 0)
		return -1;
	
	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(state->cfg.mqtt_server_port);
	addr.sin_addr.s_addr = server_addr.addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return fd;
	
	tmp = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &tmp, sizeof(tmp)) < 0)
		u_printf("set SO_KEEPALIVE fail\n");

	tmp = 60;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &tmp, sizeof(tmp)) < 0)
		u_printf("set TCP_KEEPIDLE fail\n");

	tmp = 6;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &tmp, sizeof(tmp)) < 0)
		u_printf("set TCP_KEEPINTVL fail\n");

	tmp = 5;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &tmp, sizeof(tmp)) < 0)
		u_printf("set TCP_KEEPCNT fail\n");

	if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(fd);
		return -1;
	}

	/* set non-blocking */
	tmp = 1;
	ioctlsocket(fd, FIONBIO, &tmp);

	return fd;
}


static void USER_FUNC mqttcli_thread(void* client)
{
	struct hfeasy_state *state = config_get_state();
	struct mqtt_client *c = (struct mqtt_client*) client;
	static uint8_t STATE = 0;
	
	while(1) {
		//u_printf("mqttcli_thread STATE=%d\r\n", STATE);
		switch(STATE) {
			case MQTTCLI_STATE_IDLE:
			default:
				/* disconnected */
				if (state->has_ip && (state->cfg.mqtt_server_port != 0))
					STATE = MQTTCLI_STATE_INIT;
				msleep(1000);
				break;
				
			case MQTTCLI_STATE_INIT:
				STATE = MQTTCLI_STATE_CONNECT;
				/* alloc rx/tx buffers */
				txbuf = hfmem_malloc(MQTT_TX_BUF_SIZE);
				if (txbuf == NULL) {
					u_printf("failed to alloc tx buf mem\r\n");
					STATE = MQTTCLI_STATE_RESET;
				}
				rxbuf = hfmem_malloc(MQTT_RX_BUF_SIZE);
				if (rxbuf == NULL) {
					u_printf("failed to alloc rx buf mem\r\n");
					STATE = MQTTCLI_STATE_FREETX;
				}
				break;
				
			case MQTTCLI_STATE_CONNECT:
				/* connect to server */
				{
					int fd = mqttcli_connect();
					if (fd >= 0) {
						char *user = strlen(state->cfg.mqtt_server_user) == 0 ? NULL : state->cfg.mqtt_server_user;
						char *pass = strlen(state->cfg.mqtt_server_pass) == 0 ? NULL : state->cfg.mqtt_server_pass;
						/* connected, init mqtt */
						mqtt_init(client, fd, txbuf, MQTT_TX_BUF_SIZE, rxbuf, MQTT_RX_BUF_SIZE, publish_callback);
						mqtt_connect(client, state->mac_addr_s, NULL, NULL, 0, user, pass, 0, 400);
						if (c->error != MQTT_OK) {
							u_printf("error: %s\n", mqtt_error_str(c->error));
							STATE = MQTTCLI_STATE_DISCONNECT;
						} else {
							STATE = MQTTCLI_STATE_RUN;
							state->mqtt_ready = 1;
							if (strlen(state->cfg.mqtt_sub_topic) > 0) {
								u_printf("mqtt subscribe to '%s'\r\n", state->cfg.mqtt_sub_topic);
								mqtt_subscribe(&mqttcli, state->cfg.mqtt_sub_topic, state->cfg.mqtt_qos);
							}
						}
					} else {
						msleep(1000);
					}
				}
				break;
				
			case MQTTCLI_STATE_RUN:
				/* running state */
				{
					if (state->mqtt_ready == 0) {
						/* disconnect */
						if (state->cfg.wifi_led & LED_CONFIG_MQTT)
							led_ctrl("n1f1n1fsr");	/* disconnected = 2 blinks, 1sec interval */
						STATE = MQTTCLI_STATE_DISCONNECT;
					} else if (++state->mqtt_ready == MQTT_PING_COUNT) {
						/* mqtt ping */
						if (state->cfg.wifi_led & LED_CONFIG_MQTT)
							led_ctrl("n1f1n1f1n1f"); /* ping = 3 blinks */
						state->mqtt_ready = 1;
						mqtt_ping(&mqttcli);
					}
					
					if (mqtt_sync(c) != MQTT_OK) {
						u_printf("mqtt thread sync error: %s\r\n", mqtt_error_str(c->error));
						STATE = MQTTCLI_STATE_DISCONNECT;
					}
					msleep(MQTT_SYNC_DELAY);
				}
				break;
			
			case MQTTCLI_STATE_DISCONNECT:
				close(c->socketfd);
			case MQTTCLI_STATE_FREERX:
				hfmem_free(rxbuf);
			case MQTTCLI_STATE_FREETX:
				hfmem_free(txbuf);
			case MQTTCLI_STATE_RESET:
				state->mqtt_ready = 0;
				STATE = 0;
				msleep(1000);
				break;
		}
	}
}

void USER_FUNC mqttcli_publish(char *value)
{
	struct hfeasy_state *state = config_get_state();
	uint8_t flags;
	switch (state->cfg.mqtt_qos) {
		default:
		case 0:
			flags = MQTT_PUBLISH_QOS_0;
			break;
		case 1:
			flags = MQTT_PUBLISH_QOS_1;
			break;
		case 2:
			flags = MQTT_PUBLISH_QOS_2;
			break;
	}
	if (state->mqtt_ready) {
		mqtt_publish(&mqttcli, state->cfg.mqtt_pub_topic, value, strlen(value), flags);
		if (state->cfg.wifi_led & LED_CONFIG_MQTT)
			led_ctrl("n1f1n1f"); /* publish = 2 blinks */
	}
}


void USER_FUNC mqttcli_initcfg(void)
{
	struct hfeasy_state *state = config_get_state();

	sprintf(state->cfg.mqtt_pub_topic, "hf%s", state->mac_addr_s);
	sprintf(state->cfg.mqtt_sub_topic, "hf%s", state->mac_addr_s);
	sprintf(state->cfg.mqtt_on_value, "1");
	sprintf(state->cfg.mqtt_off_value, "0");
}


void USER_FUNC mqttcli_init(void)
{
	struct hfeasy_state *state = config_get_state();
	
	/* start mqtt thread */
	if (hfthread_create((PHFTHREAD_START_ROUTINE) mqttcli_thread,
					"mqttcli", 256, &mqttcli, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("mqtt sync thread create failed!\n");
	}

	return;
}
