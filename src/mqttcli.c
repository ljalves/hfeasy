
#include "hfeasy.h"
#include "mqttlib.h"


#define MQTT_TX_BUF_SIZE	1024
#define MQTT_RX_BUF_SIZE	1024

#define MQTT_SYNC_DELAY		200 /* ms */
#define MQTT_PING_PERIOD	30  /* sec */
#define MQTT_PING_COUNT		(MQTT_PING_PERIOD * (1000 / MQTT_SYNC_DELAY))

static struct mqtt_client mqttcli;

static uint8_t *txbuf; /* should be large enough to hold multiple whole mqtt messages */
static uint8_t *rxbuf; /* should be large enough any whole mqtt message expected to be received */


void publish_callback(void** unused, struct mqtt_response_publish *published) 
{
	struct hfeasy_state *state = config_get_state();
	struct hfeasy_config *cfg = &state->cfg;

	char* topic_name = (char*) hfmem_malloc(published->topic_name_size + 1);
	char* msg = (char*) hfmem_malloc(published->application_message_size + 1);
	
	memcpy(topic_name, published->topic_name, published->topic_name_size);
	topic_name[published->topic_name_size] = '\0';
	memcpy(msg, published->application_message, published->application_message_size);
	msg[published->application_message_size] = '\0';
	u_printf("Received publish('%s'): %s\n", topic_name, msg);
	

	if (strcmp(topic_name, cfg->mqtt_sub_topic) == 0) {
		if (strcmp(cfg->mqtt_on_value, msg) == 0) {
			if (state->relay_state != 1)
				set_relay(1, 0);
		} else if (strcmp(cfg->mqtt_off_value, msg) == 0) {
			if (state->relay_state != 0)
				set_relay(0, 0);
		}
	}
	
	hfmem_free(topic_name);
	hfmem_free(msg);
}



int USER_FUNC mqttcli_connect(void)
{
	struct hfeasy_state *state = config_get_state();
	struct sockaddr_in addr;
	int fd, tmp;
	
	memset((char*)&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	/* TODO: change ip:port to user config */
	addr.sin_port = htons(state->cfg.mqtt_server_port);
	addr.sin_addr.s_addr=inet_addr("192.168.222.226");

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


static void* USER_FUNC mqttcli_thread(void* client)
{
	struct mqtt_client *c = client;
	static uint8_t STATE = 0;
	struct hfeasy_state *state = config_get_state();
	
	while(1) {
		//u_printf("mqttcli_thread STATE=%d\r\n", STATE);
		switch(STATE) {
			case 0:
			default:
				/* disconnected */
				if (state->has_ip && (state->cfg.mqtt_server_port != 0))
					STATE = 1;
				msleep(1000);
				break;
				
			case 1:
				STATE = 2;
				/* alloc rx/tx buffers */
				txbuf = hfmem_malloc(MQTT_TX_BUF_SIZE);
				if (txbuf == NULL) {
					u_printf("failed to alloc tx buf mem\r\n");
					STATE = 52;
				}
				rxbuf = hfmem_malloc(MQTT_RX_BUF_SIZE);
				if (rxbuf == NULL) {
					u_printf("failed to alloc rx buf mem\r\n");
					STATE = 51;
				}
				break;
				
			case 2:
				/* connect to server */
				{
					int fd = mqttcli_connect();
					if (fd >= 0) {
						/* connected, init mqtt */
						mqtt_init(client, fd, txbuf, MQTT_TX_BUF_SIZE, rxbuf, MQTT_RX_BUF_SIZE, publish_callback);
						mqtt_connect(client, state->mac_addr_s, NULL, NULL, 0, NULL, NULL, 0, 400);
						if (c->error != MQTT_OK) {
							u_printf("error: %s\n", mqtt_error_str(c->error));
							STATE = 49;
						} else {
							STATE = 3;
							state->mqtt_ready = 1;
						}
					} else {
						msleep(1000);
					}
				}
				break;
				
			case 3:
				/* running state */
				{
					if (++state->mqtt_ready == MQTT_PING_COUNT) {
						state->mqtt_ready = 1;
						mqtt_ping(&mqttcli);
						u_printf("mqtt ping\r\n");
					}
					
					if (mqtt_sync(c) != MQTT_OK) {
						u_printf("mqtt thread sync error: %s\r\n", mqtt_error_str(c->error));
						STATE = 49;
					}
					msleep(MQTT_SYNC_DELAY);
				}
				break;
			
			case 49:
				close(c->socketfd);
			case 50:
				hfmem_free(txbuf);
			case 51:
				hfmem_free(rxbuf);
			case 52:
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
	mqtt_publish(&mqttcli, state->cfg.mqtt_pub_topic, value, strlen(value) , MQTT_PUBLISH_QOS_0);
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
	struct hfeasy_config *cfg = &state->cfg;
	
	/* start mqtt thread */
	if (hfthread_create((PHFTHREAD_START_ROUTINE) mqttcli_thread,
					"mqttcli", 256, &mqttcli, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("mqtt sync thread create failed!\n");
	}

	u_printf("waiting for mqtt connection.");
	while (state->mqtt_ready == 0) {
		u_printf(".");
		msleep(1000);
	}
	u_printf("\r\nready\r\n");
	
	u_printf("mqtt subscribe to '%s'\r\n", cfg->mqtt_sub_topic);
	mqtt_subscribe(&mqttcli, cfg->mqtt_sub_topic, 0);

	return;
}
