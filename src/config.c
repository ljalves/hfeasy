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

#include <stdarg.h>
#include "hfeasy.h"

#define CONFIG_MAGIC_VER1  0xcf
#define CONFIG_OFFSET      0x00
#define CONFIG_SIZE        (sizeof(struct hfeasy_config))

struct hfeasy_state state;
static hftimer_handle_t reset_timer;

#if defined(__LPXX00__)

/* wifi_led, relay, but_push, but_togg, but_up, but_dn, buzz, i2c_ck, i2c_dt, inverted_outputs */
const int gpio_default_config[DEVICE_END - 1][11]	= 
{
	/* wall socket module */
	{	0, 10, 0, 8, 0, 0, 9, 0, 0, HFM_TYPE_LPT100F, 0},
	
	/* plug */
	{	43, 44, 30, 0, 0, 0, 0, 0, 0, HFM_TYPE_LPB100, GPIO_INV_LED},
	
	/* us dimmer */
	{	6, 0, 10, 0, 8, 9, 0, 4, 7, HFM_TYPE_LPT100F, GPIO_INV_LED},
	
	/* us wall switch */
	{	10, 8, 9, 0, 0, 0, 0, 0, 0, HFM_TYPE_LPT100F, GPIO_INV_LED},
	
	/* obi g-homa */
	{ 11, 12, 18, 0, 0, 0, 0, 0, 0, HFM_TYPE_LPB100, 0 },
	
	/* Orvibo S20 WiWo-S20-E2 */
	{ 12, 20, 45, 0, 0, 0, 0, 0, 0, HFM_TYPE_LPB100, 0},
};

#elif defined(__LPXX30__)

/* wifi_led, relay, but_push, but_togg, but_up, but_dn, buzz, i2c_ck, i2c_dt, inverted_outputs */
const int gpio_default_config[DEVICE_END - 1][11]	= 
{
	/* plug */
	{	29, 30, 28, 0, 0, 0, 0, 0, 0, HFM_TYPE_LPB130, GPIO_INV_LED},
	
};

#else
#error "platform not supported"
#endif


int at_cmd(char *cmd, char **words, int nrwords, int maxlen)
{
	char tmp[150];
	int ret;
	snprintf(tmp, 149, "%s\r\n", cmd);
	
	hfat_send_cmd(tmp, strlen(tmp)+1, cmd, maxlen);
	log_printf("atcmd: %s, %s", tmp, cmd);
	ret = hfat_get_words(cmd, words, nrwords);
	if (ret > 0) {
		if ((cmd[0]=='+') && (cmd[1]=='o') && (cmd[2]=='k')) {
		} else {
			ret = 0;
		}
	}
	return ret;
}

static void USER_FUNC reboot_timer_handler(hftimer_handle_t timer)
{
	hfsys_reset();
}

void USER_FUNC reboot(void)
{
	hftimer_start(reset_timer);
}

static const char *config_page_save =
	"<!DOCTYPE html><html><head>"\
	"<meta http-equiv=\"refresh\" content=\"5;url=/\" />"\
	"<title>HFeasy config v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	". Please wait...</body></html>";

static void USER_FUNC httpd_page_save(char *url, char *rsp)
{
	char tmp[50];
	int ret;
	int save = 0;
	
	ret = httpd_arg_find(url, "save", tmp);
	if (ret > 0) {
		config_save();
		save = 1;
		reboot();
	}
	ret = httpd_arg_find(url, "restart", tmp);
	if (ret > 0) {
		reboot();
	}
	sprintf(rsp, config_page_save, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR, save ? "Saving config to flash" : "Restarting");
}


static const char *at_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body onload=\"check()\">"\
	"<h1>HFeasy</h1><hr>"\
	"<form action=\"at\" method=\"GET\">"\
	"<table><tr><th colspan=\"2\">Time settings"\
	"<tr><td>AT CMD<td><input type=\"text\" name=\"cmd\" value=\"%s\">"\
	"<tr><td>Answer<td>%s"\
	"</table></form></body></html>";

