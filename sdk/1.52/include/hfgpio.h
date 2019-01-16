
/* hfgpio.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */

#ifndef _HF_GPIO_H_H_H_
#define _HF_GPIO_H_H_H_

typedef void (*hfgpio_interrupt_func)(uint32_t,uint32_t);
//gpio function define

#define HFM_TYPE_LPB100	0
#define HFM_TYPE_LPT100	1
#define HFM_TYPE_LPT200	2
#define HFM_TYPE_LPB105	3
#define HFM_TYPE_LPT100F	4


#define HFM_PIN_NUMBER		(48)

#define HFM_MAX_FUNC_CODE	(HFM_PIN_NUMBER*2)

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
#define HFM_PIN1			(F_GND|1)
#define HFM_PIN2			(F_PI|2)
#define HFM_PIN3			(F_PI|3)
#define HFM_PIN4			(F_PI|4)
#define HFM_PIN5			(F_PI|5)
#define HFM_PIN6			(F_NC|6)
#define HFM_PIN7			(F_GPIO|F_IT|F_ADC|7)
#ifdef __LPB100U__
#define HFM_PIN8			(F_GPIO|F_IT|F_PWM|F_ADC|8)
#else
#define HFM_PIN8			(F_GPIO|F_IT|F_ADC|8)
#endif
#define HFM_PIN9			(F_VCC|9)
#define HFM_PIN10			(F_NC|10)
#ifdef __LPB100U__
#define HFM_PIN11			(F_NC|11)
#define HFM_PIN12			(F_NC|12)
#define HFM_PIN13			(F_NC|13)
#else
#define HFM_PIN11			(F_GPIO|F_IT|F_PWM|F_ADC|11)
#define HFM_PIN12			(F_GPIO|F_IT|F_PWM|F_ADC|12)
#define HFM_PIN13			(F_GPIO|F_IT|13)
#endif
#define HFM_PIN14			(F_NC|14)
#define HFM_PIN15			(F_GPIO|F_IT|F_PI|15)
#define HFM_PIN16			(F_NC|16)
#define HFM_PIN17			(F_GND|17)
#define HFM_PIN18			(F_GPIO|F_IT|F_PWM|18)
#define HFM_PIN19			(F_NC|19)
#define HFM_PIN20			(F_GPIO|F_IT|F_PWM|20)
#define HFM_PIN21			(F_NC|21)
#define HFM_PIN22			(F_NC|22)
#ifdef __LPB100U__
#define HFM_PIN23			(F_NC|23)
#define HFM_PIN24			(F_NC|24)
#define HFM_PIN25			(F_GPIO|F_IT|F_PI|25)
#define HFM_PIN26			(F_GPIO|F_IT|F_PI|26)
#define HFM_PIN27			(F_GPIO|F_IT|F_PI|F_PWM|F_ADC|27)
#define HFM_PIN28			(F_NC|28)
#define HFM_PIN29			(F_GPIO|F_IT|F_PI|F_PWM|F_ADC|29)
#define HFM_PIN30			(F_GPIO|F_IT|F_PI|F_PWM|F_ADC|30)
#else
#define HFM_PIN23			(F_GPIO|F_IT|F_PWM|F_ADC|23)
#define HFM_PIN24			(F_NC|24)
#define HFM_PIN25			(F_NC|25)
#define HFM_PIN26			(F_NC|26)
#define HFM_PIN27			(F_PI|F_GPIO|F_IT|F_ADC|27)
#define HFM_PIN28			(F_PI|F_GPIO|F_IT|28)
#define HFM_PIN29			(F_PI|F_GPIO|F_IT|F_ADC|29)
#define HFM_PIN30			(F_GPIO|F_IT|F_PI|30)
#endif
#define HFM_PIN31			(F_VCC|31)
#define HFM_PIN32			(F_GND|32)
#define HFM_PIN33			(F_NC|33)
#define HFM_PIN34			(F_VCC|34)
#ifdef __LPB100U__
#define HFM_PIN35			(F_GPIO|F_IT|35)
#else
#define HFM_PIN35			(F_NC|35)
#endif
#define HFM_PIN36			(F_NC|36)
#define HFM_PIN37			(F_NC|37)
#define HFM_PIN38			(F_NC|38)
#define HFM_PIN39			(F_PI|F_GPIO|F_IT|39)
#define HFM_PIN40			(F_PI|F_GPIO|F_IT|40)
#define HFM_PIN41			(F_PI|F_GPIO|F_IT|41)
#ifdef __LPB100U__
#define HFM_PIN42			(F_GPIO|F_IT|F_PI|F_PWM|42)
#define HFM_PIN43			(F_GPIO|F_IT|F_PWM|F_ADC|43)
#define HFM_PIN44			(F_GPIO|F_IT|F_PWM|F_ADC|44)
#define HFM_PIN45			(F_GPIO|F_IT|F_PWM|45)
#else
#define HFM_PIN42			(F_PI|F_GPIO|F_IT|42)
#define HFM_PIN43			(F_GPIO|F_IT|43)
#define HFM_PIN44			(F_GPIO|F_IT|44)
#define HFM_PIN45			(F_GPIO|F_IT|45)
#endif
#define HFM_PIN46			(F_NC|46)
#define HFM_PIN47			(F_RST|47)
#define HFM_PIN48			(F_GND|48)

