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




static void USER_FUNC sec2time(time_t seconds, int *time)
{
		time[0] = seconds % 60;
		seconds /= 60;
		time[1] = seconds % 60;
		seconds /= 60;
		time[2] = seconds;
}

static void USER_FUNC time2sec(time_t *seconds, int *time)
{
	*seconds = time[0];
	*seconds += time[1] * 60;
	*seconds += time[2] * 60 * 60;
}


#define SET 0
#define GET 1

int at_cmd(char *cmd, char **words, int nrwords, int maxlen)
{
	char tmp[150];
	int ret;
	snprintf(tmp, 149, "%s\r\n", cmd);
	
	hfat_send_cmd(tmp, strlen(tmp)+1, cmd, maxlen);
	ret = hfat_get_words(cmd, words, nrwords);
	if (ret > 0) {
		if ((cmd[0]=='+') && (cmd[1]=='o') && (cmd[2]=='k')) {
			
		} else {
			ret = 0;
		}
	}
	return ret;
}

int at_cmd_set(char *cmd, char *val)
{
	char tmp[100], *words[2] = {NULL};
	
	snprintf(tmp, 99, "%s=%s", cmd, val);
	return at_cmd(tmp, words, 1, 99);
}

int ntp_serverip(char *ip, int get)
{
	char ans[150], cmd[50], *words[2] = {NULL};
	const char *ntpser = "AT+NTPSER";
	int ret = 0;
	
	if (get)
		strcpy(cmd, ntpser);
	else
		snprintf(cmd, 49, "%s=%s", ntpser, ip);
	
	if (at_cmd(cmd, words, 2, 49) > 0) {
		if (get)
			strcpy(ip, words[1]);
		log_printf("ntp: %s ip %s", get ? "get" : "set", ip);
	} else {
		log_printf("ntp: ntpser %s error %s", get ? "get" : "set", ip);
		ret = -1;
	}
#if 0
	hfat_send_cmd(cmd, strlen(cmd)+1, ans, 50);
	if (hfat_get_words(ans, words, 2) > 0) {
		if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
			if (get) {
				strcpy(ip, words[1]);
			}
			log_printf("ntp: %s ip %s", get ? "get" : "set", ip);
		} else {
			log_printf("ntp: ntpser %s error %s", get ? "get" : "set", ip);
		}
	}
#endif
	return ret;
}

static const char *ntp_config_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body onload=\"check()\">"\
	"<h1>HFeasy - NTP client</h1><hr>"\
	"<form action=\"ntp\" method=\"GET\">"\
	"<table><tr><th colspan=\"2\">Time settings"\
	"<tr><td>System time<td>%s"\
	"<tr><td>Enabled<td><input id=\"ien\" type=\"checkbox\" name=\"en\" value=\"1\" %s onclick=\"check()\">"\
	"<tr><td>Server IP<td><input id=\"iip\" type=\"text\" name=\"ntpip\" value=\"%s\">"\
	"<tr><td>Sync interval (0~7200 minutes)<td><input id=\"isync\" type=\"text\" name=\"ntpsync\" value=\"%d\">"\
	"<tr><td><input type=\"submit\" value=\"Apply\" name=\"apply\"></table></form>"\
	"<script type=\"text/javascript\">"\
	"function check() {"\
  "if(document.getElementById(\"ien\").checked == true){"\
	"document.getElementById(\"iip\").disabled = false;"\
	"document.getElementById(\"isync\").disabled = false;"\
	"}else{"\
	"document.getElementById(\"iip\").disabled = true;"\
	"document.getElementById(\"isync\").disabled = true;"\
	"}}"\
	"</script>"\
	"</body></html>";