static void USER_FUNC httpd_page_at(char *url, char *rsp)
{
	char tmp[100], ans[200];
	int ret;

	ret = httpd_arg_find(url, "cmd", tmp);
	if (ret == 1) {
		strcat(tmp, "\r\n");
		hfat_send_cmd(tmp, strlen(tmp)+1, ans, 199);	
	}	else {
		strcpy(tmp, "");
		strcpy(ans, "");
	}
	sprintf(rsp, at_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR, tmp, ans);
}


#if 0
static const char *firmware_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body>"\
	"<h1>Firmare</h1><hr>"\
	"<a href=\"config\">Go back</a>"\
	"<form action=\"firmware\" method=\"GET\"><table>"\
	"<th colspan=\"2\">Firmware"\
	"<tr><td>Update firmware from: <td><input type=\"text\" name=\"fwurl\" value=\"%s\"><td><input type=\"submit\" value=\"Update\">"\
	"</table></form>"\
	"<form action=\"firmware\" method=\"GET\"><input type=\"submit\" value=\"Restart\" name=\"restart\"></form>"\
	"</body></html>";

static void USER_FUNC httpd_page_firmware(char *url, char *rsp)
{
	char tmp[100];
	int ret;
	
	ret = httpd_arg_find(url, "fwurl", tmp);
	if (ret > 0) {
		httpc_get_fwfile(tmp);
	}

	sprintf(rsp, firmware_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
		"https://github.com/ljalves/hfeasy/releases/download/0v9/HFEASY_UPGRADE.bin" 
	);
}
#endif

static const char *main_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body>"\
	"<h1>HFeasy v%d.%d</h1><hr>"\
	"<button id=\"btn_on\">ON</button>"\
	"<button id=\"btn_off\">OFF</button>"\
	"<hr>"\
	"<table><tr>"\
	"<tr><td><a href=\"config\">Module</a>"\
	"<tr><td><a href=\"config_mqtt\">MQTT</a>"\
	"<tr><td><a href=\"config_device\">Device</a>"\
	"<tr><td><a href=\"config_gpio\">GPIO</a>"\
	"<tr><td><a href=\"config_wifi\">WiFi</a>"\
	"<tr><td><a href=\"config_network\">Network</a>"\
	"<tr><td><a href=\"ntp\">NTP</a>"\
	"<tr><td><a href=\"timer\">Timers</a>"\
	"<tr><td><a href=\"status\">Status</a>"\
	"<tr><td><a href=\"log\">Logs</a>"\
	"<tr><td><a href=\"iweb.html\">Upgrade</a>"\
	"<tr><td><a href=\"hf\">HF</a>"\
	"</table>"\
	"<hr><form action=\"save\" method=\"GET\"><input type=\"submit\" value=\"Save changes to flash and restart\" name=\"save\"></form>"\
	"<hr><form action=\"restart\" method=\"GET\"><input type=\"submit\" value=\"Restart\" name=\"restart\"></form>"\
	"<script type=\"text/javascript\">"\
	"const bon = document.getElementById('btn_on');"\
	"const boff = document.getElementById('btn_off');"\
	"bon.addEventListener('click', async _ => { try { const response = await fetch('/state?sw=255', {method: 'get'}); } catch(err) { console.error(`Error: ${err}`); }});"\
	"boff.addEventListener('click', async _ => { try { const response = await fetch('/state?sw=0', {method: 'get'}); } catch(err) { console.error(`Error: ${err}`); }});"\
	"</script>"\
	"</body></html>";

static void USER_FUNC httpd_page_main(char *url, char *rsp)
{
	sprintf(rsp, main_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
			HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR);

	log_printf("page_size=%d\r\n", strlen(rsp));

}