#define HF_M_PINNO(_pin)	((_pin)&0xFF)

#define HFM_VALID_PINNO(_pinno)	((_pinno)>0&&(_pinno)<=HFM_PIN_NUMBER)

#define HF_M_PIN(_no)		HFM_PIN##_no

#define HFM_PIN_NUM		(48)

/*  Default pin configuration (no attribute). */
#define HFPIO_DEFAULT               (0u << 0)
/*  The internal pin pull-up is active. */
#define HFPIO_PULLUP                  (1u << 0)

#ifdef CONFIG_CHIP_SAM4SXX
/*  The internal glitch filter is active. */
#define HFPIO_DEGLITCH               (1u << 1)
/*  The pin is open-drain. */
#define HFPIO_OPENDRAIN            (1u << 2)

/*  The internal debouncing filter is active. */
#define HFPIO_DEBOUNCE                (1u << 3)

/*  Enable additional interrupt modes. */
#define HFPIO_IT_AIME                 (1u << 4)

/*  Interrupt High Level/Rising Edge detection is active. */
#define HFPIO_IT_RE_OR_HL             (1u << 5)
/*  Interrupt Edge detection is active. */
#define HFPIO_IT_EDGE                     (1u << 6)

/*  Low level interrupt is active */
#define HFPIO_IT_LOW_LEVEL            (0               | 0 | HFPIO_IT_AIME)
/*  High level interrupt is active */
#define HFPIO_IT_HIGH_LEVEL           (HFPIO_IT_RE_OR_HL | 0 | HFPIO_IT_AIME)
/*  Falling edge interrupt is active */
#define HFPIO_IT_FALL_EDGE            (0               | HFPIO_IT_EDGE | HFPIO_IT_AIME)
/*  Rising edge interrupt is active */
#define HFPIO_IT_RISE_EDGE            (HFPIO_IT_RE_OR_HL | HFPIO_IT_EDGE | HFPIO_IT_AIME)

#define HFPIO_TYPE_Pos                27
/* PIO Type Mask */
#define HFPIO_TYPE_Msk                (0xFu << HFPIO_TYPE_Pos)

/*   The pin is an input. */
#define   HFM_IO_TYPE_INPUT       (0x5u << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 0.*/
#define   HFM_IO_OUTPUT_0         (0x6u << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 1.*/
#define   HFM_IO_OUTPUT_1         (0x7u << HFPIO_TYPE_Pos)

#define   HFPIO_DS				  (0x8u << HFPIO_TYPE_Pos)
#elif defined(CONFIG_CHIP_HFO18)

#define HFPIO_PULLDOWN                  (1u << 1)

/*  Low level interrupt is active */
#define	HFPIO_IT_LOW_LEVEL          (1u<<4)
/*  High level interrupt is active */
#define HFPIO_IT_HIGH_LEVEL	    (1u<<5)
/*  Falling edge interrupt is active */
#define HFPIO_IT_FALL_EDGE            (1u<<6)
/*  Rising edge interrupt is active */
#define HFPIO_IT_RISE_EDGE             (1u<<7)
/*Interrupt Edge detection is active.*/
#define HFPIO_IT_EDGE			    (1u<<8)

#define HFPIO_TYPE_Pos                27
/* PIO Type Mask */
#define HFPIO_TYPE_Msk                (0x7u << HFPIO_TYPE_Pos)

/*   The pin is an input. */
#define   HFM_IO_TYPE_INPUT       (0x01 << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 0.*/
#define   HFM_IO_OUTPUT_0          (0x02 << HFPIO_TYPE_Pos)
/*   The pin is an output and has a default level of 1.*/
#define   HFM_IO_OUTPUT_1          (0x04 << HFPIO_TYPE_Pos)
#define   HFPIO_DS				  (0x8u << HFPIO_TYPE_Pos)

#endif

//内部使用函数
#if defined(CONFIG_CHIP_HFO18)&&defined(__BUILD_HSF_SDK__)

void HSF_IAPI SetGpioOutput(int bank, int pin);
void HSF_IAPI SetGpioInput(int bank, int pin);
void HSF_IAPI SetGpioNoPull(int bank, int pin);
void HSF_IAPI SetGpioPullUp(int bank, int pin);
void HSF_IAPI SetGpioPullDown(int bank, int pin);
void HSF_IAPI Set_Gpio_High(int bank, int pin);
void HSF_IAPI Set_Gpio_Low(int bank, int pin);
void HSF_IAPI SetGpioNoInputOutput(int bank, int pin);
void HSF_IAPI Set_Gpio_DS(int bank, int pin, int enable);


