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


#if defined(__HFEASY_MODULE__) 
#ifdef __LPT100F__
static int module_type = HFM_TYPE_LPT100F;
#else
static int module_type = HFM_TYPE_LPT100;
#endif
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE] =
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HFM_NOPIN, //HF_M_PIN(11),	//HFGPIO_F_NLINK
	HFM_NOPIN, //HF_M_PIN(12),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
	
	HFM_NOPIN, //HF_M_PIN(18),	//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HF_M_PIN(12),	//	buzzer
	HF_M_PIN(11),	//	relay
	HF_M_PIN(18),	//	switch/button
};
#elif defined(__HFEASY_PLUG__)
static int module_type = HFM_TYPE_LPB100;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HFM_NOPIN, //HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HFM_NOPIN, //HF_M_PIN(43),	//HFGPIO_F_NLINK
	HFM_NOPIN, //HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HF_M_PIN(8),	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HF_M_PIN(43),	// led
	HF_M_PIN(44),	// relay
	HF_M_PIN(30),	// switch/button
};
#elif defined(__HFEASY_DIMMER__)
#ifdef __LPT100F__
static int module_type = HFM_TYPE_LPT100F;
#else
static int module_type = HFM_TYPE_LPT100;
#endif
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE] =
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HFM_NOPIN, //HF_M_PIN(11),	//HFGPIO_F_NLINK
	HFM_NOPIN, //HF_M_PIN(12),	//HFGPIO_F_NREADY
	HFM_NOPIN, //HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
	
	HFM_NOPIN, //HF_M_PIN(18),	//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HF_M_PIN(18),	//	up
	HF_M_PIN(11),	//	down
	HF_M_PIN(12),	//	on
	HF_M_PIN(23),	//	scl
	HF_M_PIN(20),	//	sda
};
#else
#error "Device not supported!"
#endif

static hftimer_handle_t debounce_timer, recovery_timer;
static volatile uint8_t key_counter = 0, recovery_counter = 0;

inline int USER_FUNC gpio_get_state(int fid)
{
	return hfgpio_fpin_is_high(fid) ? 1 : 0;
}