static const char *config_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body>"\
	"<h1>HFeasy config page</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"/config\" method=\"GET\"><table>"\
	"<th colspan=\"2\">Module"\
	"<tr><td>Friendly name<td><input type=\"text\" name=\"fn\" value=\"%s\">"\
	"<tr><td>Module name (id)<td><input type=\"text\" name=\"mn\" value=\"%s\">"\
	"<tr><td>HTTP auth<td><input type=\"checkbox\" name=\"auth\" value=\"1\" %s>"\
	"<tr><td>Enable CORS:*<td><input type=\"checkbox\" name=\"cors\" value=\"1\" %s>"\
	"<tr><td>Wifi LED (if supported)<td><select name=\"led\">"\
	"<option value=\"0\"%s>Off</option>"\
	"<option value=\"1\"%s>MQTT</option>"\
	"<option value=\"2\"%s>HTTP</option>"\
	"<option value=\"3\"%s>MQTT+HTTP</option>"\
	"<option value=\"4\"%s>Relay</option>"\
	"<option value=\"5\"%s>MQTT topic</option>"\
	"<option value=\"6\"%s>Find</option>"\
	"<option value=\"7\"%s>Wifi</option>"\
	"</select>"\
	"<tr><td>Power on state<td><input type=\"text\" name=\"pwron\" value=\"%d\"> (0=off, >0=on; dimmer: level=0~128)"\
	"</table><input type=\"submit\" value=\"Apply\"></form>"\
	"</body></html>";


static void USER_FUNC httpd_page_config(char *url, char *rsp)
{
	char tmp[50];
	int ret;
	
	ret = httpd_arg_find(url, "fn", tmp);
	if (ret > 0) {
		if (tmp[0] != '\0')
			strcpy(state.cfg.friendly_name, tmp);
		
		state.cfg.httpd_settings = 0;
		ret = httpd_arg_find(url, "auth", tmp);
		if ((ret > 0) && (tmp[0] == '1'))
			state.cfg.httpd_settings |= HTTPD_AUTH;	
		ret = httpd_arg_find(url, "cors", tmp);
		if ((ret > 0) && (tmp[0] == '1'))
			state.cfg.httpd_settings |= HTTPD_CORS;	
	}

	ret = httpd_arg_find(url, "mn", tmp);
	if (ret > 0) {
		if (tmp[0] != '\0')
			strcpy(state.module_name, tmp);
	}

	ret = httpd_arg_find(url, "led", tmp);
	if (ret > 0) {
		state.cfg.wifi_led = atoi(tmp);
		if (state.cfg.wifi_led >= LED_CONFIG_END)
			state.cfg.wifi_led = 0;
	}
	
	ret = httpd_arg_find(url, "pwron", tmp);
	if (ret > 0) {
		state.cfg.pwron_state = atoi(tmp);
	}
	
	sprintf(rsp, config_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
		state.cfg.friendly_name, state.module_name,
		state.cfg.httpd_settings & HTTPD_AUTH ? "checked" : "",
		state.cfg.httpd_settings & HTTPD_CORS ? "checked" : "",
		state.cfg.wifi_led == 0 ? "selected" : "",
		state.cfg.wifi_led == 1 ? "selected" : "",
		state.cfg.wifi_led == 2 ? "selected" : "",
		state.cfg.wifi_led == 3 ? "selected" : "",
		state.cfg.wifi_led == 4 ? "selected" : "",
		state.cfg.wifi_led == 5 ? "selected" : "",
		state.cfg.wifi_led == 6 ? "selected" : "",
		state.cfg.pwron_state
	);

	if (state.cfg.wifi_led == LED_CONFIG_FIND) {
		led_ctrl("n5f5n2f2n2f2n5f5n1f1r"); /* "find" blink pattern */
	} else {
		led_ctrl("f");
	}	
	
	log_printf("page_size=%d\r\n", strlen(rsp));
}


static const char *config_page_mqtt =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	"<h1>HFeasy MQTT config page</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"/config_mqtt\" method=\"GET\"><table>"\
	"<TH colspan=\"2\">MQTT client"\
	"<TR><TD>Server IP<TD><input type=\"text\" name=\"host\" value=\"%s\">"\
	"<TR><TD>Server port (0=disabled)<TD><input type=\"text\" name=\"port\" value=\"%d\">"\
	"<TR><TD>Username<TD><input type=\"text\" name=\"user\" value=\"%s\">"\
	"<TR><TD>Password<TD><input type=\"password\" name=\"pass\" value=\"%s\">"\
	"<TR><TD>Topic (%%topic%%)<TD><input type=\"text\" name=\"topic\" value=\"%s\">"\
	"<TR><TD>Full topic<TD><input type=\"text\" name=\"full_topic\" value=\"%s\">"\
	"<TR><TD>QOS<TD><input type=\"text\" name=\"qos\" value=\"%d\">"\
	"<TR><TD>ON value<TD><input type=\"text\" name=\"on_val\" value=\"%s\">"\
	"<TR><TD>OFF value<TD><input type=\"text\" name=\"off_val\" value=\"%s\">"\
	"</table><input type=\"submit\" value=\"Apply\"></form>"\
	"</body></html>";