#endif

/*
 *根据功能码设置PIN
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: fid非法,或者它对应的PIN脚非法
 *@HF_FAIL:设置失败
 *@HF_E_ACCES:要配置的PIN不能配置成直到的flags，例如GND,VCC PIN脚,
			  只能做输入的脚不能配置成输出等.
*/
int HSF_API hfgpio_configure_fpin(int fid,int flags);

int HSF_API hfgpio_fconfigure_get(int fid);

int HSF_API hfgpio_fpin_add_feature(int fid,int flags);

int HSF_API hfgpio_fpin_clear_feature(int fid,int flags);

/*
 *根据功能码设置对应PIN为输出高电平
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: fid非法,或者它对应的PIN脚非法
 *@HF_FAIL:设置失败
*/
//int hfgpio_fset_out_high(int fid);
#define hfgpio_fset_out_high(__fid)	hfgpio_configure_fpin(__fid,HFPIO_DEFAULT|HFM_IO_OUTPUT_1)
/*
 *根据功能码设置对应PIN为输出低电平
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: fid非法,或者它对应的PIN脚非法
 *@HF_FAIL:设置失败
*/

//int hfgpio_fset_out_low(int fid);
#define hfgpio_fset_out_low(__fid)	hfgpio_configure_fpin(__fid,HFPIO_DEFAULT|HFM_IO_OUTPUT_0)


/*
 *根据PIN脚ID设置PIN(内部使用，外部请使用hfgpio_configure_fpin)
 *flags:
		HFPIO_IT_LOW_LEVEL:低电平触发
		HFPIO_IT_HIGH_LEVEL:高电平触发
		HFPIO_IT_FALL_EDGE:下降沿触发
		HFPIO_IT_RISE_EDGE:上升沿触发
		HFPIO_IT_EDGE:边沿触发
 *handle:
		中断触发的时候调用，函数里面不能用延时，处理时间近可能短 		
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: pid非法
 *@HF_FAIL:设置失败
 *@HF_E_ACCES:要配置的PIN不能配置成中断，PIN脚不具备F_IT属性,
*/
int hfgpio_configure_fpin_interrupt(int fid,uint32_t flags,hfgpio_interrupt_func handle,int enable);

/*
 *使能中断,在使能中断之前一定要先调用hfgpio_configure_fpin_interrupt
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: fid非法,或者它对应的PIN脚非法
 *@HF_FAIL:设置失败
*/
int hfgpio_fenable_interrupt(int fid);

/*
 *禁止中断
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: fid非法,或者它对应的PIN脚非法
 *@HF_FAIL:设置失败
*/
int hfgpio_fdisable_interrupt(int fid);

/*
 *禁止所有的GPIO 中断
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_FAIL:设置失败
*/
int HSF_API hfgpio_disabel_all_interrupt(void);

/*
 *功能码对应PIN是否为高电平
 *return value:
 *@>=1:高电平
 *@<=0:低电平,或者fid非法，fid对应的pin脚没有定义或者不是gpio
*/
int hfgpio_fpin_is_high(int fid);

/*
 *检查功能码到PIN脚映射表合法行，HFGPIO_F_JTAG_TCK~HFGPIO_F_UART1_CTS，只能配置固定
 *的PIN脚，或者HFM_NOPIN,一个PIN脚只能对应一个功能码。
 *return value:
 *@1:固定功能码映射的PIN脚错误
 *@2:表中存在两个功能码对应同一个PIN脚
 *@0:合法
*/
int hfgpio_fmap_check(int type);

#ifdef __BUILD_HSF_SDK__

/*
 *根据PIN脚ID设置PIN(内部使用，外部请使用hfgpio_configure_fpin)
 *return value:
 *@HF_SUCCESS:设置成功
 *@HF_E_INVAL: pid非法
 *@HF_FAIL:设置失败
 *@HF_E_ACCES:要配置的PIN不能配置成直到的flags，例如GND,VCC PIN脚,
			  ,只能做输入的脚不能配置成输出等.
*/
int HSF_IAPI hfgpio_configure_pin(int pid,int flags);

#define  hfgpio_set_out_high(pid)	hfgpio_configure_pin(pid,HFPIO_DEFAULT|HFM_IO_OUTPUT_1)

#define  hfgpio_set_out_low(pid)	hfgpio_configure_pin(pid,HFPIO_DEFAULT|HFM_IO_OUTPUT_0)

int HSF_IAPI hfgpio_get_pid(int pin_no);

void HSF_IAPI hfgpio_init_gpio_pin(void);

int HSF_IAPI hfgpio_pin_is_high(int pid);

#endif


//PWM
int HSF_API  hfgpio_pwm_disable(int fid);

int HSF_API hfgpio_pwm_enable(int fid, int freq, int hrate);

//ADC
int HSF_API hfgpio_adc_enable(int fid);

int  HSF_API  hfgpio_adc_get_value(int fid);



#endif

