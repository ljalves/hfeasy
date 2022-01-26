 /* hfgpio.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_GPIO_H_
#define _HF_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif


#define HFGPIO_PWM_SUPPORT_RAW_FUNCTION

typedef void (*hfgpio_interrupt_func)(uint32_t,uint32_t);

#define HFM_PIN_NUMBER		(40+1)

#define HFM_TYPE_LPB130		0
#define HFM_TYPE_LPT230		1
#define HFM_TYPE_LPT130		2
#define HFM_TYPE_LPT330		3
#define HFM_TYPE_LPB135		4
#define HFM_TYPE_LPT130B		5


#define HFM_MAX_FUNC_CODE	(HFM_PIN_NUMBER*3)

enum HF_GPIO_FUNC_E
{
	//////fix/////////////////////
	HFGPIO_F_JTAG_TCK=0,
	HFGPIO_F_JTAG_TDO=1,
	HFGPIO_F_JTAG_TDI,
	HFGPIO_F_JTAG_TMS,
	HFGPIO_F_USBDP,
	HFGPIO_F_USBDM,
	HFGPIO_F_UART0_TX,
	HFGPIO_F_UART0_RTS,
	HFGPIO_F_UART0_RX,
	HFGPIO_F_UART0_CTS,
	HFGPIO_F_SPI_MISO,
	HFGPIO_F_SPI_CLK,
	HFGPIO_F_SPI_CS,
	HFGPIO_F_SPI_MOSI,
	HFGPIO_F_UART1_TX,
	HFGPIO_F_UART1_RTS,
	HFGPIO_F_UART1_RX,
	HFGPIO_F_UART1_CTS,
	////////////////////////////////
	HFGPIO_F_NLINK,
	HFGPIO_F_NREADY,
	HFGPIO_F_NRELOAD,
	HFGPIO_F_SLEEP_RQ,
	HFGPIO_F_SLEEP_ON,
	HFGPIO_F_WPS,
	HFGPIO_F_IR,
	HFGPIO_F_RESERVE2,
	HFGPIO_F_RESERVE3,
	HFGPIO_F_RESERVE4,
	HFGPIO_F_RESERVE5,
	HFGPIO_F_USER_DEFINE
};

#define F_GPI			(0x00010000)
#define F_GPO			(0x00020000)
//GND
#define F_GND			(0x00040000)
//use to Peripherals interface
#define F_PI			(0x00080000)
//vcc
#define F_VCC			(0x00100000)
#define F_NC			(0x00200000)
//use to system reset
#define F_RST			(0x00400000)
//use to interrupt input pin
#define F_IT			(0x00800000|F_GPI)

#define F_GPIO			(F_GPI|F_GPO)

#define F_PWM			(0x01000000)

#define F_ADC			(0x02000000)

#define HFM_NOPIN			(0)
#define HFM_PIN1			(F_VCC|1)
#define HFM_PIN2			(F_GND|2)
#define HFM_PIN3			(F_VCC|3)
#define HFM_PIN4			(F_VCC|4)
#define HFM_PIN5			(F_VCC|5)
#define HFM_PIN6			(F_VCC|6)
#define HFM_PIN7			(F_GPIO|F_IT|7)
#define HFM_PIN8			(F_VCC|8)
#define HFM_PIN9			(F_GPIO|F_PI|F_IT|9)
#define HFM_PIN10			(F_GPIO|F_PI|F_IT|10)
#define HFM_PIN11			(F_GPIO|F_IT|F_ADC|11)
#define HFM_PIN12			(F_GPIO|F_IT|12)
#define HFM_PIN13			(F_GPIO|F_IT|F_ADC|13)
#define HFM_PIN14			(F_GPIO|F_IT|14)
#define HFM_PIN15			(F_GPIO|F_IT|15)
#define HFM_PIN16			(F_GPIO|F_IT|16)
#define HFM_PIN17			(F_VCC|17)
#define HFM_PIN18			(F_VCC|18)
#define HFM_PIN19			(F_VCC|19)
#define HFM_PIN20			(F_PI|20)
#define HFM_PIN21			(F_GPIO|F_IT|21)
#define HFM_PIN22			(F_GPIO|F_IT|22)
#define HFM_PIN23			(F_GPIO|F_IT|F_PWM|23)
#define HFM_PIN24			(F_GPIO|F_IT|24)
#define HFM_PIN25			(F_VCC|25)
#define HFM_PIN26			(F_VCC|26)
#define HFM_PIN27			(F_PI|27)
#define HFM_PIN28			(F_PI|28)
#define HFM_PIN29			(F_GPIO|F_IT|29)
#define HFM_PIN30			(F_GPIO|F_IT|30)
#define HFM_PIN31			(F_GPIO|F_IT|F_PWM|31)
#define HFM_PIN32			(F_GPIO|F_IT|F_PWM|32)
#define HFM_PIN33			(F_GPIO|F_IT|F_PWM|33)
#define HFM_PIN34			(F_GPIO|F_IT|F_PWM|34)
#define HFM_PIN35			(F_GPIO|F_IT|35)
#define HFM_PIN36			(F_GPIO|F_IT|36)
#define HFM_PIN37			(F_VCC|37)
#define HFM_PIN38			(F_VCC|38)
#define HFM_PIN39			(F_VCC|39)
#define HFM_PIN40			(F_VCC|40)


#define LPx30_GPIO0			HFM_PIN36
#define LPx30_GPIO1			HFM_PIN21
#define LPx30_GPIO2			HFM_PIN22
#define LPx30_GPIO3			HFM_PIN23
#define LPx30_GPIO4			HFM_PIN15
#define LPx30_GPIO5			HFM_PIN14
#define LPx30_GPIO7			HFM_PIN12
#define LPx30_GPIO8			HFM_PIN13
#define LPx30_GPIO9			HFM_PIN7
#define LPx30_GPIO12		HFM_PIN30
#define LPx30_GPIO13		HFM_PIN29
#define LPx30_GPIO21		HFM_PIN35
#define LPx30_GPIO22		HFM_PIN34
#define LPx30_GPIO23		HFM_PIN33
#define LPx30_GPIO24		HFM_PIN32
#define LPx30_GPIO25		HFM_PIN31
#define LPx30_GPIO26		HFM_PIN24
#define LPx30_GPIO27		HFM_PIN16
#define LPx30_GPADC0		HFM_PIN11


#define HF_M_PINNO(_pin)	((_pin)&0xFF)

#define HFM_VALID_PINNO(_pinno)	((_pinno)>0&&(_pinno)<=HFM_PIN_NUMBER)

#define HF_M_PIN(_no)		HFM_PIN##_no

/*  Default pin configuration (no attribute). */
#define HFPIO_DEFAULT               (0u << 0)