static void USER_FUNC httpd_page_config_mqtt(char *url, char *rsp)
{
	char tmp[50];
	int ret;
	
	ret = httpd_arg_find(url, "host", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_server_hostname, tmp);
	
	ret = httpd_arg_find(url, "port", tmp);
	if (ret > 0) {
		state.cfg.mqtt_server_port = atoi(tmp);
		if (state.cfg.mqtt_server_port == 0)
			state.mqtt_ready = 0;
	}
	
	ret = httpd_arg_find(url, "user", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_server_user, tmp);

	ret = httpd_arg_find(url, "pass", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_server_pass, tmp);

	ret = httpd_arg_find(url, "topic", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_topic, tmp);
		
	ret = httpd_arg_find(url, "full_topic", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_full_topic, tmp);

	ret = httpd_arg_find(url, "qos", tmp);
	if (ret > 0)
		state.cfg.mqtt_qos = atoi(tmp);
	
	ret = httpd_arg_find(url, "on_val", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_on_value, tmp);

	ret = httpd_arg_find(url, "off_val", tmp);
	if (ret > 0)
		strcpy(state.cfg.mqtt_off_value, tmp);
	
	sprintf(rsp, config_page_mqtt, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					state.cfg.mqtt_server_hostname, state.cfg.mqtt_server_port,
					state.cfg.mqtt_server_user, state.cfg.mqtt_server_pass,
					state.cfg.mqtt_topic, state.cfg.mqtt_full_topic, state.cfg.mqtt_qos,
					state.cfg.mqtt_on_value, state.cfg.mqtt_off_value);

	u_printf("page_size=%d\r\n", strlen(rsp));
}

static const char *config_page_device =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	"<h1>HFeasy device selection</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"/config_device\" method=\"GET\"><table>"\
	"<TR><TD>Device<TD><select name=\"device\">"\
	"<option %s value=0>custom</option>"
#if defined(__LPXX00__)
	"<option %s value=1>module</option>"\
	"<option %s value=2>plug</option>"\
	"<option %s value=3>us_dimmer</option>"\
	"<option %s value=4>us_wall_sw</option>"\
	"<option %s value=5>g-homa</option>"
#elif defined(__LPXX30__)
	"<option %s value=1>plug</option>"
#endif
	"</select>"\
	"<TD><input type=\"submit\" value=\"Change device\">"\
	"</table></form>"
	"<br>Note: After changing device and/or after setting custom gpio's, go back, save to flash and reboot";

static void USER_FUNC httpd_page_config_device(char *url, char *rsp)
{
	char tmp[50];
	int ret;
	
	ret = httpd_arg_find(url, "device", tmp);
	if (ret > 0) {
		state.cfg.device = atoi(tmp);
		if (state.cfg.device >= DEVICE_END)
			state.cfg.device = DEVICE_CUSTOM;
	
#ifdef __LPXX00__1
		relay_deinit();
		led_deinit();
		dimmer_deinit();
		buzzer_deinit();
		gpio_deinit();
#endif

		/* apply device default gpio config */
		if (state.cfg.device > DEVICE_CUSTOM) {
			memcpy(state.cfg.gpio_config, gpio_default_config[state.cfg.device - 1], sizeof(state.cfg.gpio_config));
		}

#ifdef __LPXX00__1
		hfeasy_gpio_init();
		relay_init();
		led_init();
		dimmer_init();
		buzzer_init();
#endif

	}

	sprintf(rsp, config_page_device, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR
		,state.cfg.device == 0 ? "selected" : ""
		,state.cfg.device == 1 ? "selected" : ""
#ifdef __LPXX00__
		,state.cfg.device == 2 ? "selected" : ""
		,state.cfg.device == 3 ? "selected" : ""
		,state.cfg.device == 4 ? "selected" : ""
		,state.cfg.device == 5 ? "selected" : ""
#endif
	);
}

static const char *config_page_gpio =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	"<h1>HFeasy GPIO config</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"/config_gpio\" method=\"GET\"><table>"\
	"<TR><TD>Module type<TD><select %s name=\"hfmod\">"
