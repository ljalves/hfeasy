#include "hfeasy.h"


void USER_FUNC relay_init(void)
{
	struct hfeasy_state *state = config_get_state();
	if (*gpio_pin(GPIO_RELAY) == HFM_NOPIN)
		return;

	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);
	
	if (state->cfg.pwron_state)
		gpio_set_relay(RELAY_ON, 1, RELAY_SRC_POWERON);
	else
		gpio_set_relay(RELAY_OFF, 1, RELAY_SRC_POWERON);
		
	
}

void USER_FUNC relay_deinit(void)
{
	if (*gpio_pin(GPIO_RELAY) != HFM_NOPIN)
		hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_TYPE_INPUT);
}


void USER_FUNC relay_publish_state(void)
{
	struct hfeasy_state *state = config_get_state();
	char *val = state->relay_state ? state->cfg.mqtt_on_value : state->cfg.mqtt_off_value;
	mqttcli_publish(val, "power");
}


void USER_FUNC gpio_set_relay(uint8_t action, uint8_t publish, uint8_t source)
{
	struct hfeasy_state *state = config_get_state();
	char *val;
	int changed = (action != state->relay_state);
	
	switch(action) {
		default:
		case RELAY_OFF:
			/* off */
			state->relay_state = 0;
			break;
		case RELAY_ON:
			/* on */
			state->relay_state = 1;
			break;
		case RELAY_TOGGLE:
			/* toggle */
			state->relay_state ^= 1;
			break;
	}
	
	state->relay_modifier = source | ((publish & 1) << 6) | ((action & 3) << 4);
	
	if (*gpio_pin(GPIO_RELAY) == HFM_NOPIN)
		return;

	/* set gpio */
	if (state->relay_state) {
		if (state->cfg.wifi_led == LED_CONFIG_RELAY)
			led_ctrl("n");
		
		hfgpio_fset_out_high(GPIO_RELAY);
		val = state->cfg.mqtt_on_value;
	} else {
		if (state->cfg.wifi_led == LED_CONFIG_RELAY)
			led_ctrl("f");

		hfgpio_fset_out_low(GPIO_RELAY);
		val = state->cfg.mqtt_off_value;
	}
	if (publish && changed)
		mqttcli_publish(val, "power");
}