/*  Low level interrupt is active */
#define	HFPIO_IT_LOW_LEVEL          	(1u<<4)
/*  High level interrupt is active */
#define HFPIO_IT_HIGH_LEVEL	    		(1u<<5)
/*  Falling edge interrupt is active */
#define HFPIO_IT_FALL_EDGE            	(1u<<6)
/*  Rising edge interrupt is active */
#define HFPIO_IT_RISE_EDGE             	(1u<<7)
/*Interrupt Edge detection is active.*/
#define HFPIO_IT_EDGE			    	(1u<<8)

#define HFPIO_TYPE_Pos                	27
/* PIO Type Mask */
#define HFPIO_TYPE_Msk                	(0x7u << HFPIO_TYPE_Pos)

/*   The pin is an input. */
#define   HFM_IO_TYPE_INPUT       (0x01 << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 0.*/
#define   HFM_IO_OUTPUT_0          (0x02 << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 1.*/
#define   HFM_IO_OUTPUT_1          (0x04 << HFPIO_TYPE_Pos)
#define   HFPIO_DS				  (0x8u << HFPIO_TYPE_Pos)

#define HF_GPIO_IRQ_RISE		(1)
#define HF_GPIO_IRQ_FALL		(2)


void HSF_IAPI hfgpio_init_gpio_pin(void);

int HSF_IAPI hfgpio_get_pid(int pin_no);