#if defined(__LPXX00__)
	"<option %s value=0>LPB100</option>"\
	"<option %s value=4>LPT100F</option>"
#elif defined(__LPXX30__)
	"<option %s value=0>LPB130</option>"
#endif
	"</select>"\
	"</table>"\
	"<br>"\
	"<table><tr><th>Function<th>Pin nr (0=disabled)"\
	"<TR><TD>WiFi LED<TD><input %s type=\"text\" name=\"f0\" value=\"%d\"> Active low:<input %s type=\"checkbox\" name=\"inv_led\" value=\"1\" %s>"\
	"<TR><TD>Relay<TD><input %s type=\"text\" name=\"f1\" value=\"%d\"> Active low:<input %s type=\"checkbox\" name=\"inv_relay\" value=\"1\" %s>"\
	"<TR><TD>Button (push)<TD><input %s type=\"text\" name=\"f2\" value=\"%d\">"\
	"<TR><TD>Button (toggle)<TD><input %s type=\"text\" name=\"f3\" value=\"%d\">"\
	"<TR><TD>Button Up<TD><input %s type=\"text\" name=\"f4\" value=\"%d\">"\
	"<TR><TD>Button Down<TD><input %s type=\"text\" name=\"f5\" value=\"%d\">"\
	"<TR><TD>Buzzer<TD><input %s type=\"text\" name=\"f6\" value=\"%d\">"\
	"<TR><TD>Dimmer I2C CLK<TD><input %s type=\"text\" name=\"f7\" value=\"%d\">"\
	"<TR><TD>Dimmer I2C DTA<TD><input %s type=\"text\" name=\"f8\" value=\"%d\">"\
	"</table><input %s type=\"submit\" value=\"Apply\"></form>"\
	"</body></html>";


static void USER_FUNC httpd_page_config_gpio(char *url, char *rsp)
	{
	char tmp[50], f[10];
	int ret, i;
	
	if (state.cfg.device == DEVICE_CUSTOM){
		ret = httpd_arg_find(url, "hfmod", tmp);
		if (ret > 0) {

#ifdef __LPXX00__1
			relay_deinit();
			led_deinit();
			dimmer_deinit();
			buzzer_deinit();
			gpio_deinit();
#endif
			
			state.cfg.gpio_config[9] = atoi(tmp);
		
			for (i = 0; i < 9; i++) {
				sprintf(f, "f%d", i);
				ret = httpd_arg_find(url, f, tmp);
				if (ret > 0)
					state.cfg.gpio_config[i] = atoi(tmp);
			}

			state.cfg.gpio_config[10] &= ~GPIO_INV_LED;
			ret = httpd_arg_find(url, "inv_led", tmp);
			if (ret > 0) {
				if (tmp[0] == '1')
					state.cfg.gpio_config[10] |= GPIO_INV_LED;
			}
			
			state.cfg.gpio_config[10] &= ~GPIO_INV_RELAY;
			ret = httpd_arg_find(url, "inv_relay", tmp);
			if (ret > 0) {
				if (tmp[0] == '1')
					state.cfg.gpio_config[10] |= GPIO_INV_RELAY;
			}

#ifdef __LPXX00__1
			hfeasy_gpio_init();
			relay_init();
			led_init();
			dimmer_init();
			buzzer_deinit();
#endif

		}
	}

	sprintf(rsp, config_page_gpio, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
		
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled",
#if defined (__LPXX00__)
		state.cfg.gpio_config[9] == HFM_TYPE_LPB100 ? "selected" : "",
		state.cfg.gpio_config[9] == HFM_TYPE_LPT100F ? "selected" : "",
#elif defined(__LPXX30__)
		state.cfg.gpio_config[9] == HFM_TYPE_LPB130 ? "selected" : "",
#endif
		/* gpios*/
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[0],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[10] & GPIO_INV_LED ? "checked": "",
		
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[1],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[10] & GPIO_INV_RELAY ? "checked": "",
		
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[2],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[3],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[4],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[5],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[6],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[7],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled", state.cfg.gpio_config[8],
		state.cfg.device == DEVICE_CUSTOM ? "" : "disabled"
	);
	log_printf("page_size=%d\r\n", strlen(rsp));
}


