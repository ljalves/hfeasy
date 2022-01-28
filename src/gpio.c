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


#if defined(__LPXX00__)

const int gpio_lpt100[11] =
{
	HFM_NOPIN,
	HFM_PIN1,
	HFM_PIN9,
	HFM_PIN45,
	HFM_PIN47,
	HFM_PIN41,
	HFM_PIN39,
	HFM_PIN25,
	HFM_PIN18,
	HFM_PIN12,
	HFM_PIN11
};

const int gpio_lpt100f[11] =
{
	HFM_NOPIN,
	HFM_PIN1,
	HFM_PIN9,
	HFM_PIN45,
	HFM_PIN23,
	HFM_PIN41,
	HFM_PIN39,
	HFM_PIN20,
	HFM_PIN18,
	HFM_PIN12,
	HFM_PIN11
};

const int gpio_lpb100[49] = 
{
	HFM_NOPIN,
	HFM_PIN1,
	HFM_PIN2,
	HFM_PIN3,
	HFM_PIN4,
	HFM_PIN5,
	HFM_PIN6,
	HFM_PIN7,
	HFM_PIN8,
	HFM_PIN9,
	HFM_PIN10,
	HFM_PIN11,
	HFM_PIN12,
	HFM_PIN13,
	HFM_PIN14,
	HFM_PIN15,
	HFM_PIN16,
	HFM_PIN17,
	HFM_PIN18,
	HFM_PIN19,
	HFM_PIN20,
	HFM_PIN21,
	HFM_PIN22,
	HFM_PIN23,
	HFM_PIN24,
	HFM_PIN25,
	HFM_PIN26,
	HFM_PIN27,
	HFM_PIN28,
	HFM_PIN29,
	HFM_PIN30,
	HFM_PIN31,
	HFM_PIN32,
	HFM_PIN33,
	HFM_PIN34,
	HFM_PIN35,
	HFM_PIN36,
	HFM_PIN37,
	HFM_PIN38,
	HFM_PIN39,
	HFM_PIN40,
	HFM_PIN41,
	HFM_PIN42,
	HFM_PIN43,
	HFM_PIN44,
	HFM_PIN45,
	HFM_PIN46,
	HFM_PIN47,
	HFM_PIN48,
};

#define NR_MODULES 5

const int *gpio_module_types[NR_MODULES] = {
	gpio_lpb100,
	gpio_lpt100,
	NULL,
	NULL,
	gpio_lpt100f,
};

const int gpio_module_nrpins[NR_MODULES] = {
	48,
	10,
	0,
	0,
	10
};

int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HFM_NOPIN,	//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDI
	HFM_NOPIN,	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HFM_NOPIN, //HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HFM_NOPIN, //HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HFM_NOPIN, //HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HFM_NOPIN, //HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HFM_NOPIN, //HF_M_PIN(43),	//HFGPIO_F_NLINK
	HFM_NOPIN, //HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN, //HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN, //HF_M_PIN(8),	//HFGPIO_F_SLEEP_ON
		
	HFM_NOPIN, //HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,		/* led wifi */
	HFM_NOPIN,		/* relay */
	HFM_NOPIN,		/* push button */
	HFM_NOPIN,		/* toggle button */
	HFM_NOPIN,		/* button up */
	HFM_NOPIN,		/* button dw */
	HFM_NOPIN,		/* buzzer */
	HFM_NOPIN,		/* i2c ck */
	HFM_NOPIN,		/* i2c dt */
};

#elif defined(__LPXX30__)

