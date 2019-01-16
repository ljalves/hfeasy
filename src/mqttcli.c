
#include "hfeasy.h"
#include "mqttlib.h"


#define MQTT_TX_BUF_SIZE	1024
#define MQTT_RX_BUF_SIZE	1024


static struct mqtt_client client;

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
	hfmem_free(topic_name);
}

static void* USER_FUNC mqttcli_thread(void* client)
{
	while(1) {
		mqtt_sync((struct mqtt_client*) client);
		msleep(100);
	}
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

void USER_FUNC test_mqtt(void)
{
	int fd;

	msleep(5000);

	fd = mqttcli_connect();
	if (fd < 0)
		return;
	u_printf("mqtt client connected\n");
	
	/* alloc buf mem */
	txbuf = hfmem_malloc(MQTT_TX_BUF_SIZE);
	if (txbuf == NULL) {
		u_printf("failed to alloc tx buf mem\r\n");
		goto err3;
	}
	rxbuf = hfmem_malloc(MQTT_RX_BUF_SIZE);
	if (rxbuf == NULL) {
		u_printf("failed to alloc rx buf mem\r\n");
		goto err2;
	}
	
	mqtt_init(&client, fd, txbuf, MQTT_TX_BUF_SIZE, rxbuf, MQTT_RX_BUF_SIZE, publish_callback);
	mqtt_connect(&client, "subscribing_client", NULL, NULL, 0, NULL, NULL, 0, 400);

	if (client.error != MQTT_OK) {
		u_printf("error: %s\n", mqtt_error_str(client.error));
		goto err1;
	}

	/* start a thread to refresh the client (handle egress and ingree client traffic) */
	if (hfthread_create((PHFTHREAD_START_ROUTINE) mqttcli_thread,
					"mqttcli", 256, &client, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("mqtt sync thread create failed!\n");
		goto err1;
	}
	
	u_printf("mqtt subscribe\n");
	mqtt_subscribe(&client, "light", 0);

	int st = 0;
	while (1) {
		char topic[] = "light";
		char topic2[] = "luz";
		char msg[10];
		sprintf(msg, "%d", st);
		mqtt_publish(&client, topic, msg, strlen(msg) , MQTT_PUBLISH_QOS_0);
		msleep(1000);
		st = st ^ 1;
		sprintf(msg, "%s", st ? "ON" : "OFF");
		mqtt_publish(&client, topic2, msg, strlen(msg) , MQTT_PUBLISH_QOS_0);
		msleep(5000);
	};
	
err1:
	hfmem_free(rxbuf);
err2:
	hfmem_free(txbuf);
err3:
	return;
}


void USER_FUNC mqttcli_init(void)
{
	test_mqtt();
	return;
}
