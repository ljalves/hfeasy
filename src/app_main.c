/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "hfeasy.h"

#if (defined(__LPT100__)||defined(__LPT100F__)) 
#ifdef __LPT100F__
static int module_type = HFM_TYPE_LPT100F;// macro definition module type
#else
static int module_type = HFM_TYPE_LPT100;
#endif
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
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
	
	//HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
	HF_M_PIN(12),	//	HFGPIO_F_BUZZER
	HF_M_PIN(11),	//	HFGPIO_F_SWITCH
	HF_M_PIN(18),	//	HFGPIO_F_EXTRA_SWITCH

};
#elif defined(__LPB105__)
static int module_type= HFM_TYPE_LPB105;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HF_M_PIN(3),	//HFGPIO_F_JTAG_TDO
	HF_M_PIN(4),	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HFM_NOPIN,	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HFM_NOPIN,	//HFGPIO_F_UART0_CTS
	
	HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(11),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
	
	HF_M_PIN(18),	//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPT200__)
static int module_type= HFM_TYPE_LPT200;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
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
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(7),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPB100__)// lpb100 pin definition, refer to the user manual
static int module_type= HFM_TYPE_LPB100;
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
	HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HFM_NOPIN,	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HFM_NOPIN,	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HF_M_PIN(8),	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HFM_NOPIN,	//HFGPIO_F_USER_DEFINE
};
#elif defined(__LPB100U__)
static int module_type= HFM_TYPE_LPB100;
const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
	HF_M_PIN(2),	//HFGPIO_F_JTAG_TCK
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDO
	HFM_NOPIN,	//HFGPIO_F_JTAG_TDI
	HF_M_PIN(5),	//HFGPIO_F_JTAG_TMS
	HFM_NOPIN,		//HFGPIO_F_USBDP
	HFM_NOPIN,		//HFGPIO_F_USBDM
	HF_M_PIN(39),	//HFGPIO_F_UART0_TX
	HFM_NOPIN,//HF_M_PIN(40),	//HFGPIO_F_UART0_RTS
	HF_M_PIN(41),	//HFGPIO_F_UART0_RX
	HFM_NOPIN,//HF_M_PIN(42),	//HFGPIO_F_UART0_CTS
	
	HFM_NOPIN,//HF_M_PIN(27),	//HFGPIO_F_SPI_MISO
	HFM_NOPIN,//HF_M_PIN(28),	//HFGPIO_F_SPI_CLK
	HFM_NOPIN,//HF_M_PIN(29),	//HFGPIO_F_SPI_CS
	HFM_NOPIN,//HF_M_PIN(30),	//HFGPIO_F_SPI_MOSI
	
	HF_M_PIN(29),	//HFGPIO_F_UART1_TX,
	HFM_NOPIN,	//HFGPIO_F_UART1_RTS,
	HF_M_PIN(30),	//HFGPIO_F_UART1_RX,
	HFM_NOPIN,	//HFGPIO_F_UART1_CTS,	
	
	HF_M_PIN(43),	//HFGPIO_F_NLINK
	HF_M_PIN(44),	//HFGPIO_F_NREADY
	HF_M_PIN(45),	//HFGPIO_F_NRELOAD
	HFM_NOPIN,//HF_M_PIN(7),	//HFGPIO_F_SLEEP_RQ
	HFM_NOPIN,//HF_M_PIN(8),	//HFGPIO_F_SLEEP_ON
		
	HF_M_PIN(15),		//HFGPIO_F_WPS
	HFM_NOPIN,		//HFGPIO_F_RESERVE1
	HFM_NOPIN,		//HFGPIO_F_RESERVE2
	HFM_NOPIN,		//HFGPIO_F_RESERVE3
	HFM_NOPIN,		//HFGPIO_F_RESERVE4
	HFM_NOPIN,		//HFGPIO_F_RESERVE5
	
	HF_M_PIN(7),	//HFGPIO_F_USER_DEFINE
	HF_M_PIN(8),	//HFGPIO_F_USER_DEFINE+1	
};
#else
#error "invalid project !you must define module type(__LPB100__,__LPT100__,_LPT200__,__LPB105__,__LPT100F__,__LPB100U__)"
#endif

const hfat_cmd_t user_define_at_cmds_table[] = {
	{NULL,NULL,NULL,NULL}
};


static void show_reset_reason(void)
{
	uint32_t reset_reason = hfsys_get_reset_reason();
	
#if 1
	u_printf("reset_reasion:%08x\n", reset_reason);
#else	
	if(reset_reason&HFSYS_RESET_REASON_ERESET) // hardware restart
		u_printf("ERESET\n");
	if(reset_reason&HFSYS_RESET_REASON_IRESET0) // soft restart softreset
		u_printf("IRESET0\n");
	if(reset_reason&HFSYS_RESET_REASON_IRESET1) // call hfsys_reset
		u_printf("IRESET1\n");
	if(reset_reason==HFSYS_RESET_REASON_NORMAL) // power off restart
		u_printf("RESET NORMAL\n");
	if(reset_reason&HFSYS_RESET_REASON_WPS) // WPS restart
		u_printf("RESET FOR WPS\n");
	if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_START) // turn SmartLink
		u_printf("RESET FOR SMARTLINK START\n");
	if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_OK) // smartlink success
		u_printf("RESET FOR SMARTLINK OK\n");
	if(reset_reason&HFSYS_RESET_REASON_WPS_OK) // wps success
		u_printf("RESET FOR WPS OK\n");
#endif
	
	return;
}


void app_init(void)
{
	u_printf("app_init\r\n");
}


int USER_FUNC app_main(void)
{
	u_printf("sdk version(%s), app_main start time is %s %s\n",
						hfsys_get_sdk_version(), __DATE__, __TIME__);

	if (hfgpio_fmap_check(module_type) != 0) {
		HF_Debug(DEBUG_ERROR,"error: gpio map file error\n");
		while(1)
			msleep(1000);
	}
	
	show_reset_reason();
	
	while(!hfnet_wifi_is_active()) { // waiting for wifi startup, mainly RF
		msleep(100);
	}
	
	if (hfnet_start_assis(ASSIS_PORT) != HF_SUCCESS) {
		HF_Debug(DEBUG_WARN,"start assis fail\n");
	}
	
	if (hfnet_start_uart(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)NULL) != HF_SUCCESS) {
		HF_Debug(DEBUG_WARN,"start uart fail!\n");
	}

	if (hfnet_start_httpd(HFTHREAD_PRIORITIES_MID) != HF_SUCCESS) {
		HF_Debug(DEBUG_WARN,"start httpd fail\n");
	}
	
	httpd_init();
	gpio_init();
		
	return 1;
}