#if defined(__HFEASY_DIMMER__)
int USER_FUNC gpio_i2c_send(uint8_t addr, uint8_t data)
{
	int i, nack;
	uint8_t d;
	
	/* bitbang i2c */
	
	/* start */
	hfgpio_fset_out_low(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	/* send slave addr */
	d = addr;
	for (i = 0; i < 8; i++) {
		if (d & 0x80)
			hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		else
			hfgpio_fset_out_low(GPIO_I2C_SDA);

		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		hfgpio_fset_out_low(GPIO_I2C_SCL);
		d <<= 1;
	}
	/* release SDA */
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	/* read ack */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	nack = gpio_get_state(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	if (nack == 0) {
		/* ack received */
		/* send data */
		d = data;
		for (i = 0; i < 8; i++) {
			if (d & 0x80)
				hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
			else
				hfgpio_fset_out_low(GPIO_I2C_SDA);

			hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
			hfgpio_fset_out_low(GPIO_I2C_SCL);
			d <<= 1;
		}
		/* release SDA */
		hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		/* read ack */
		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		nack = gpio_get_state(GPIO_I2C_SDA);
		hfgpio_fset_out_low(GPIO_I2C_SCL);
	}
	
	/* stop condition */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	return nack;
}
#endif


void USER_FUNC gpio_set_led(uint8_t st)
{
#if defined (__HFEASY_PLUG__)
	if (st)
		hfgpio_fset_out_low(GPIO_LED);
	else
		hfgpio_fset_out_high(GPIO_LED);
#endif
}

void USER_FUNC gpio_set_dimmer(uint8_t lvl, uint8_t publish, uint8_t source)
{
	struct hfeasy_state *state = config_get_state();
	int changed = (lvl != state->relay_state);
	char buf[10];
	
	/* set dimmer level */
	//gpio_i2c_send(???);
	
	state->relay_state = lvl;

	if (publish && changed) {
		sprintf(buf, "%d", lvl);
		mqttcli_publish(buf);
	}
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
	struct hfeasy_state *state = config_get_state();
	char val[20];
	int ret;

	ret = httpd_arg_find(url, "tone", val);
	if (ret == 1) {
		buzzer_play(atoi(val));
		u_printf("playing tone %d\r\n", atoi(val));
	}

	ret = httpd_arg_find(url, "sw", val);
	if (ret != 1)
		strcpy(val, "none");


#if defined(__HFEASY_DIMMER__)
	uint8_t li = 0;
	uint16_t addr;

	char buf[20];
	uint8_t data;

	strcpy(rsp, "<html><head><title>I2C</title></head>" \
							 "<body>I2C SCAN<br>");
	
	addr = 0xffff;
	ret = httpd_arg_find(url, "a", val);
	if (ret == 1) {
		addr = atoi(val);
	}
	data = 0;
	ret = httpd_arg_find(url, "v", val);
	if (ret == 1) {
		data = atoi(val);
	}

	if (addr != 0xffff) {
		if (gpio_i2c_send((uint8_t)addr, data) == 0) {
			sprintf(buf, "sent %u=%u", addr, data);
			strcat(rsp, buf);
		} else {
			sprintf(buf, "sent failed");
			strcat(rsp, buf);
		}
	} else {
	
		li = 0;
		for (addr = 0; addr < 0x80; addr++)
		{
			if (gpio_i2c_send((uint8_t)(addr<<1), data) == 0) {
				sprintf(buf, " %u", addr);
				strcat(rsp, buf);
				li++;
			}
			msleep(1);
		}
	}
	
	sprintf(buf, " | t=%u", li);
	strcat(rsp, buf);
	strcat(rsp, "</body></html>");
	//sprintf(rsp, "<html><head><title>I2C STATE</title></head>" \
	//						 "<body>I2C SCAN RESULT<br>" \
	//						 "found=%u, addrs=%s" \
	//						 "</body></html>", li, str);
#else

	
	sprintf(rsp, "<html><head><title>SWITCH STATE</title></head>" \
							 "<body>SWITCH STATE WEB PAGE<br>" \
							 "ret=%d, sw='%s' relay_state=%d" \
							 "</body></html>", ret, val, state->relay_state);

	//u_printf("ret=%d, sw='%s' relay_state=%d\r\n", ret, val, state->relay_state);

	/* set relay */
	if (strcmp(val, "1") == 0)
		gpio_set_relay(1, 1, RELAY_SRC_HTTP);
	else if (strcmp(val, "0") == 0)
		gpio_set_relay(0, 1, RELAY_SRC_HTTP);

#endif
	
}


const char *switch_ctrl_page_html =
	"<html><head><title>HFeasy Control</title></head><body>"
	"<button onclick=\"turnOn()\">ON</button>"
	"<button onclick=\"turnOff()\">OFF</button>"
	"<script>"
	"function turnOn(){"
		"const http = new XMLHttpRequest();"
		"http.open(\"GET\", \"/state?sw=1\", false);"
		"http.send(null);}"
	"function turnOff(){"
		"const http = new XMLHttpRequest();"
		"http.open(\"GET\", \"/state?sw=0\", false);"
		"http.send(null);}"
	"</script></body></html>";

static void USER_FUNC switch_ctrl_page(char *url, char *rsp)
{
	strcpy(rsp, switch_ctrl_page_html);
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

static void USER_FUNC recovery_timer_handler(hftimer_handle_t timer)
{
	if (recovery_counter > 5) {
		recovery_mode();
	}
	recovery_counter = 0;
}

static int prev_sw_state;

static void USER_FUNC debounce_timer_handler(hftimer_handle_t timer)
{
#if (defined (__HFEASY_PLUG__) || defined(__HFEASY_DIMMER__))
	if (gpio_get_state(GPIO_SWITCH) == 0) {
		if (recovery_counter++ == 0) {
			hftimer_start(recovery_timer);
		}
		hftimer_start(timer);
	} else {
		hftimer_stop(recovery_timer);
		recovery_counter = 0;
		key_counter = 0;
	}
#else 
	key_counter = 0;
	
	int sw_state = hfgpio_fpin_is_high(GPIO_SWITCH) ? 1 : 0;
	if (prev_sw_state == sw_state)
		return;
		
	gpio_set_relay(RELAY_TOGGLE, 1, RELAY_SRC_SWITCH);
	prev_sw_state = sw_state;
	
	if (recovery_counter++ == 0) {
		hftimer_start(recovery_timer);
	}
#endif
}


static void USER_FUNC switch_irqhandler(uint32_t arg1, uint32_t arg2)
{
	if (key_counter == 0) {
		key_counter++;
#if defined(__HFEASY_PLUG__)
		gpio_set_relay(RELAY_TOGGLE, 1, RELAY_SRC_SWITCH);
#endif
#if defined(__HFEASY_DIMMER__)
		gpio_set_dimmer(0, 1, RELAY_SRC_SWITCH);
#endif
	}
	hftimer_start(debounce_timer);
}


#if defined(__HFEASY_DIMMER__)
#define MAX_LIGHT_LEVEL 5

static void USER_FUNC switch_up_timer_cbk( hftimer_handle_t htimer)
{
	struct hfeasy_state *state = config_get_state();

	if (hfgpio_fpin_is_high(GPIO_SWITCH_UP))
		return;
	
	if (state->relay_state < MAX_LIGHT_LEVEL)
		gpio_set_dimmer(state->relay_state + 1, 1, RELAY_SRC_SWITCH_UP);
}

static void USER_FUNC switch_up_irqhandler(uint32_t arg1, uint32_t arg2)
{
	struct hfeasy_state *state = config_get_state();
	static hftimer_handle_t dimmer_kup_timer = NULL;

	if (state->relay_state == 0)
		return;

	if(dimmer_kup_timer == NULL)
		dimmer_kup_timer = hftimer_create("key_up_debounce", 30, false, HFTIMER_ID_UP, switch_up_timer_cbk, 0);

	hftimer_change_period(dimmer_kup_timer, 30);
}

static void USER_FUNC switch_dn_timer_cbk( hftimer_handle_t htimer )
{
	struct hfeasy_state *state = config_get_state();

	if(hfgpio_fpin_is_high(GPIO_SWITCH_DN))
		return;

	if (state->relay_state > 1)
		gpio_set_dimmer(state->relay_state - 1, 1, RELAY_SRC_SWITCH_DN);
}

static void USER_FUNC switch_dn_irqhandler(uint32_t arg1, uint32_t arg2)
{
	static hftimer_handle_t dimmer_kdn_timer = NULL;

	if (dimmer_kdn_timer == NULL)
		dimmer_kdn_timer = hftimer_create("key_down_debounce", 30, false, HFTIMER_ID_DN, switch_dn_timer_cbk, 0);

	hftimer_change_period(dimmer_kdn_timer, 30);
}

#endif



void USER_FUNC gpio_init(void)
{
	/* init gpio */
	if (hfgpio_fmap_check(module_type) != 0) {
		HF_Debug(DEBUG_ERROR,"error: gpio map file error\n");
		while(1)
			msleep(1000);
	}

	prev_sw_state = gpio_get_state(GPIO_SWITCH);

#if defined (__HFEASY_PLUG__)
	/* status led */
	hfgpio_configure_fpin(GPIO_LED, HFM_IO_OUTPUT_1);
#endif
	
#if (defined(__HFEASY_MODULE__) || defined (__HFEASY_PLUG__))
	/* relay control pin */
	hfgpio_configure_fpin(GPIO_RELAY, HFM_IO_OUTPUT_0);
#endif

#if defined(__HFEASY_MODULE__)
	/* on/off button */
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");
#endif
	
#if (defined(__HFEASY_PLUG__) || defined(__HFEASY_DIMMER__))
	/* on/off toggle button */
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add switch interrupt\n");	
#endif
	
#if defined(__HFEASY_DIMMER__)
	/* up/down buttons */
	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_UP,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_up_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add key up interrupt\n");

	if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_DN,
				HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
				switch_dn_irqhandler, 1) != HF_SUCCESS)
		u_printf("failed to add key down interrupt\n");
	
	/* I2C pins */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFPIO_DEFAULT | HFM_IO_TYPE_INPUT);
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFPIO_DEFAULT | HFM_IO_TYPE_INPUT);
#endif
	
	debounce_timer = hftimer_create("debouncer", 10, false, HFTIMER_ID_DEBOUNCE, debounce_timer_handler, 0);
	recovery_timer = hftimer_create("recovery", 3000, false, HFTIMER_ID_RECOVERY, recovery_timer_handler, 0);

	httpd_add_page("/state", switch_state_page);
	httpd_add_page("/ctrl", switch_ctrl_page);
}
