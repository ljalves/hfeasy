
#include "hfeasy.h"



static hftimer_handle_t debounce_timer;
static uint8_t debouncing = 0;

#if defined (__LPT100F__)
static hftimer_handle_t buzzer_timer;
static void USER_FUNC buzzer_timer_handler(hftimer_handle_t timer)
{
	static uint16_t f = 200, t = 1, i = 0;

	if (i >100) {
		hfgpio_pwm_disable(GPIO_BUZZER);
		hfgpio_fset_out_low(GPIO_BUZZER);
		f = 200;
		t = 1;
		i = 0;
	} else {
		hftimer_change_period(buzzer_timer, 1);
		hfgpio_pwm_enable(GPIO_BUZZER, f++, 1);
	}
}

void USER_FUNC gpio_buzzer_play(void)
{
	hftimer_start(buzzer_timer);
}
#endif

inline int USER_FUNC gpio_get_state(int fid)
{
	return hfgpio_fpin_is_high(fid) ? 1 : 0;
}


void USER_FUNC gpio_set_relay(uint8_t action, uint8_t publish)
{
	struct hfeasy_state *state = config_get_state();
	char *val;
	int changed = (action != state->relay_state);
	
	switch(action) {
		default:
		case 0:
			/* off */
			state->relay_state = 0;
			break;
		case 1:
			/* on */
			state->relay_state = 1;
			break;
		case 2:
			/* toggle */
			state->relay_state ^= 1;
			break;
	}
	
	/* set gpio */
	if (state->relay_state) {
		hfgpio_fset_out_high(GPIO_RELAY);
		val = state->cfg.mqtt_on_value;
	} else {
		hfgpio_fset_out_low(GPIO_RELAY);
		val = state->cfg.mqtt_off_value;
	}
	
	if (publish && changed)
		mqttcli_publish(val);
}

static void USER_FUNC switch_state_page(char *url, char *rsp)
{
	char val[20];
	int ret;

	ret = httpd_arg_find(url, "sw", val);
	if (ret == 2)
		strcpy(val, "none");
	
	sprintf(rsp, "<html><head><title>SWITCH STATE</title></head>" \
							 "<body>SWITCH STATE WEB PAGE<br>" \
							 "ret=%d, sw='%s'" \
							 "</body></html>", ret, val);

	u_printf("ret=%d, sw='%s', page '%s' served\r\n", ret, val, url);	
	
	/* set relay */
	if (strcmp(val, "1") == 0)
		gpio_set_relay(1, 1);
	else
		gpio_set_relay(0, 1);
}


static void USER_FUNC debounce_timer_handler(hftimer_handle_t timer)
{
#if defined (__LPB100__)
	struct hfeasy_state *state = config_get_state();
	char rsp[64]={0};

	if (gpio_get_state(GPIO_SWITCH) == 0) {
		if (++debouncing > 5 * 3) {
			hfgpio_fset_out_low(GPIO_LED);
	
			hfat_send_cmd("AT+WMODE=AP\r\n", sizeof("AT+WMODE=AP\r\n"), rsp, 32);
			state->cfg.ver = 0;
			config_save();
			reboot();
		}

		hftimer_start(timer);
} else
#endif
	debouncing = 0;
}

static void USER_FUNC switch_irqhandler(uint32_t arg1, uint32_t arg2)
{
	if (!debouncing) {
		hftimer_start(debounce_timer);
		gpio_set_relay(2, 1);
		debouncing = 1;
	}
}

void USER_FUNC gpio_init(void)
{
	/* init gpio */
	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);

#if defined(__LPT100F__)
	hfgpio_configure_fpin(GPIO_BUZZER, HFM_IO_OUTPUT_0);
	buzzer_timer = hftimer_create("buzzer", 200, false, 0, buzzer_timer_handler, 0);

	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");
#elif defined(__LPB100__)
	hfgpio_configure_fpin(GPIO_LED, HFM_IO_OUTPUT_1);
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");
#endif
	

	debounce_timer = hftimer_create("debouncer", 200, false, 0, debounce_timer_handler, 0);

	httpd_add_page("/state", switch_state_page);
}