int gpio_lpb130[35] = 
{
	HFM_NOPIN,
	HFM_NOPIN, //HFM_PIN1, GND
	HFM_NOPIN, //HFM_PIN2, N.C.
	HFM_NOPIN, //HFM_PIN3, N.C.
	LPx30_GPIO21, //HFM_PIN4,
	LPx30_GPIO5, //HFM_PIN5,
	LPx30_GPIO4, //HFM_PIN6,
	LPx30_GPADC0, //HFM_PIN7,
	HFM_NOPIN, //HFM_PIN8, N.C.
	HFM_NOPIN, //HFM_PIN9, DVDD3V3
	HFM_NOPIN, //HFM_PIN10, N.C.
	LPx30_GPIO3, //HFM_PIN11,
	LPx30_GPIO7, //HFM_PIN12,
	LPx30_GPIO13, //HFM_PIN13,
	LPx30_GPIO12, //HFM_PIN14,
	LPx30_GPIO0, //HFM_PIN15,
	LPx30_GPIO9, //HFM_PIN16,
	HFM_NOPIN, //HFM_PIN17, GND
	HFM_NOPIN, //HFM_PIN18, GND
	LPx30_GPIO26, //HFM_PIN19, UART1_RX
	HFM_NOPIN, //HFM_PIN20, DVDD3V3
	LPx30_GPIO27, //HFM_PIN21, UART1_TX
	HFGPIO_F_USBDM, //HFM_PIN22,
	HFGPIO_F_USBDP, //HFM_PIN23, PWM1
	HFM_NOPIN, //HFM_PIN24, N.C.
	LPx30_GPIO2, //HFM_PIN25, UART0_TX
	LPx30_GPIO23, //HFM_PIN26, UART0_RTS
	LPx30_GPIO1, //HFM_PIN27, UART0_RX
	LPx30_GPIO22, //HFM_PIN28, UART0_CTS, PWM0
	LPx30_GPIO8, //HFM_PIN29,
	LPx30_GPIO24, //HFM_PIN30, PWM2
	LPx30_GPIO25, //HFM_PIN31, PWM3
	HFM_NOPIN, //HFM_PIN32, N.C.
	HFM_NOPIN, //HFM_PIN33, nRESET
	HFM_NOPIN  //HFM_PIN34, GND
};

#define NR_MODULES 1
int *gpio_module_types[NR_MODULES] = {
	gpio_lpb130,
};

const int gpio_module_nrpins[NR_MODULES] = {
	34,
};

int g_module_id = HFM_TYPE_LPB130;

int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HFM_NOPIN,		//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,		//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,		//HFGPIO_F_JTAG_TDI
	HFM_NOPIN,		//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	LPx30_GPIO2,	//HFGPIO_F_UART0_TX
	LPx30_GPIO23,	//HFGPIO_F_UART0_RTS
	LPx30_GPIO1,	//HFGPIO_F_UART0_RX
	LPx30_GPIO22,	//HFGPIO_F_UART0_CTS
	
	HFM_NOPIN,  	//HFGPIO_F_SPI_MISO
	HFM_NOPIN,	  	//HFGPIO_F_SPI_CLK
	HFM_NOPIN,	  	//HFGPIO_F_SPI_CS
	HFM_NOPIN,  	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,		//HFGPIO_F_UART1_TX,
	HFM_NOPIN,		//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,		//HFGPIO_F_UART1_RX,
	HFM_NOPIN,		//HFGPIO_F_UART1_CTS,
	
	LPx30_GPIO8,	//HFGPIO_F_NLINK
	LPx30_GPIO24,	//HFGPIO_F_NREADY
	LPx30_GPIO25,	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	    //HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	    //HFGPIO_F_SLEEP_ON
	
	HFM_NOPIN,	    //HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,		/* led wifi */
	HFM_NOPIN,		/* relay */
	HFM_NOPIN,		/* push button */
	HFM_NOPIN,		/* toggle button */
	HFM_NOPIN,		/* button up */
	HFM_NOPIN,		/* button dw */
	HFM_NOPIN,		/* buzzer */
	HFM_NOPIN,		/* i2c ck */
	HFM_NOPIN,		/* i2c dt */
};

#else
#error "platform not supported"
#endif

const int *gpio = NULL;

static hftimer_handle_t debounce_timer, recovery_timer;
static volatile uint8_t key_counter = 0, recovery_counter = 0;



int *gpio_pin(int n)
{
	return &hf_gpio_fid_to_pid_map_table[n];
}


inline int USER_FUNC gpio_get_state(int fid)
{
	return hfgpio_fpin_is_high(fid) ? 1 : 0;
}