static void get_reset_reason(uint32_t r, char *s)
{
	s[0] = '\0';
	
	if (r == HFSYS_RESET_REASON_NORMAL) { // power off restart
		strcat(s, "normal startup");
		return;
	}
	if (r & HFSYS_RESET_REASON_ERESET) // hardware restart
		strcat(s, "reset pin,");
	if (r & HFSYS_RESET_REASON_IRESET0) // soft restart softreset
		strcat(s, "softreset,");
	if (r & HFSYS_RESET_REASON_IRESET1) // call hfsys_reset
		strcat(s, "hfsys_reset,");
	if (r & HFSYS_RESET_REASON_WPS) // WPS restart
		strcat(s, "wps restart,");
	if (r & HFSYS_RESET_REASON_WPS_OK) // wps success
		strcat(s, "wps success,");
	if (r & HFSYS_RESET_REASON_SMARTLINK_START) // turn on smartLink
		strcat(s, "smartlink restart,");
	if (r & HFSYS_RESET_REASON_SMARTLINK_OK) // smartlink success
		strcat(s, "smartlink ok,");
	if (strlen(s) == 0)
		strcat(s, "unknown,");
	s[strlen(s) - 1] = '\0';
	return;
}

static const char *status_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	"<h1>HF Easy v%d.%d module status</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
  "<h2>System</h2><br>"\
	"Reset flags: %08x, reason: %s<br>Free memory: %u bytes<br>"\
	"Uptime: %s"\
	"<hr>"\
	"<h2>GPIO status</h2><br>"\
	"Switch: %s<br>Relay: %s, last changed by: %s, publish: %d, action: %d"\
	"<hr>"\
	"<h2>Timer status</h2><br>"\
	"Countdown (turn OFF): %s<br>"\
	"Countdown (turn ON): %s<br>"\
	"<hr>"\
	"<h2>Connectivity</h2>"\
	"Hostname: %s<br>"\
	"MQTT server: %s"\
  "</body></html>";


const char *relay_change_src[] = { "HTTP", "MQTT", "TIMER", "SWITCH", "SWUP", "SWDOWN", "POWER ON" };

static void USER_FUNC httpd_page_status(char *url, char *rsp)
{
	char cd_off[25], cd_on[25], uptime[25];
	char rr[100];
	uint32_t i, h, m, s, now = hfsys_get_time() / 1000;
	
	get_reset_reason(state.reset_reason, rr);
	
	if (state.cfg.countdown[0] != 0) {
		if (state.countdown[0] == 0) {
			sprintf(cd_off, "waiting for ON state");
		} else {
			i = state.countdown[0] - now;
			s = i % 60;
			i /= 60;
			m = i % 60;
			i /= 60;
			h = i;
			sprintf(cd_off, "%dh%dm%ds", (int)h, (int)m, (int)s);
		}
	} else {
		sprintf(cd_off, "disabled");
	}
	
	if (state.cfg.countdown[1] != 0) {
		if (state.countdown[1] == 0) {
			sprintf(cd_on, "waiting for OFF state");
		} else {
			i = state.countdown[1] - now;
			s = i % 60;
			i /= 60;
			m = i % 60;
			i /= 60;
			h = i;
			sprintf(cd_on, "%dh%dm%ds", (int)h, (int)m, (int)s);
		}
	} else {
		sprintf(cd_on, "disabled");
	}
	
	i = now;
	s = i % 60;
	i /= 60;
	m = i % 60;
	i /= 60;
	h = i % 24;
	i /= 24;
	sprintf(uptime, "%d days %dh%dm%ds", (int)i, (int)h, (int)m, (int)s);
	
	sprintf(rsp, status_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					state.reset_reason, rr, hfsys_get_memory(), uptime,
					gpio_get_state(GPIO_SWITCH_PUSH) ? "High" : "Low",
					state.relay_state ? "Closed(On)" : "Open(Off)",
					relay_change_src[state.relay_modifier & 3],
					(state.relay_modifier >> 6) & 1, (state.relay_modifier >> 4) & 3,
					cd_off, cd_on,
					state.module_name,
					state.mqtt_ready ? "Connected" : "Disconnected");
}

