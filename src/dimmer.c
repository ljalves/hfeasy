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

#define MAX_LIGHT_LEVEL 0x80
#define I2C_ADDR	0x1e

static int is_first_sw = 1;


void USER_FUNC dimmer_publish_state(void)
{
	struct hfeasy_state *state = config_get_state();
	char buf[10];
	uint8_t lvl;
	
	lvl = state->dimmer_level;
	
	sprintf(buf, "%d", lvl);
	mqttcli_publish(buf, "dimmer");
}

void USER_FUNC dimmer_set(uint8_t lvl, uint8_t source)
{
	struct hfeasy_state *state = config_get_state();
	char buf[10];

	/* lvl=0xff - restore previous level */
	if (lvl == DIMMER_ON)
		lvl = state->dimmer_level;

	/* lvl=0xfe = toggle state */
	if (lvl == DIMMER_TOGGLE) {
		state->relay_state ^= 1;
		if (state->relay_state == 1)
			lvl = state->dimmer_level;
		else
			lvl = 0;
	} else if (lvl == 0)
		state->relay_state = 0;
	else
		state->relay_state = 1;

	if (state->relay_state == 0) {
		gpio_i2c_send(I2C_ADDR, 0);
	} else {

		/* top limit */
		if (lvl > 0x80)
			lvl = 0x80;

		/* send dimmer level */
		gpio_i2c_send(I2C_ADDR, lvl);
	
		if (lvl > 0)
			state->dimmer_level = lvl;
	}
	
	sprintf(buf, "%d", lvl);
	mqttcli_publish(buf, "dimmer");
}



static void USER_FUNC switch_up_timer_cbk(hftimer_handle_t htimer)
{
	struct hfeasy_state *state = config_get_state();

	if (hfgpio_fpin_is_high(GPIO_SWITCH_UP))
		return;
	
	if (state->dimmer_level < MAX_LIGHT_LEVEL)
		dimmer_set(state->dimmer_level + 6, RELAY_SRC_SWITCH_UP);

	if (is_first_sw)
		hftimer_change_period(htimer, 600);
	else
		hftimer_change_period(htimer, 100);
	is_first_sw = 0;
}

static void USER_FUNC switch_up_irqhandler(uint32_t arg1, uint32_t arg2)
{
	struct hfeasy_state *state = config_get_state();
	static hftimer_handle_t dimmer_kup_timer = NULL;

	if(dimmer_kup_timer == NULL)
		dimmer_kup_timer = hftimer_create("key_up_debounce", 30, false, HFTIMER_ID_UP, switch_up_timer_cbk, 0);

	is_first_sw = 1;
	hftimer_change_period(dimmer_kup_timer, 30);
}

static void USER_FUNC switch_dn_timer_cbk( hftimer_handle_t htimer )
{
	struct hfeasy_state *state = config_get_state();

	if(hfgpio_fpin_is_high(GPIO_SWITCH_DN))
		return;

	if (state->dimmer_level > 7)
		dimmer_set(state->dimmer_level - 6, RELAY_SRC_SWITCH_DN);
	else if (state->dimmer_level > 1)
		dimmer_set(1, RELAY_SRC_SWITCH_DN);

	if (is_first_sw)
		hftimer_change_period(htimer, 600);
	else
		hftimer_change_period(htimer, 100);
	is_first_sw = 0;
}

static void USER_FUNC switch_dn_irqhandler(uint32_t arg1, uint32_t arg2)
{
	static hftimer_handle_t dimmer_kdn_timer = NULL;

	if (dimmer_kdn_timer == NULL)
		dimmer_kdn_timer = hftimer_create("key_down_debounce", 30, false, HFTIMER_ID_DN, switch_dn_timer_cbk, 0);

	is_first_sw = 1;
	hftimer_change_period(dimmer_kdn_timer, 30);
}


void USER_FUNC dimmer_init(void)
{
	struct hfeasy_state *state = config_get_state();

	if (*gpio_pin(GPIO_I2C_SCL) == HFM_NOPIN || *gpio_pin(GPIO_I2C_SDA) == HFM_NOPIN)
		return;

	/* I2C pins */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFPIO_DEFAULT | HFM_IO_TYPE_INPUT);
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFPIO_DEFAULT | HFM_IO_TYPE_INPUT);
	state->func_state |= FUNC_I2C;
	
	/* up button */
	if (*gpio_pin(GPIO_SWITCH_UP) != HFM_NOPIN) {
		if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_UP,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_up_irqhandler, 1) != HF_SUCCESS)
			u_printf("failed to add key up interrupt\n");
		state->func_state |= FUNC_BTN_UP;
	}

	/* down button */
	if (*gpio_pin(GPIO_SWITCH_DN) != HFM_NOPIN) {
		if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_DN,
					HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
					switch_dn_irqhandler, 1) != HF_SUCCESS)
			u_printf("failed to add key down interrupt\n");
		state->func_state |= FUNC_BTN_DN;
	}	

	state->dimmer_level = 0x80;
	dimmer_set(state->cfg.pwron_state, RELAY_SRC_POWERON);
	state->func_state |= FUNC_DIMMER;
}

void USER_FUNC dimmer_deinit(void)
{
	struct hfeasy_state *state = config_get_state();

	if (state->func_state & FUNC_I2C) {
		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		state->func_state &= ~FUNC_I2C;
	}
	
	if (state->func_state & FUNC_BTN_UP) {
		hfgpio_fdisable_interrupt(GPIO_SWITCH_UP);
		state->func_state &= ~FUNC_BTN_UP;
	}

	if (state->func_state & FUNC_BTN_DN) {
		hfgpio_fdisable_interrupt(GPIO_SWITCH_DN);
		state->func_state &= ~FUNC_BTN_DN;
	}
	
	state->func_state &= ~FUNC_DIMMER;
}
