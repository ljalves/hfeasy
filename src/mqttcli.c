
#include "hfeasy.h"
#include "mqttlib.h"


#define MQTT_TX_BUF_SIZE	1024
#define MQTT_RX_BUF_SIZE	1024


static struct mqtt_client mqttcli;

static uint8_t *txbuf; /* should be large enough to hold multiple whole mqtt messages */
static uint8_t *rxbuf; /* should be large enough any whole mqtt message expected to be received */


void publish_callback(void** unused, struct mqtt_response_publish *published) 
{
	char* topic_name = (char*) hfmem_malloc(published->topic_name_size + 1);
	char* msg = (char*) hfmem_malloc(published->application_message_size + 1);
	
	memcpy(topic_name, published->topic_name, published->topic_name_size);
	topic_name[published->topic_name_size] = '\0';
	memcpy(msg, published->application_message, published->application_message_size);
	msg[published->application_message_size] = '\0';
	u_printf("Received publish('%s'): %s\n", topic_name, msg);
	
	/* TODO: change gpio state */
	
	hfmem_free(topic_name);
	hfmem_free(msg);
}



int USER_FUNC mqttcli_connect(void)
{
	struct sockaddr_in addr;
	int fd, tmp;
	
	memset((char*)&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	/* TODO: change ip:port to user config */
	addr.sin_port = htons(1883);
	addr.sin_addr.s_addr=inet_addr("192.168.222.226");

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return fd;
	
	tmp=1;
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
	struct hfeasy_runtime_config *rtcfg = config_get_rtcfg();
	
	while(1) {
		//u_printf("mqttcli_thread STATE=%d\r\n", STATE);
		switch(STATE) {
			case 0:
			default:
				/* disconnected */
				if (rtcfg->has_ip)
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
						mqtt_connect(client, "subscribing_client", NULL, NULL, 0, NULL, NULL, 0, 400);
						if (c->error != MQTT_OK) {
							u_printf("error: %s\n", mqtt_error_str(c->error));
							STATE = 49;
						} else {
							STATE = 3;
							rtcfg->mqtt_ready = 1;
						}
					} else {
						msleep(1000);
					}
				}
				break;
				
			case 3:
				/* running state */
				mqtt_sync(c);
				msleep(100);
				break;
			
			case 49:
				close(c->socketfd);
			case 50:
				hfmem_free(txbuf);
			case 51:
				hfmem_free(txbuf);
			case 52:
				rtcfg->mqtt_ready = 0;
				STATE = 0;
				msleep(1000);
				break;
		}
	}
}

void USER_FUNC mqttcli_publish(char *value)
{
	struct hfeasy_config *cfg = config_get_cfg();
	mqtt_publish(&mqttcli, cfg->mqtt_pub_topic, value, strlen(value) , MQTT_PUBLISH_QOS_0);
}


void USER_FUNC mqttcli_initcfg(void)
{
	struct hfeasy_config *cfg = config_get_cfg();

	sprintf(cfg->mqtt_pub_topic, "light");
	sprintf(cfg->mqtt_sub_topic, "light");
	sprintf(cfg->mqtt_on_value, "1");
	sprintf(cfg->mqtt_off_value, "0");
}


void USER_FUNC mqttcli_init(void)
{
	struct hfeasy_runtime_config *rtcfg = config_get_rtcfg();
	struct hfeasy_config *cfg = config_get_cfg();
	
	/* start mqtt thread */
	if (hfthread_create((PHFTHREAD_START_ROUTINE) mqttcli_thread,
					"mqttcli", 256, &mqttcli, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("mqtt sync thread create failed!\n");
	}

	u_printf("waiting for mqtt connection.");
	while (rtcfg->mqtt_ready == 0) {
		u_printf(".");
		msleep(1000);
	}
	u_printf("\r\nready\r\n");
	
	u_printf("mqtt subscribe\n");
	mqtt_subscribe(&mqttcli, cfg->mqtt_sub_topic, 0);

	return;
}
