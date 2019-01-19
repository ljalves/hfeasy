
#include "hfeasy.h"

struct hfeasy_state state;


#define CONFIG_MAGIC_VER1  0xa1
#define CONFIG_OFFSET      0x00
#define CONFIG_SIZE        (sizeof(struct hfeasy_config))


static void USER_FUNC reboot_timer_handler(hftimer_handle_t timer)
{
	char rsp[50];
	hfat_send_cmd("AT+Z\r\n", sizeof("AT+Z\r\n"), rsp, 64);
}

void USER_FUNC reboot(void)
{
	hftimer_start(hftimer_create("reboot", 1000, false, 0, reboot_timer_handler, 0));
}

static const char *config_page =
	"<!DOCTYPE html><html><head><title>HFeasy config v%d.%d</title></head><body>"\
	"<h1>HFeasy config page</h1>"\
	"<form action=\"/config\" method=\"GET\">"\
	"MQTT server IP: <input type=\"text\" name=\"mqtt_hostname\" value=\"%s\"><br>"\
	"MQTT server port (0=disabled): <input type=\"text\" name=\"mqtt_port\" value=\"%d\"><br>"\
	"Subscribe topic: <input type=\"text\" name=\"mqtt_sub_topic\" value=\"%s\"><br>"\
	"Publish topic: <input type=\"text\" name=\"mqtt_pub_topic\" value=\"%s\"><br>"\
	"ON value: <input type=\"text\" name=\"mqtt_on_value\" value=\"%s\"><br>"\
	"OFF value: <input type=\"text\" name=\"mqtt_off_value\" value=\"%s\"><br>"\
	"<input type=\"submit\" value=\"Update\">"\
	"</form>"\
	"<hr><form action=\"/config\" method=\"GET\"><input type=\"submit\" value=\"Save and reboot\" name=\"save\"></form>"\
	"</body></html>";

static void USER_FUNC httpd_page_config(char *url, char *rsp)
{
	//char mqtt_ip[20];
	char tmp[50];
	int ret;
	
	ret = httpd_arg_find(url, "mqtt_hostname", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_server_hostname, tmp);
	
	ret = httpd_arg_find(url, "mqtt_port", tmp);
	if (ret > 0)
		state.cfg.mqtt_server_port = atoi(tmp);

	ret = httpd_arg_find(url, "mqtt_sub_topic", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_sub_topic, tmp);
	
	ret = httpd_arg_find(url, "mqtt_pub_topic", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_pub_topic, tmp);
	
	ret = httpd_arg_find(url, "mqtt_on_value", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_on_value, tmp);

	ret = httpd_arg_find(url, "mqtt_off_value", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_off_value, tmp);

	ret = httpd_arg_find(url, "save", tmp);
	if (ret > 0) {
		config_save();
		reboot();
	}
	
	sprintf(rsp, config_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					state.cfg.mqtt_server_hostname, state.cfg.mqtt_server_port,
					state.cfg.mqtt_sub_topic, state.cfg.mqtt_pub_topic,
					state.cfg.mqtt_on_value, state.cfg.mqtt_off_value);

	u_printf("page_size=%d\r\n", strlen(rsp));
}


static const char *status_page =
	"<!DOCTYPE html><html><head><title>HFeasy status v%d.%d</title></head><body>"\
	"<h1>HF Easy module status</h1><hr>"\
	"<h2>GPIO status</h2><br>"\
	"Switch: %s<br>Relay: %s<br><hr>"
	"<h2>Connectivity</h2>"\
	"MQTT server: %s"\
  "</body></html>";

static void USER_FUNC httpd_page_status(char *url, char *rsp)
{
	sprintf(rsp, status_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					state.relay_state ? "Closed" : "Open",
					gpio_get_state(GPIO_SWITCH) ? "High" : "Low",
					state.mqtt_ready ? "Connected" : "Disconnected");
}


static int USER_FUNC hfsys_event_callback(uint32_t event_id, void *param)
{
	switch(event_id) {
		case HFE_WIFI_STA_CONNECTED:
			u_printf("wifi sta connected!\r\n");
			break;
			
		case HFE_WIFI_STA_DISCONNECTED:
			state.has_ip = 0;
			u_printf("wifi sta disconnected!\r\n");
			break;
			
		case HFE_DHCP_OK:
			{
				uint32_t *p_ip;
				p_ip = (uint32_t*) param;
				HF_Debug(DEBUG_WARN, "dhcp ok %08X!\r\n", *p_ip);
				u_printf("dhcp ok %08X!", *p_ip);
				state.has_ip = 1;
			}
			break;
		
		case HFE_SMTLK_OK:
			u_printf("smartlink ok!\r\n");
			break;
			
		case HFE_CONFIG_RELOAD:
			u_printf("reload!\r\n");
			break;
			
		default:
			break;
	}
	return 0;
}


struct hfeasy_state* USER_FUNC config_get_state(void)
{
	return &state;
}


static uint8_t USER_FUNC get_macaddr(void)
{
	char *words[3] = {NULL};
	char rsp[64] = {0};
	char tmp[3], *p;
	int ret = 0;
	int i;

	memset(state.mac_addr, 0, sizeof(state.mac_addr));
	state.mac_addr_s[0] = '\0';
	hfat_send_cmd("AT+WSMAC\r\n", sizeof("AT+WSMAC\r\n"), rsp, 64);
	if (hfat_get_words(rsp, words, 2) > 0) {
		if ((rsp[0]=='+') && (rsp[1]=='o') && (rsp[2]=='k')) {
			u_printf("mac = %s\n", words[1]);
			p = words[1];
			strcpy(state.mac_addr_s, p);
			tmp[2] = '\0';
			for (i = 0; i < 6; i++) {
				memcpy(tmp, p, 2);
				p += 2;
				state.mac_addr[i] = strtol(tmp, NULL, 16);
			}
			u_printf("%02x:%02x:%02x:%02x:%02x:%02x",
						(int) state.mac_addr[0], (int) state.mac_addr[1],
						(int) state.mac_addr[2], (int) state.mac_addr[3],
						(int) state.mac_addr[4], (int) state.mac_addr[5]
			);
			ret = 1;
		}
	}
	return ret;
}



void USER_FUNC config_save(void)
{
	hffile_userbin_write(CONFIG_OFFSET, (char*) &state.cfg, CONFIG_SIZE);
	u_printf("saving config to flash. size=%d\r\n", CONFIG_SIZE);
}
	

static void USER_FUNC config_load(uint8_t reset)
{
	memset(&state, 0, sizeof(struct hfeasy_state));
	get_macaddr();
	if (!reset)
		hffile_userbin_read(CONFIG_OFFSET, (char*) &state.cfg, CONFIG_SIZE);
	
	u_printf("loading config from flash. size=%d magic=%02x reset=%d\r\n", CONFIG_SIZE, state.cfg.ver, reset);
	if (state.cfg.ver != CONFIG_MAGIC_VER1) {
		/* init config data */
		state.cfg.ver = CONFIG_MAGIC_VER1;
		mqttcli_initcfg();
		
		hffile_userbin_zero();
		config_save();
	}
	
}


void USER_FUNC config_init(void)
{
	config_load(0);
	
	if(hfsys_register_system_event((hfsys_event_callback_t) hfsys_event_callback) != HF_SUCCESS)
		HF_Debug(DEBUG_ERROR,"error registering system event callback\r\n");

	/* register config webpage */
	httpd_add_page("/config", httpd_page_config);

	httpd_add_page("/status", httpd_page_status);
}