/*
void log_write(char *txt)
{
	char buf[100];
	sprintf(buf, "%s\n", txt);
	hfuflash_write(state.cfg.log_ptr, buf, strlen(buf));
	state.cfg.log_ptr += strlen(buf);
}

void log_read(uint32_t line, char *txt)
{
	char buf[100];
	uint32_t j, i = 0;
	
	do {
		hfuflash_read(0, buf, 100);
		for (j = 0; j < 100; j++) {
			if (buf[j] == '\n')
				i++;
		}
	} while (i != line);
}*/


static const char *log_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title></head><body>"\
	"<h1>HF Easy v%d.%d log</h1><hr>"\
	"<a href=\"/\">Go back</a><hr>"\
	"%s"\
  "</body></html>";


#define LOG_BUF_SIZE 1024
static char *log_buf = NULL;

void USER_FUNC log_printf(const char *fmt, ...)
{
	static char *p;
	int i;
	char *f;
	static unsigned int line = 0;
	char *buf;
	char lnr[10];
	const char *nl = "<br>";
	int ret;
	va_list ap;

	if (log_buf == NULL) {
		log_buf = hfmem_malloc(LOG_BUF_SIZE);
		if (log_buf == NULL)
			return;
		p = log_buf;
	}
	
	buf = hfmem_malloc(500);
	if (buf == NULL)
		return;

	
	va_start(ap, fmt);
	ret = vsnprintf(buf, 500-1, fmt, ap);
	va_end(ap);
	snprintf(lnr, 10-1, "%d: ", line++);
	if (ret > 0) {
		
		while ((p - log_buf) > (LOG_BUF_SIZE - (strlen(buf) + strlen(nl) + strlen(lnr) + 1))) {
			f = strstr(log_buf, nl);
			f += 4;
			p = log_buf;
			for (i = strlen(f)+1; i > 0; i--)
				*(p++) = *(f++);
			p--;
		}

		strcpy(p, lnr);
		p+=strlen(lnr);
		strcpy(p, buf);
		p+=strlen(buf);
		strcpy(p, nl);
		p+=strlen(nl);
	}
	hfmem_free(buf);
}


static void USER_FUNC httpd_page_log(char *url, char *rsp)
{
	sprintf(rsp, log_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					log_buf);
}


void USER_FUNC get_sta_settings(char *buf)
{
	char *words[3] = {NULL};
	char rsp[64] = {0};

	hfat_send_cmd("AT+WANN\r\n", sizeof("AT+WANN\r\n"), rsp, 64);
	if (hfat_get_words(rsp, words, 2) > 0) {
		if ((rsp[0]=='+') && (rsp[1]=='o') && (rsp[2]=='k')) {
			strcpy(buf, words[1]);
			return;
		}
	}
	strcpy(buf, "");
}


static int sys_event_callback(uint32_t event_id, void *param)
{
	char tmp[50];
	log_printf("cbk id=%d", event_id);

	switch(event_id) {
		case HFE_WIFI_STA_CONNECTED:
			log_printf("wifi sta connected");
			led_ctrl("n51f51r");
		
			/* check if static ip */
			get_sta_settings(tmp);
			if (strcmp(tmp, "static") == 0) {
				state.has_ip = 1;
				if (state.cfg.wifi_led == LED_CONFIG_WIFI)
					led_ctrl("n");

			}		
			break;
			
		case HFE_WIFI_STA_DISCONNECTED:
			state.has_ip = 0;
			log_printf("wifi sta disconnected!");
			led_ctrl("n2f2r");
			break;
			
		case HFE_DHCP_OK:
			{
				uint32_t *p_ip;
				p_ip = (uint32_t*) param;
				log_printf("network: dhcp ok got ip %s", inet_ntoa(*p_ip));
				state.has_ip = 1;
				if (state.cfg.wifi_led == LED_CONFIG_WIFI)
					led_ctrl("n");
				else
					led_ctrl("f");
			}
			break;
		
		case HFE_SMTLK_OK:
			//u_printf("smartlink ok!\r\n");
			break;
			
		case HFE_CONFIG_RELOAD:
			log_printf("sys: config reload");
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
			/*u_printf("%02x:%02x:%02x:%02x:%02x:%02x",
						(int) state.mac_addr[0], (int) state.mac_addr[1],
						(int) state.mac_addr[2], (int) state.mac_addr[3],
						(int) state.mac_addr[4], (int) state.mac_addr[5]
			);*/
			ret = 1;
		}
	}
	return ret;
}



