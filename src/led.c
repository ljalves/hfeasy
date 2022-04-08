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


static hftimer_handle_t led_timer[2];
static char *led_actions[2];
static uint8_t led_idx[2] = {0, 0};

static inline void USER_FUNC set_led(uint8_t l, uint8_t st)
{
	struct hfeasy_state *state = config_get_state();
	int led_gpio = (l == 0) ? GPIO_LED1 : GPIO_LED2;
	int gpio_inv = (l == 0) ? GPIO_INV_LED1 : GPIO_INV_LED2;
	
	if (state->cfg.gpio_config[CONFIG_GPIO_CONFIG] & gpio_inv)
		st ^= 1;

	if (st)
		hfgpio_fset_out_high(led_gpio);
	else
		hfgpio_fset_out_low(led_gpio);
}


static void USER_FUNC led_timer_handler(hftimer_handle_t timer)
{
	struct hfeasy_state *state = config_get_state();
	uint32_t id = hftimer_get_timer_id(timer);
	uint8_t l = (id == HFTIMER_ID_LED1) ? 0 : 1;

	if (state->cfg.led[l] == LED_CONFIG_OFF) {
		set_led(l, 0);
		return;
	}

	switch (led_actions[l][led_idx[l]]) {
		case LED_ACTION_END:
			led_idx[l] = 0;
			break;
		
		case LED_ACTION_ON:
			set_led(l, 1);
			led_idx[l]++;
			hftimer_change_period(timer, 10);
			break;
		
		case LED_ACTION_OFF:
			set_led(l, 0);
			led_idx[l]++;
			hftimer_change_period(timer, 10);
			break;
		
		case LED_ACTION_DELAY100:
			led_idx[l]++;
			hftimer_change_period(timer, 100);
			break;
		case LED_ACTION_DELAY200:
			led_idx[l]++;
			hftimer_change_period(timer, 200);
			break;
		case LED_ACTION_DELAY500:
			led_idx[l]++;
			hftimer_change_period(timer, 500);
			break;
		case LED_ACTION_DELAY1000:
			led_idx[l]++;
			hftimer_change_period(timer, 1000);
			break;
		
		case LED_ACTION_REPEAT:
			led_idx[l] = 0;
			hftimer_change_period(timer, 10);
			break;
		default:
			led_idx[l]++;
			hftimer_change_period(timer, 10);
			break;
	}
}

void USER_FUNC led_ctrl(uint8_t led, char *a)
{
	struct hfeasy_state *state = config_get_state();
	
	int f = (led == 0) ? FUNC_LED1 : FUNC_LED2;
	
	if (state->func_state & f) {
		if (state->cfg.led[led] == LED_CONFIG_OFF) {
			set_led(led, 0);
		} else {
			hftimer_stop(led_timer[led]);
			strcpy(led_actions[led], a);
			led_idx[led] = 0;
			hftimer_change_period(led_timer[led], 10);
		}
	}
}

void USER_FUNC leds_ctrl_if(int c, char *a_true, char *a_false)
{
	struct hfeasy_state *state = config_get_state();
	int l;
	
	for (l = 0; l < 2; l++) {
		if (state->cfg.led[l] == c) {
			if (a_true != NULL)
				led_ctrl(l, a_true);
		} else {
			if (a_false != NULL)
				led_ctrl(l, a_false);
		}
	}
}

void USER_FUNC led_init(void)
{
	struct hfeasy_state *state = config_get_state();
	int i;
	
	for (i = 0; i < 2; i++) {
		int led_gpio = (i == 0) ? GPIO_LED1 : GPIO_LED2;
		int gpio_inv = (i == 0) ? GPIO_INV_LED1 : GPIO_INV_LED2;
		int tmr_id = (i == 0) ? HFTIMER_ID_LED1 : HFTIMER_ID_LED2;
		int f = (i == 0) ? FUNC_LED1 : FUNC_LED2;
		const char *tmr_name = (i == 0) ? "led1" : "led2";
	
		if (*gpio_pin(led_gpio) != HFM_NOPIN) {
			led_actions[i] = hfmem_malloc(50);
			if (led_actions[i] == NULL) {
				log_printf("led: no mem!");
				return;
			}
			hfgpio_configure_fpin(led_gpio, state->cfg.gpio_config[CONFIG_GPIO_CONFIG] & gpio_inv ? HFM_IO_OUTPUT_1 : HFM_IO_OUTPUT_0);

			led_idx[i] = 0;
			strcpy(led_actions[i], "");
			led_timer[i] = hftimer_create(tmr_name, 1, false, tmr_id, led_timer_handler, 0);
			
			state->func_state |= f;
		}

	}
	
}

void USER_FUNC led_deinit(void)
{
	struct hfeasy_state *state = config_get_state();

	if (state->func_state & FUNC_LED1) {
		hfgpio_configure_fpin(GPIO_LED1, HFM_IO_TYPE_INPUT);
		hftimer_delete(led_timer[0]);
		hfmem_free(led_actions[0]);
		state->func_state &= ~FUNC_LED1;
	}
	if (state->func_state & FUNC_LED2) {
		hfgpio_configure_fpin(GPIO_LED2, HFM_IO_TYPE_INPUT);
		hftimer_delete(led_timer[1]);
		hfmem_free(led_actions[1]);
		state->func_state &= ~FUNC_LED2;
	}
}
