
#include "hfeasy.h"


static const char *config_page_wifi =
	"<!DOCTYPE html><html><head><title>HFeasy config v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body>"\
	"<h1>WiFi</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"config_wifi\" method=\"GET\"><table>"\
	"<TR><TH colspan=\"2\">WiFi config"\
	"<TR><TD>Mode<TD><select name=\"wmode\">"\
	"<option %s value=0>AP</option>"\
	"<option %s value=1>Client</option>"\
	"<option %s value=2>AP+Client</option>"\
	"</select>"\
	"<TR><TH colspan=\"3\">Client config"\
	"<TR><TD>SSID<TD><input type=\"text\" name=\"stassid\" value=\"%s\">"\
	"<TR><TD>Encryption<TD><select name=\"staauth\">"\
	"<option %s value=0>Open</option>"\
	"<option %s value=1>WPA</option>"\
	"<option %s value=2>WPA2</option>"\
	"</select>"\
	"<select name=\"staenc\">"\
	"<option %s value=0>NONE</option>"\
	"<option %s value=1>TKIP</option>"\
	"<option %s value=2>AES</option>"\
	"</select>"\
	"<TR><TD>Password<TD><input type=\"text\" name=\"stapass\" value=\"%s\">"\
	"</table><input type=\"submit\" value=\"Apply\" name=\"apply\"></form>"\
	"</body></html>";
	
static void USER_FUNC httpd_page_config_wifi(char *url, char *rsp)
{
	char tmp[150], ans[150];
	char *words[5] = {NULL};
	int ret;
	int wmode = 0, auth = 0, enc = 0;
	char ssid[50], pass[50];
	
	
	ret = httpd_arg_find(url, "apply", tmp);
	if (ret > 0) {
		
		ret = httpd_arg_find(url, "wmode", tmp);
		if (ret > 0) {
			wmode = atoi(tmp);
		}
		ret = httpd_arg_find(url, "staauth", tmp);
		if (ret > 0) {
			auth = atoi(tmp);
		}
		ret = httpd_arg_find(url, "staenc", tmp);
		if (ret > 0) {
			enc = atoi(tmp);
		}
		
		ret = httpd_arg_find(url, "stassid", tmp);
		if (ret > 0)
			strcpy(ssid, tmp);

		ret = httpd_arg_find(url, "stapass", tmp);
		if (ret > 0)
			strcpy(pass, tmp);
		

		sprintf(tmp, "AT+WMODE=%s\r\n", wmode==0?"AP":(wmode==1?"STA":"APSTA"));
		hfat_send_cmd(tmp, sizeof(tmp), ans, 150);

		sprintf(tmp, "AT+WSSSID=%s\r\n", ssid);
		hfat_send_cmd(tmp, sizeof(tmp), ans, 150);

		sprintf(tmp, "AT+WSKEY=%s,%s,%s\r\n", auth==0?"OPEN":(auth==1?"WPAPSK":(auth==2?"WPA2PSK":"")), enc==0?"NONE":(enc==1?"TKIP":(enc==2?"AES":"")), pass);
		hfat_send_cmd(tmp, sizeof(tmp), ans, 150);
		
	} else {
		hfat_send_cmd("AT+WMODE\r\n", sizeof("AT+WMODE\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 2) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				if (strcmp(words[1], "AP") == 0)
					wmode = 0;
				else if (strcmp(words[1], "STA") == 0)
					wmode = 1;
				else if (strcmp(words[1], "APSTA") == 0)
					wmode = 2;
			}
		}

		hfat_send_cmd("AT+WSSSID\r\n", sizeof("AT+WSSSID\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 2) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				strcpy(ssid, words[1]);
			}
		}
		hfat_send_cmd("AT+WSKEY\r\n", sizeof("AT+WSKEY\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 4) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				if (strcmp(words[1], "OPEN") == 0)
					auth = 0;
				else if (strcmp(words[1], "WPAPSK") == 0)
					auth = 1;
				else if (strcmp(words[1], "WPA2PSK") == 0)
					auth = 2;
				
				log_write(words[1]);
				log_write(words[2]);
				if (strcmp(words[2], "NONE") == 0)
					enc = 0;
				else if (strcmp(words[2], "TKIP") == 0)
					enc = 1;
				else if (strcmp(words[2], "AES") == 0)
					enc = 2;

				strcpy(pass, words[3]);
				log_write(pass);
			}
		}

	}
	
	sprintf(rsp, config_page_wifi, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					wmode == 0 ? "selected" : "",
					wmode == 1 ? "selected" : "",
					wmode == 2 ? "selected" : "",
					ssid,
					auth == 0 ? "selected" : "",
					auth == 1 ? "selected" : "",
					auth == 2 ? "selected" : "",
					enc == 0 ? "selected" : "",
					enc == 1 ? "selected" : "",
					enc == 2 ? "selected" : "",
					pass);

	log_printf("page_size=%d\r\n", strlen(rsp));
}