int HSF_IAPI hfgpio_pin_is_high(int pid);

void HSF_IAPI hfgpio_gpio_test_enable(int enable);

int HSF_IAPI hfgpio_fpin_add_feature(int fid,int flags);

int HSF_IAPI hfgpio_fpin_clear_feature(int fid,int flags);

int HSF_IAPI hfgpio_configure_pin(int pid,int flags);

void HSF_IAPI set_nLink_pin(int val);

void HSF_IAPI set_nReady_pin(int val);

#define  hfgpio_set_out_high(pid)	hfgpio_configure_pin(pid,HFPIO_DEFAULT|HFM_IO_OUTPUT_1)

#define  hfgpio_set_out_low(pid)	hfgpio_configure_pin(pid,HFPIO_DEFAULT|HFM_IO_OUTPUT_0)

void HSF_IAPI hfgpio_write_out_reg(int gpio, int value);

int HSF_IAPI hfgpio_read_out_reg(int gpio);

int HSF_IAPI hfgpio_read_in_reg(int gpio);


/**
 * @brief check the validity of hf_gpio_fid_to_pid_map_table.
 *
 * @param[in] type: the module type, as follows
 * ==========================================
 * | HFM_TYPE_LPB130                        |
 * |----------------------------------------|
 * | HFM_TYPE_LPT230                        |
 * |----------------------------------------|
 * | HFM_TYPE_LPT130                        |
 * |----------------------------------------|
 * | HFM_TYPE_LPT330                        |
 * ==========================================
 * @return[out] HF_SUCCESS-successfully, other value is have multiple functions in the same pin
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_fmap_check(int type);

/**
 * @brief set the configure parameters of pin by the function id.
 *
 * @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 *			 flags: the flag of pin, as follows, can be used together, such as: HFPIO_DEFAULT|HFM_IO_TYPE_INPUT
 * ==========================================
 * | HFPIO_DEFAULT                          |
 * |----------------------------------------|
 * | HFM_IO_TYPE_INPUT                      |
 * |----------------------------------------|
 * | HFM_IO_OUTPUT_0                        |
 * |----------------------------------------|
 * | HFM_IO_OUTPUT_1                        |
 * ==========================================
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal, HF_E_ACCES-not allowed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_configure_fpin(int fid, int flags);

/**
 * @brief get the configure parameters of pin by the function id.
 *
 * @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] the configure parameters of pin
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_fconfigure_get(int fid);

/**
 * @brief set the pin to high level by the function id.
 *
 * @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal
 * @see None.
 * @note None.
 */
#define hfgpio_fset_out_high(fid)	hfgpio_configure_fpin(fid,HFPIO_DEFAULT|HFM_IO_OUTPUT_1)

/**
 * @brief set the pin to low level by the function id.
 *
 * @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal
 * @see None.
 * @note None.
 */
#define hfgpio_fset_out_low(fid)	hfgpio_configure_fpin(fid,HFPIO_DEFAULT|HFM_IO_OUTPUT_0)

/**
 * @brief configure the interrupt of pin by the function id.
 *
* @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 *			 flags: the interrupt type, as follows
 * ==========================================
 * | HFPIO_IT_FALL_EDGE                     |
 * |----------------------------------------|
 * | HFPIO_IT_RISE_EDGE                     |
 * |----------------------------------------|
 * | HFPIO_IT_EDGE                          |
 * ==========================================
 *			 handle: a function pointer, called back when trigger
 *			 enable: enable or disable to trigger
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal, HF_E_ACCES-not allowed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_configure_fpin_interrupt(int fid,uint32_t flags,hfgpio_interrupt_func handle,int enable);

/**
 * @brief enable interrupt of pin by the function id.
 *
* @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_fenable_interrupt(int fid);

/**
 * @brief disable interrupt of pin by the function id.
 *
* @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed, HF_E_INVAL-illegal
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_fdisable_interrupt(int fid);

/**
 * @brief disable interrupt of all pins by the function id.
 *
* @param[in] fid:None
 * @return[out] HF_SUCCESS-successfully, HF_FAIL-failed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_disabel_all_interrupt(void);

/**
 * @brief check the level of the pin to be high by the function id.
 *
* @param[in] fid: the function id define by table 'hf_gpio_fid_to_pid_map_table'
 * @return[out] >=1 high level, <=0 low level or illegal
 * @see None.
 * @note effective in output mode and input mode.
 */
