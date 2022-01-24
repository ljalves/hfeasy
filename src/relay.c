#include "hfeasy.h"


void USER_FUNC relay_publish_state(void)
{
	struct hfeasy_state *state = config_get_state();
	char *val = state->relay_state ? state->cfg.mqtt_on_value : state->cfg.mqtt_off_value;
	mqttcli_publish(val, "power");
}


static inline void USER_FUNC set_relay_pin(uint8_t st)
{
	struct hfeasy_state *state = config_get_state();
	
	if (state->cfg.gpio_config[10] & GPIO_INV_RELAY)
		st ^= 1;
	
	if (st)
		hfgpio_fset_out_high(GPIO_RELAY);
	else
		hfgpio_fset_out_low(GPIO_RELAY);
}


void USER_FUNC relay_set(uint8_t action, uint8_t source)
{
	struct hfeasy_state *state = config_get_state();
	char *val;

	if (!(state->func_state & FUNC_RELAY))
		return;
	
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
	
	state->relay_modifier = source | ((action & 3) << 4);

	/* set gpio */
	if (state->relay_state) {
		if (state->cfg.wifi_led == LED_CONFIG_RELAY)
			led_ctrl("n");
		
		set_relay_pin(1);
		val = state->cfg.mqtt_on_value;
	} else {
		if (state->cfg.wifi_led == LED_CONFIG_RELAY)
			led_ctrl("f");

		set_relay_pin(0);
		val = state->cfg.mqtt_off_value;
	}
	hfsys_nvm_write(0, (char *) &state->relay_state, sizeof(state->relay_state));
	mqttcli_publish(val, "power");
}


void USER_FUNC relay_init(void)
{
	struct hfeasy_state *state = config_get_state();

	if (*gpio_pin(GPIO_RELAY) == HFM_NOPIN)
		return;

	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);
	state->func_state |= FUNC_RELAY;
	

	if (state->cfg.pwron_state == 0xff) {
		hfsys_nvm_read(0, (char *) &state->relay_state, sizeof(state->relay_state));
	} else {
		if (state->cfg.pwron_state)
			relay_set(RELAY_ON, RELAY_SRC_POWERON);
		else
			relay_set(RELAY_OFF, RELAY_SRC_POWERON);
	}
}

void USER_FUNC relay_deinit(void)
{
	struct hfeasy_state *state = config_get_state();

	if (state->func_state & FUNC_RELAY) {
		hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_TYPE_INPUT);
		state->func_state &= ~FUNC_RELAY;
	}
}