static const char *config_page_network =
	"<!DOCTYPE html><html><head><title>HFeasy config v%d.%d</title><link rel=\"stylesheet\" href=\"styles.css\"></head><body onload=\"check()\">"\
	"<h1>WiFi</h1><hr>"\
	"<a href=\"/\">Go back</a>"\
	"<form action=\"config_network\" method=\"GET\"><table>"\
	"<TR><TH colspan=\"2\">Network settings"\
	"<TR><TD>DHCP (automatic IP)<TD><input id=\"idhcp\" %s type=\"checkbox\" name=\"dhcp\" value=\"1\" onclick=\"check()\">"\
	"<TR><TD>IP address<TD><input id=\"iip\" type=\"text\" name=\"ip\" value=\"%s\">"\
	"<TR><TD>Subnet mask<TD><input id=\"imask\" type=\"text\" name=\"mask\" value=\"%s\">"\
	"<TR><TD>Gateway<TD><input id=\"igw\" type=\"text\" name=\"gw\" value=\"%s\">"\
	"<TR><TD>DNS<TD><input id=\"idns\" type=\"text\" name=\"dns\" value=\"%s\">"\
	"</table><input type=\"submit\" value=\"Apply\" name=\"apply\"></form>"\
	"<script type=\"text/javascript\">"\
	"function check() {"\
  "if(document.getElementById(\"idhcp\").checked == true){"\
	"document.getElementById(\"iip\").disabled = true;"\
	"document.getElementById(\"imask\").disabled = true;"\
	"document.getElementById(\"igw\").disabled = true;"\
	"document.getElementById(\"idns\").disabled = true;"\
	"}else{"\
	"document.getElementById(\"iip\").disabled = false;"\
	"document.getElementById(\"imask\").disabled = false;"\
	"document.getElementById(\"igw\").disabled = false;"\
	"document.getElementById(\"idns\").disabled = false;"\
	"}}"\
	"</script>"\
	"</body></html>";

static void USER_FUNC httpd_page_config_network(char *url, char *rsp)
{
	char tmp[150], ans[150];
	char *words[6] = {NULL};
	char dhcp, ip[20], mask[20], gw[20], dns[20];
	int ret;
	
	strcpy(ip, "");
	strcpy(mask, "");
	strcpy(gw, "");
	strcpy(dns, "");

	ret = httpd_arg_find(url, "apply", tmp);
	if (ret > 0) {
		dhcp = 0;
		ret = httpd_arg_find(url, "dhcp", tmp);
		if (ret > 0) {
			dhcp = 1;
			hfat_send_cmd("AT+WANN=DHCP\r\n", sizeof("AT+WANN=DHCP\r\n"), ans, 64);
			
		} else {
			ret = httpd_arg_find(url, "ip", tmp);
			if (ret > 0)
				strcpy(ip, tmp);
			
			ret = httpd_arg_find(url, "mask", tmp);
			if (ret > 0)
				strcpy(mask, tmp);

			ret = httpd_arg_find(url, "gw", tmp);
			if (ret > 0)
				strcpy(gw, tmp);

			sprintf(tmp, "AT+WANN=static,%s,%s,%s\r\n", ip, mask, gw);
			hfat_send_cmd(tmp, sizeof(tmp), ans, 150);

			/* set dns */
			ret = httpd_arg_find(url, "dns", tmp);
			if (ret > 0)
				strcpy(dns, tmp);
			sprintf(tmp, "AT+WSDNS=%s\r\n", dns);
			hfat_send_cmd(tmp, sizeof(tmp), ans, 150);
		}
	} else {
		/* get params */
		hfat_send_cmd("AT+WANN\r\n", sizeof("AT+WANN\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 5) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				if (strcmp(words[1], "DHCP") == 0) {
					dhcp = 1;
				} else {
					dhcp = 0;
				}
				strcpy(ip, words[2]);
				strcpy(mask, words[3]);
				strcpy(gw, words[4]);
			}
		}
		hfat_send_cmd("AT+WSDNS\r\n", sizeof("AT+WSDNS\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 2) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				strcpy(dns, words[1]);
			}
		}
	} 
	
	sprintf(rsp, config_page_network, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
					dhcp ? "checked": "", ip, mask, gw, dns);

	log_printf("page_size=%d\r\n", strlen(rsp));
}


void USER_FUNC network_init(void)
{
	//while (!hfnet_wifi_is_active())
	//	msleep(100);

	httpd_add_page("/config_network", httpd_page_config_network, NULL);
	httpd_add_page("/config_wifi", httpd_page_config_wifi, NULL);
}
