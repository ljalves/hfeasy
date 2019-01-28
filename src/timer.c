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


static const char *timer_page =
	"<!DOCTYPE html><html><head><title>HFeasy config v%d.%d</title></head><body>"\
	"<h1>HFeasy timers</h1><hr>"\
	"<h2>Countdown timer</h2><br>"\
	"<form action=\"/timer\" method=\"GET\">"\
	"Turn OFF (0 to disable): <input type=\"text\" name=\"cd0_h\" value=\"%d\" maxlength=\"4\" size=\"4\">h"\
		"<input type=\"text\" name=\"cd0_m\" value=\"%d\" maxlength=\"2\" size=\"2\">m"\
		"<input type=\"text\" name=\"cd0_s\" value=\"%d\" maxlength=\"2\" size=\"2\">s<br>"\
	"Turn ON (0 to disable): <input type=\"text\" name=\"cd1_h\" value=\"%d\" maxlength=\"4\" size=\"4\">h"\
		"<input type=\"text\" name=\"cd1_m\" value=\"%d\" maxlength=\"2\" size=\"2\">m"\
		"<input type=\"text\" name=\"cd1_s\" value=\"%d\" maxlength=\"2\" size=\"2\">s<br>"\
	"<input type=\"submit\" value=\"Apply\"></form>"\
	"<hr><form action=\"/timer\" method=\"GET\"><input type=\"submit\" value=\"Save to flash\" name=\"save\"></form>"\
	"</body></html>";

static void USER_FUNC httpd_page_timer(char *url, char *rsp)
{
	struct hfeasy_state *state = config_get_state();
	struct hfeasy_config *cfg = &state->cfg;
	char tmp[50];
	int ret;
	time_t countdown;
	int cd0_h, cd0_m, cd0_s;
	int cd1_h, cd1_m, cd1_s;
	
	
	ret = httpd_arg_find(url, "cd0_h", tmp);
	if (ret > 0)
		cd0_h = atoi(tmp);
	else
		cd0_h = -1;

	ret = httpd_arg_find(url, "cd0_m", tmp);
	if (ret > 0)
		cd0_m = atoi(tmp);
	else
		cd0_m = -1;

	ret = httpd_arg_find(url, "cd0_s", tmp);
	if (ret > 0)
		cd0_s = atoi(tmp);
	else
		cd0_s = -1;

	ret = httpd_arg_find(url, "cd1_h", tmp);
	if (ret > 0)
		cd1_h = atoi(tmp);
	else
		cd1_h = -1;

	ret = httpd_arg_find(url, "cd1_m", tmp);
	if (ret > 0)
		cd1_m = atoi(tmp);
	else
		cd1_m = -1;

	ret = httpd_arg_find(url, "cd1_s", tmp);
	if (ret > 0)
		cd1_s = atoi(tmp);
	else
		cd1_s = -1;


	if (cd0_h >= 0 && cd0_m >= 0 && cd0_s >= 0) {
		countdown = cd0_s;
		countdown += cd0_m * 60;
		countdown += cd0_h * 60 * 60;
		/* new value */
		cfg->countdown[0] = countdown;
	} else if (cfg->countdown[0] > 0) {
		countdown = cfg->countdown[0];
		cd0_s = countdown % 60;
		countdown /= 60;
		cd0_m = countdown % 60;
		countdown /= 60;
		cd0_h = countdown;
	} else {
		cd0_h = 0;
		cd0_m = 0;
		cd0_s = 0;
	}

	if (cd1_h >= 0 && cd1_m >= 0 && cd1_s >= 0) {
		countdown = cd1_s;
		countdown += cd1_m * 60;
		countdown += cd1_h * 60 * 60;
		/* new value */
		cfg->countdown[1] = countdown;
	} else if (cfg->countdown[1] > 0) {
		countdown = cfg->countdown[1];
		cd1_s = countdown % 60;
		countdown /= 60;
		cd1_m = countdown % 60;
		countdown /= 60;
		cd1_h = countdown;
	} else {
		cd1_h = 0;
		cd1_m = 0;
		cd1_s = 0;
	}

	ret = httpd_arg_find(url, "save", tmp);
	if (ret > 0) {
		config_save();
		reboot();
	}
	
	sprintf(rsp, timer_page, HFEASY_VERSION_MAJOR, HFEASY_VERSION_MINOR,
			cd0_h, cd0_m, cd0_s, cd1_h, cd1_m, cd1_s);
}	


static void USER_FUNC timer_thread(void *opaque)
{
	struct hfeasy_state *state = (struct hfeasy_state *) opaque;
	uint32_t now;
	
	while (1) {
		msleep(1000);
		
		if (state->cfg.countdown[0] > 0) {
			now = hfsys_get_time() / 1000;
			if (state->countdown[0] > 0) {
				/* turn OFF timer running */
				if (state->relay_state == 0) {
					//u_printf("ABORT OFF COUNTDOWN\r\n");
					state->countdown[0] = 0;
				}
				
				if (now > state->countdown[0]) {
					//u_printf("DONE OFF\n\r");
					state->countdown[0] = 0;
					gpio_set_relay(RELAY_OFF, 1);
				}
			} else {
				/* wait to start */
				if (state->relay_state) {
					/* start turn OFF timer */
					state->countdown[0] = now + state->cfg.countdown[0];
					//u_printf("turn OFF after %lu sec, time = %lu\r\n", state->cfg.countdown[0], state->countdown[0]);
				}
			}
		}

		if (state->cfg.countdown[1] > 0) {
			now = hfsys_get_time() / 1000;
			if (state->countdown[1] > 0) {
				/* turn ON timer running */
				if (state->relay_state == 1) {
					//u_printf("ABORT ON COUNTDOWN\r\n");
					state->countdown[1] = 0;
				}
				
				if (now > state->countdown[1]) {
					//u_printf("DONE ON\n\r");
					state->countdown[1] = 0;
					gpio_set_relay(RELAY_ON, 1);
				}
			} else {
				/* wait to start */
				if (state->relay_state == 0) {
					/* start turn ON timer */
					state->countdown[1] = now + state->cfg.countdown[1];
					//u_printf("turn ON after %lu sec, time = %lu\r\n", state->cfg.countdown[1], state->countdown[1]);
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

	
	httpd_add_page("/timer", httpd_page_timer);
}