static void USER_FUNC httpd_page_ntp(char *url, char *rsp)
{
	char tmp[100];
	int ret;
	char now_s[50], ntpip[16], ans[150], *words[5] = {NULL};
	int ntpsync, en = 0;
	
	time_t now = time(NULL) - 8*60*60;

#if 0	
	/* get time */
	strcpy(tmp, "AT+NTPTM");
	if (at_cmd(tmp, words, 4, 99) > 0) {
		sprintf(now_s, "%s %s %s", words[1], words[2], words[3]);
		log_printf("ntp: time %s %s %s %s", words[1], words[2], words[3], words[4]);
	} else {
		strcpy(now_s, "error");
		log_printf("ntp: error getting time");
	}
#endif

	ret = httpd_arg_find(url, "apply", tmp);
	if (ret) {
		/* set ntp enabled/disabled */
		ret = httpd_arg_find(url, "en", tmp);
		if (ret) {
			strcpy(tmp, "AT+NTPEN=on");
			en = 1;
		} else {
			strcpy(tmp, "AT+NTPEN=off");
		}
		at_cmd(tmp, words, 1, 99);

		/* set ntp server ip */
		ret = httpd_arg_find(url, "ntpip", tmp);
		if (ret == 1) {
			strcpy(ntpip, tmp);
			ntp_serverip(ntpip, SET);
		}
		
		/* set ntp sync interval */
		ret = httpd_arg_find(url, "ntpsync", tmp);
		if (ret == 1) {
			ntpsync = atoi(tmp);
			snprintf(tmp, 49, "AT+NTPRF=%d\r\n", ntpsync / 10);
			hfat_send_cmd(tmp, sizeof(tmp), ans, 150);
			if (hfat_get_words(ans, words, 4) > 0) {
				if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
					log_printf("ntp: sync ok %d", ntpsync);
				} else {
					log_printf("ntp: error setting sync %d", ntpsync);
					strcpy(ntpip, "0.0.0.0");
				}
			}
		}


	} else {
		/* get ntp server ip */
		ntp_serverip(ntpip, GET);
		
		/* get sync interval */
		hfat_send_cmd("AT+NTPRF\r\n", sizeof("AT+NTPRF\r\n"), ans, 150);
		if (hfat_get_words(ans, words, 4) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				ntpsync = atoi(words[1]) * 10;
				log_printf("ntp: sync %d", ntpsync);
			}
		}

		/* get enabled/disabled */
		snprintf(tmp, 49, "AT+NTPEN\r\n");
		hfat_send_cmd(tmp, sizeof(tmp), ans, 150);
		if (hfat_get_words(ans, words, 2) > 0) {
			if ((ans[0]=='+') && (ans[1]=='o') && (ans[2]=='k')) {
				if (strcmp(words[1], "on") == 0)
					en = 1;
				log_printf("ntp: enabled %d", en);
			}
		}
	}
	
	snprintf(rsp, 1000, ntp_config_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
	ctime(&now), en ? "checked" : "", now_s, ntpip, ntpsync);
}	


static const char *timer_page =
	"<!DOCTYPE html><html><head><title>HFeasy v%d.%d</title><link rel=\"stylesheet\" href=\"/styles.css\"></head><body>"\
	"<h1>HFeasy timer</h1><hr>"\
	"<br><h2>Countdown timer</h2><br>"\
	"<form action=\"timer\" method=\"GET\">"\
	"Turn OFF (0 to disable): <input type=\"text\" name=\"cd02\" value=\"%d\" maxlength=\"4\" size=\"4\">h"\
		"<input type=\"text\" name=\"cd01\" value=\"%d\" maxlength=\"2\" size=\"2\">m"\
		"<input type=\"text\" name=\"cd00\" value=\"%d\" maxlength=\"2\" size=\"2\">s<br>"\
	"Turn ON (0 to disable): <input type=\"text\" name=\"cd12\" value=\"%d\" maxlength=\"4\" size=\"4\">h"\
		"<input type=\"text\" name=\"cd11\" value=\"%d\" maxlength=\"2\" size=\"2\">m"\
		"<input type=\"text\" name=\"cd10\" value=\"%d\" maxlength=\"2\" size=\"2\">s<br>"\
	"<input type=\"submit\" value=\"Apply\"></form>"\
	"<hr>"\
	"<h2>Timer (not functional - under implementation)</h2><br>"\
	"<form action=\"/timer\" method=\"GET\">"\
	"%s"\
	"Action <select name=\"t_t\"><option value=\"on\">Turn ON</option><option value=\"off\">Turn OFF</option></select>"\
	" at <input type=\"text\" name=\"t_h\" value=\"%d\" maxlength=\"2\" size=\"2\">h"\
	"<input type=\"text\" name=\"t_m\" value=\"%d\" maxlength=\"2\" size=\"2\">m"\
	" repeat <select name=\"t_r\"><option value=\"no\">No</option><option value=\"day\">Daily</option><option value=\"week\">Weekly</option></select>"\
	"<input type=\"submit\" value=\"Add\"></form>"\
	"</body></html>";

