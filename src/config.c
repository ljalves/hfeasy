
#include "hfeasy.h"


struct hfeasy_config hfcfg;
struct hfeasy_runtime_config hfrtcfg;



static const char *config_page =
"<!DOCTYPE html><html><head><title>HFeasy config page</title></head><body>"\
"<h1>HFeasy config page</h1>"\
"<form action=\"/config\" method=\"GET\">"\
"MQTT server IP: <input type=\"text\" name=\"mqtt_ip\" value=\"%s\"><br>"\
"MQTT server port: <input type=\"text\" name=\"mqtt_port\" value=\"%s\"><br>"\
"Subscribe topic: <input type=\"text\" name=\"mqtt_sub_topic\" value=\"%s\"><br>"\
"Publish topic: <input type=\"text\" name=\"mqtt_pub_topic\" value=\"%s\"><br>"\
"<input type=\"submit\" value=\"Save\">"\
"</form>"\
"</body></html>";



static void USER_FUNC httpd_page_config(char *url, char *rsp)
{
	char mqtt_ip[20];
	char mqtt_port[10];
	int ret;
	
	ret = httpd_arg_find(url, "mqtt_ip", mqtt_ip);
	if (ret <= 0)
		sprintf(mqtt_ip, "%d.%d.%d.%d",
				hfcfg.mqtt_server_ip & 0xff,
				(hfcfg.mqtt_server_ip >> 8) & 0xff,
				(hfcfg.mqtt_server_ip >> 16) & 0xff,
				(hfcfg.mqtt_server_ip >> 24) & 0xff);
	else
		inet_aton(mqtt_ip, (ip_addr_t *) &hfcfg.mqtt_server_ip);
	
	u_printf("config: ip_s=%s ip_n=%08x page_size=%d\r\n", mqtt_ip, hfcfg.mqtt_server_ip, sizeof(config_page));
	
	httpd_arg_find(url, "mqtt_port", mqtt_port);
	if (ret <= 0)
		sprintf(mqtt_port, "%d", hfcfg.mqtt_server_port);
	else
		hfcfg.mqtt_server_port = atoi(mqtt_port);
	
	sprintf(rsp, config_page, mqtt_ip, mqtt_port);
}




static int USER_FUNC hfsys_event_callback(uint32_t event_id, void *param)
{
	extern char enaiter_wifi_connected;
	switch(event_id) {
		case HFE_WIFI_STA_CONNECTED:
			u_printf("wifi sta connected!\r\n");
			break;
			
		case HFE_WIFI_STA_DISCONNECTED:
			hfrtcfg.has_ip = 0;
			u_printf("wifi sta disconnected!\r\n");
			break;
			
		case HFE_DHCP_OK:
			{
				uint32_t *p_ip;
				p_ip = (uint32_t*) param;
				HF_Debug(DEBUG_WARN, "dhcp ok %08X!\r\n", *p_ip);
				u_printf("dhcp ok %08X!", *p_ip);
				hfrtcfg.has_ip = 1;
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


struct hfeasy_runtime_config* USER_FUNC config_get_rtcfg(void)
{
	return &hfrtcfg;
}


void USER_FUNC config_init(void)
{
	memset(&hfrtcfg, 0, sizeof(struct hfeasy_runtime_config));
	
	if(hfsys_register_system_event((hfsys_event_callback_t) hfsys_event_callback) != HF_SUCCESS)
		HF_Debug(DEBUG_ERROR,"error registering system event callback\r\n");

	/* register config webpage */
	httpd_add_page("/config", httpd_page_config);

}