static void USER_FUNC switch_state_page(char *url, char *rsp)
{
	struct hfeasy_state *state = config_get_state();
	char val[20];
	int ret;
	int dimmer_val = 0;

	if (state->func_state & FUNC_BUZZER) {
		ret = httpd_arg_find(url, "tone", val);
		if (ret == 1) {
			buzzer_play(atoi(val));
			u_printf("playing tone %d\r\n", atoi(val));
		}
	}
	
	if (state->func_state & FUNC_DIMMER) {
		ret = httpd_arg_find(url, "sw", val);
		if (ret != 1) {
			strcpy(val, "none");
			dimmer_val = -1;
		} else {
			dimmer_val = atoi(val);
		}

		if (dimmer_val != -1)
			dimmer_set((uint8_t) dimmer_val, RELAY_SRC_HTTP);

		if (state->cfg.wifi_led & LED_CONFIG_HTTP)
			led_ctrl("n1f"); /* got data = 1 blink */

		sprintf(rsp, "{ \"set\": \"%d\", \"relay_status\": \"%d\", \"level\": \"%d\" }", dimmer_val, state->relay_state, state->dimmer_level);
	}
	
	
	if (state->func_state & FUNC_RELAY) {
		ret = httpd_arg_find(url, "sw", val);
		if (ret != 1)
			strcpy(val, "none");

		/* set relay */
		if (atoi(val) > 0)
			relay_set(1, RELAY_SRC_HTTP);
		else
			relay_set(0, RELAY_SRC_HTTP);

		if (state->cfg.wifi_led & LED_CONFIG_HTTP)
			led_ctrl("n1f"); /* got data = 1 blink */
		
		sprintf(rsp, "{ \"set\": \"%s\", \"relay_status\": \"%d\" }", val, state->relay_state);
		//u_printf("ret=%d, sw='%s' relay_state=%d\r\n", ret, val, state->relay_state);
	}
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

volatile static int irq_flag = 0;

static void USER_FUNC debounce_timer_handler(hftimer_handle_t timer)
{
	struct hfeasy_state *state = config_get_state();

	if (state->func_state & FUNC_BTN_PUSH) {
		if (gpio_get_state(GPIO_SWITCH_PUSH) == 0) {
			if (recovery_counter++ == 0) {
				hftimer_start(recovery_timer);
			}
			hftimer_start(timer);
		} else {
			hftimer_stop(recovery_timer);
			recovery_counter = 0;
			key_counter = 0;
		}
	}
	
	if (state->func_state & FUNC_BTN_TOG) {
		key_counter = 0;
		
		int sw_state = hfgpio_fpin_is_high(GPIO_SWITCH_TOGGLE) ? 1 : 0;
		if (prev_sw_state == sw_state)
			return;
			
		relay_set(RELAY_TOGGLE, RELAY_SRC_SWITCH);
		prev_sw_state = sw_state;
		
		if (recovery_counter++ == 0) {
			hftimer_start(recovery_timer);
		}
	}
}


static void USER_FUNC switch_irqhandler_toggle(uint32_t arg1, uint32_t arg2)
{
	if (key_counter == 0) {
		key_counter++;
	}
	hftimer_start(debounce_timer);
}


static void USER_FUNC switch_irqhandler_push(uint32_t arg1, uint32_t arg2)
{
	struct hfeasy_state *state = config_get_state();

	if (key_counter == 0) {
		key_counter++;
		if (state->func_state & FUNC_RELAY)
			irq_flag |= 2;
			//relay_set(RELAY_TOGGLE, RELAY_SRC_SWITCH);

		if (state->func_state & FUNC_DIMMER)
			irq_flag |= 4;
			//dimmer_set(DIMMER_TOGGLE, RELAY_SRC_SWITCH);
		
	}
	irq_flag |= 1;//hftimer_start(debounce_timer);
}


void button_handler(void)
{
	while(1) {
		if (irq_flag & 1) {
			hftimer_start(debounce_timer);
			irq_flag &= ~1;
		}
		if (irq_flag & 2) {
			relay_set(RELAY_TOGGLE, RELAY_SRC_SWITCH);
			irq_flag &= ~2;
		}
		if (irq_flag & 4) {
			dimmer_set(DIMMER_TOGGLE, RELAY_SRC_SWITCH);
			irq_flag &= ~4;
		}
		msleep(1);
	}
}

void USER_FUNC hfeasy_gpio_init(void)
{
	struct hfeasy_state *state = config_get_state();
	int module_type, i, j;

	module_type = state->cfg.gpio_config[9];
	if (module_type > NR_MODULES)
		return;

	gpio = gpio_module_types[module_type];
	if (gpio == NULL) {
		HF_Debug(DEBUG_ERROR,"error: gpio map not available for this module\n");
		return;
	}

	/* init pin mapping */
	for (i = 0; i < 9; i++) {
		if (state->cfg.gpio_config[i] > gpio_module_nrpins[module_type])
			state->cfg.gpio_config[i] = 0;
		
		hf_gpio_fid_to_pid_map_table[HFGPIO_F_USER_DEFINE + i] = gpio[state->cfg.gpio_config[i]];
		if (hf_gpio_fid_to_pid_map_table[HFGPIO_F_USER_DEFINE + i] == HFM_NOPIN)
			continue;
		for (j = 0; j < HFGPIO_F_USER_DEFINE; j++)
			if (hf_gpio_fid_to_pid_map_table[j] == hf_gpio_fid_to_pid_map_table[HFGPIO_F_USER_DEFINE + i])
				hf_gpio_fid_to_pid_map_table[j] = HFM_NOPIN;
	}

	/* init gpio */
	if (hfgpio_fmap_check(module_type) != 0) {
		HF_Debug(DEBUG_ERROR,"error: gpio map file error\n");
		return;
	}
	
	if (hf_gpio_fid_to_pid_map_table[GPIO_SWITCH_TOGGLE] != HFM_NOPIN) {
		prev_sw_state = gpio_get_state(GPIO_SWITCH_TOGGLE);
		/* on/off button */
		if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_TOGGLE,
					HFM_IO_TYPE_INPUT | HFPIO_IT_EDGE | HFPIO_PULLUP,
					switch_irqhandler_toggle, 1) != HF_SUCCESS)
			log_printf("failed to add switch interrupt\n");
		state->func_state |= FUNC_BTN_TOG;
	}

	if (hf_gpio_fid_to_pid_map_table[GPIO_SWITCH_PUSH] != HFM_NOPIN) {
		/* on/off push button */
		if (hfgpio_configure_fpin_interrupt(GPIO_SWITCH_PUSH,
					HFM_IO_TYPE_INPUT | HFPIO_IT_FALL_EDGE | HFPIO_PULLUP,
					switch_irqhandler_push, 1) != HF_SUCCESS)
			log_printf("failed to add switch interrupt\n");
		log_printf("added switch interrupt\n");
		state->func_state |= FUNC_BTN_PUSH;
	}
	
	debounce_timer = hftimer_create("debouncer", 50, false, HFTIMER_ID_DEBOUNCE, debounce_timer_handler, 0);
	recovery_timer = hftimer_create("recovery", 3000, false, HFTIMER_ID_RECOVERY, recovery_timer_handler, 0);

	if(hfthread_create((PHFTHREAD_START_ROUTINE)button_handler, "button_handler", 128, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("error starting button_handler thread\r\n");
	}

	
	httpd_add_page("/state", switch_state_page, NULL);
	httpd_add_page("/ctrl", switch_ctrl_page, NULL);
}


void USER_FUNC gpio_deinit(void)
{
	struct hfeasy_state *state = config_get_state();

	if (state->func_state & FUNC_BTN_TOG) {
		hfgpio_fdisable_interrupt(GPIO_SWITCH_TOGGLE);
		state->func_state &= ~FUNC_BTN_TOG;
	}

	if (state->func_state & FUNC_BTN_PUSH) {
		hfgpio_fdisable_interrupt(GPIO_SWITCH_PUSH);
		state->func_state &= ~FUNC_BTN_PUSH;
	}
	
	hftimer_delete(debounce_timer);
	hftimer_delete(recovery_timer);
}