static void USER_FUNC httpd_page_timer(char *url, char *rsp)
{
	struct hfeasy_state *state = config_get_state();
	struct hfeasy_config *cfg = &state->cfg;
	char tmp[50], var[10];
	int ret;

	/* i:timer, j:0=s,1=m,2=h */
	int i, j, cd[2][3];
	
	time_t now = time(NULL);
	
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			cd[i][j] = -1;
			snprintf(var, 9, "cd%d%d", i, j);
			ret = httpd_arg_find(url, var, tmp);
			if (ret == 1){
				cd[i][j] = atoi(tmp);
				log_printf("timer: got %s=%s/%d ret=%d", var, tmp, cd[i][j], ret);
			}
		}
	}

	for (i = 0; i < 2; i++) {
		if (cd[i][0] >= 0 && cd[i][1] >= 0 && cd[i][2] >= 0) {
		/* new value */
		time2sec(&cfg->countdown[i], cd[i]);
		} else if (cfg->countdown[i] > 0) {
			/* get current value */
			sec2time(cfg->countdown[i], cd[i]);
		} else {
			/* invalid time */
			for (j = 0; j < 3; j++)
				cd[i][j] = 0;
		}
	}
	log_printf("timer: cdoff=%d cdon=%d", cfg->countdown[0], cfg->countdown[1]);
	
	snprintf(rsp, 1200, timer_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
			cd[0][2], cd[0][1], cd[0][0], cd[1][2], cd[1][1], cd[1][0], ctime(&now), 0, 0);
}	


static void USER_FUNC timer_thread(void *opaque)
{
	struct hfeasy_state *state = (struct hfeasy_state *) opaque;
	uint32_t now;
	int i;
	
	while (1) {
		msleep(1000);
		
		for (i = 0; i < 2; i++) {
		
			if (state->cfg.countdown[i] > 0) {
				now = hfsys_get_time() / 1000;
				if (state->countdown[i] > 0) {
					/* turn OFF/ON timer running */
					if (state->relay_state == i) {
						//u_printf("ABORT %s COUNTDOWN\r\n", (i == 0) ? "OFF" : "ON");
						state->countdown[i] = 0;
					} else if (now > state->countdown[i]) {
						//u_printf("DONE OFF/ON\n\r");
						state->countdown[i] = 0;
						
						if (state->func_state & FUNC_RELAY)
							relay_set((i == 0) ? RELAY_OFF : RELAY_ON, RELAY_SRC_TIMER);
						
						if (state->func_state & FUNC_DIMMER)
							dimmer_set((i == 0) ? DIMMER_OFF : DIMMER_ON, RELAY_SRC_TIMER);
						
					}
				} else {
					/* wait to start */
					if (state->relay_state == (1 - i)) {
						/* start turn OFF/ON timer */
						state->countdown[i] = now + state->cfg.countdown[i];
						//u_printf("turn %s after %lu sec, time = %lu\r\n", (i == 0) ? "OFF" : "ON", state->cfg.countdown[i], state->countdown[i]);
					}
				}
			}
		}
	}
}


void USER_FUNC timer_init(void)
{
	struct hfeasy_state *state = config_get_state();

	
	/* start timer thread */
	if (hfthread_create((PHFTHREAD_START_ROUTINE) timer_thread,
					"timer", 256, state, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("timer thread create failed!\n");
	}
	
	httpd_add_page("/timer", httpd_page_timer, NULL);
	httpd_add_page("/ntp", httpd_page_ntp, NULL);
}