void USER_FUNC get_module_name(char *buf)
{
	char *words[3] = {NULL};
	char rsp[64] = {0};

	hfat_send_cmd("AT+MID\r\n", sizeof("AT+MID\r\n"), rsp, 64);
	if (hfat_get_words(rsp, words, 2) > 0) {
		if ((rsp[0]=='+') && (rsp[1]=='o') && (rsp[2]=='k')) {
			u_printf("module name = %s\n", words[1]);
			strcpy(buf, words[1]);
			return;
		}
	}
	strcpy(buf, "LPx130");
}

int USER_FUNC set_module_name(void)
{
	char *words[3] = {NULL};
	char rsp[64] = {0};
	char tmp[50];

	/* module name not setup */
	if (strlen(state.module_name) == 0)
		return 3;
	
	get_module_name(tmp);
	/* module name already set */
	if (strcmp(tmp, state.module_name) == 0)
		return 2;
	
	sprintf(tmp, "AT+WRMID=%s\r\n", state.module_name);
	hfat_send_cmd(tmp, strlen(tmp) + 1, rsp, 64);
	if ((hfat_get_words(rsp, words, 1) > 0) && 
			((rsp[0]=='+') && (rsp[1]=='o') && (rsp[2]=='k')))
			return 0;
	return 1;
}

void USER_FUNC config_save(void)
{
	set_module_name();
	hffile_userbin_write(CONFIG_OFFSET, (char*) &state.cfg, CONFIG_SIZE);
	u_printf("saving config to flash. size=%d\r\n", CONFIG_SIZE);
}

static void USER_FUNC config_load(uint8_t reset)
{
	memset(&state, 0, sizeof(struct hfeasy_state));
	get_macaddr();
	get_module_name(state.module_name);

	if (!reset)
		hffile_userbin_read(CONFIG_OFFSET, (char*) &state.cfg, CONFIG_SIZE);
	
	u_printf("loading config from flash. size=%d magic=%02x reset=%d\r\n", CONFIG_SIZE, state.cfg.ver, reset);
	if (state.cfg.ver != CONFIG_MAGIC_VER1) {
		/* set APSTA mode */
		char rsp[64] = {0};
		hfat_send_cmd("AT+WMODE=APSTA\r\n", sizeof("AT+WMODE=APSTA\r\n"), rsp, 32);

		/* init config data */
		memset(&state.cfg, 0, sizeof(struct hfeasy_config));
		state.cfg.ver = CONFIG_MAGIC_VER1;
		state.cfg.device = DEVICE_CUSTOM;
		mqttcli_initcfg();
		
		strcpy(state.cfg.friendly_name, "HFEASY");
		if (state.module_name[0] == '\0')
			strcpy(state.module_name, "HFEASY");
		
		state.cfg.recovery_time = 3000;
		state.cfg.debounce_time = 50;
		
		hffile_userbin_zero();
		config_save();
		reboot();
	}
}

void USER_FUNC config_init(void)
{
	config_load(0);
	
	state.reset_reason = hfsys_get_reset_reason();
	
	if (hfsys_register_system_event(sys_event_callback) != HF_SUCCESS)
		log_printf("init: error registering system event callback");

	reset_timer = hftimer_create("reboot", 2000, false, HFTIMER_ID_RESET, reboot_timer_handler, 0);
	
	/* register webpages */
	httpd_add_page("/", httpd_page_main, NULL);
	httpd_add_page("/config", httpd_page_config, NULL);
	httpd_add_page("/config_mqtt", httpd_page_config_mqtt, NULL);
	httpd_add_page("/config_device", httpd_page_config_device, NULL);
	httpd_add_page("/config_gpio", httpd_page_config_gpio, NULL);
	httpd_add_page("/status", httpd_page_status, NULL);
	httpd_add_page("/log", httpd_page_log, NULL);
	httpd_add_page("/save", httpd_page_save, NULL);
	httpd_add_page("/at", httpd_page_at, NULL);
}