int HSF_API hfgpio_fpin_is_high(int fid);

/**
 * @brief check whether or not in GPIO test mode .
 *
 * @param[in] None
 * @return[out] 0-not in test mode, 1-in test mode
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_gpio_test_is_running(void);

/**
 * @brief stop PWM.
 *
 * @param[in] fid: the function id of gpio
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_pwm_disable(int fid);

/**
 * @brief start PWM.
 *
 * @param[in] fid: the function id of gpio
 *			 freq: the frequency of PWM, the range is [200-1000000], the GPIO3 only support [2000-1000000]
 *			 hrate: the duty cycle of PWM, the range is [0-100]
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_pwm_enable(int fid, int freq, int hrate);

/**
 * @brief start ADC.
 *
 * @param[in] fid: the function id of gpio
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_adc_enable(int fid);

/**
 * @brief get ADC value.
 *
 * @param[in] fid: the function id of gpio
 * @return[out] 
 			<0-failed
 			>0-the value of ADC, [0-1023] indicate [0V-2V]
 * @see None.
 * @note None.
 */
int HSF_API hfgpio_adc_get_value(int fid);


#ifdef HFGPIO_PWM_SUPPORT_RAW_FUNCTION
/**
 * @brief init PWM use ram function.
 *
 * @param[in] fid: the function id of gpio
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note please do not use both 'hfpwm_raw' function and 'hfgpio_pwm' function at the same time.
 */
int hfpwm_raw_init(int fid);

/**
 * @brief set PWM period by tick.
 *
 * @param[in] fid: the function id of gpio
 *			 period_ticks: the tick number of period, range is [10-60000], GPIO3 only support multiples of 8, range is [16-8000]
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note one tick is 100ns.
 */
int hfpwm_raw_period_ticks(int fid, int period_ticks);

/**
 * @brief set PWM pulse width by tick.
 *
 * @param[in] fid: the function id of gpio
 *			 pulsewidth_ticks: the tick number of pulse width, range is [0-60000], GPIO3 only support multiples of 8, range is [0-8000]
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note one tick is 100ns.
 */
int hfpwm_raw_pulsewidth_ticks(int fid, int pulsewidth_ticks);

/**
 * @brief set GPIO22 pulse width by tick in differential output mode, only support GPIO22&GPIO25 in same period.
 *
 * @param[in] fid: the function id of gpio
 *			 pulsewidth_ticks: the tick number of pulse width, the range is [0-60000]
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see please don't use both 'hfpwm_raw_pulsewidth_ticks' and 'hfpwm_raw_pulsewidth_ticks_diff' and 'hfpwm_raw_pulsewidth_ticks_sync' at the same time.
 * @note one tick is 100ns, differential output have a 100ns~200ns deviation.
 */
int hfpwm_raw_pulsewidth_ticks_diff(int fid, int pulsewidth_ticks);

/**
 * @brief set GPIO22&GPIO25 pulse width by tick in same period.
 *
 * @param[in] fid: the function id of gpio
 *			 gpio22_pulsewidth_ticks: first high level time of GPIO22, the range is [4-60000]
 *			 gpio25_pulsewidth_ticks: first low level time of GPIO25, the range is [4-60000]
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see please don't use both 'hfpwm_raw_pulsewidth_ticks' and 'hfpwm_raw_pulsewidth_ticks_diff' and 'hfpwm_raw_pulsewidth_ticks_sync' at the same time.
 * @note one tick is 100ns, have a 100ns~400ns deviation.
 */
int hfpwm_raw_pulsewidth_ticks_sync(int fid, int gpio22_pulsewidth_ticks, int gpio25_pulsewidth_ticks);

#endif


#ifdef __cplusplus
}
#endif

#endif

