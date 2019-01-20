
#include "hfeasy.h"


static hftimer_handle_t debounce_timer, recovery_timer;
static uint8_t key_counter;

inline int USER_FUNC gpio_get_state(int fid)
{
	return hfgpio_fpin_is_high(fid) ? 1 : 0;
}

void USER_FUNC gpio_set_led(uint8_t st)
{
#if defined (__LPB100__)
	if (st)
		hfgpio_fset_out_low(GPIO_LED);
	else
		hfgpio_fset_out_high(GPIO_LED);
#endif
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

	ret = httpd_arg_find(url, "tone", val);
	if (ret == 1) {
		buzzer_play(atoi(val));
		u_printf("playing tone %d\r\n", atoi(val));
	}

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

static void USER_FUNC recovery_mode(void)
{
	struct hfeasy_state *state = config_get_state();
	char rsp[64] = {0};

	hfat_send_cmd("AT+WMODE=AP\r\n", sizeof("AT+WMODE=AP\r\n"), rsp, 32);
	state->cfg.ver = 0;
	config_save();
	reboot();
}

#if defined(__LPT100F__)
static void USER_FUNC recovery_timer_handler(hftimer_handle_t timer)
{
	if (key_counter == 6) {
		recovery_mode();
	}
	key_counter = 0;
}
#endif

static void USER_FUNC debounce_timer_handler(hftimer_handle_t timer)
{
#if defined (__LPB100__)
	if (gpio_get_state(GPIO_SWITCH) == 0) {
		if (++key_counter > 5 * 3) {
			gpio_set_led(1);
			recovery_mode();
		}
		hftimer_start(timer);
} else
#endif
		hfgpio_fenable_interrupt(GPIO_SWITCH);
}

static void USER_FUNC switch_irqhandler(uint32_t arg1, uint32_t arg2)
{
	hfgpio_fdisable_interrupt(GPIO_SWITCH);
	gpio_set_relay(2, 1);
	hftimer_start(debounce_timer);

#if defined(__LPT100F__)
	if (key_counter == 0)
		hftimer_start(recovery_timer);
#endif
	key_counter++;
}

void USER_FUNC gpio_init(void)
{
	/* init gpio */
	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);

#if defined(__LPT100F__)
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");
	recovery_timer = hftimer_create("recovery", 2000, false, HFTIMER_ID_DEBOUNCE, recovery_timer_handler, 0);
	hftimer_start(recovery_timer);
#elif defined(__LPB100__)
	hfgpio_configure_fpin(GPIO_LED, HFM_IO_OUTPUT_1);
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");
#endif

	debounce_timer = hftimer_create("debouncer", 200, false, HFTIMER_ID_DEBOUNCE, debounce_timer_handler, 0);

	httpd_add_page("/state", switch_state_page);
}
