
#include "hfeasy.h"

/* module */
#define GPIO_BUZZER		         (HFGPIO_F_USER_DEFINE+0)
#define GPIO_RELAY		         (HFGPIO_F_USER_DEFINE+1)
#define GPIO_SWITCH		         (HFGPIO_F_USER_DEFINE+2)


static hftimer_handle_t debounce_timer;
static uint8_t debouncing = 0;


void USER_FUNC set_relay(uint8_t action, uint8_t publish)
{
	struct hfeasy_state *state = config_get_state();
	
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
		if (publish)
			mqttcli_publish(state->cfg.mqtt_on_value);
	} else {
		hfgpio_fset_out_low(GPIO_RELAY);
		if (publish)
			mqttcli_publish(state->cfg.mqtt_off_value);
	}
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
		set_relay(1, 1);
	else
		set_relay(0, 1);
}


static void USER_FUNC debounce_timer_handler(hftimer_handle_t timer)
{
	u_printf("timer finished\n");
	debouncing = 0;
}

static void USER_FUNC switch_irqhandler(uint32_t arg1, uint32_t arg2)
{
	if (!debouncing) {
		hftimer_start(debounce_timer);
		set_relay(2, 1);
		debouncing = 1;
	}
}

void USER_FUNC gpio_init(void)
{
	/* init gpio */
	hfgpio_configure_fpin(GPIO_BUZZER, HFM_IO_OUTPUT_0);
	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");

	
	debounce_timer = hftimer_create("debouncer", 100, false, 0, debounce_timer_handler, 0);
	
	httpd_add_page("/state", switch_state_page);
}
