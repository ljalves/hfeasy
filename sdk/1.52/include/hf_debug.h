
/* hf_debug.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */

#ifndef HF_DEBUG
#define HF_DEBUG

//#include "status_codes.h"
//extern status_code_t usart_serial_write_packet(usart_if usart, const uint8_t *data,size_t len);

#define DEBUG_LWIP			DEBUG_LEVEL_CLOSE
#define DEBUG_LWIP_ND		DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI			DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI_ND		DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI_TEST		DEBUG_LEVEL_CLOSE
#define DEBUG_APP			DEBUG_LEVEL_CLOSE
#define DEBUG_DHCPD			DEBUG_LEVEL_CLOSE
#define DEBUG_HTTPD			DEBUG_LEVEL_CLOSE
#define DEBUG_HTTPD_ND		DEBUG_LEVEL_CLOSE
#define DEBUG_TESTTH		DEBUG_LEVEL_CLOSE
#define DEBUG_ASSIS			DEBUG_LEVEL_CLOSE
#define DEBUG_PING			DEBUG_LEVEL_CLOSE
#define DEBUG_WEB_LOGIN		DEBUG_LEVEL_CLOSE
#define DEBUG_BOOTUP		DEBUG_LEVEL_CLOSE
#define DEBUG_SECU			DEBUG_LEVEL_CLOSE
#define DEBUG_DNS			DEBUG_LEVEL_CLOSE
#define DEBUG_HTTPC			DEBUG_LEVEL_CLOSE
#define DEBUG_SPI			DEBUG_LEVEL_CLOSE
#define DEBUG_SOCKB			DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI_HEX		DEBUG_LEVEL_CLOSE
#define DEBUG_APSTA			DEBUG_LEVEL_LOW
#define DEBUG_CXMODE		DEBUG_LEVEL_CLOSE
#define DEBUG_UPGRADE		DEBUG_LEVEL_MID
#define DEBUG_WIFICOMPLETE	DEBUG_LEVEL_CLOSE	
#define DEBUG_WPS				DEBUG_LEVEL_MID
#define DEBUG_WPA				DEBUG_LEVEL_MID
#define DEBUG_SCAN				DEBUG_LEVEL_LOW
#define DEBUG_SMTLK				6
#define DEBUG_SMTLK_MID			5
#define DEBUG_SMTLK_LOW			4
#define DEBUG_TCPIP					10
#define DEBUG_SSL				DEBUG_LEVEL_MID
#define DEBUG_WEBSOCKETS	DEBUG_LEVEL_MID

#define DEBUG_LEVEL_NON		-1
#define DEBUG_LEVEL_CLOSE   	0
#define DEBUG_LEVEL_LOW		1
#define DEBUG_LEVEL_MID		2
#define DEBUG_LEVEL_HI			3
#define DEBUG_LEVEL_USER		10
#define DEBUG_LEVEL			DEBUG_LEVEL_HI

#define DEBUG_WARN			(DEBUG_LEVEL_USER-2)
#define DEBUG_ERROR			(DEBUG_LEVEL_USER-2)

//#define u_printf(net)
#define __HF_DEBUG
#ifdef __HF_DEBUG
void HF_Debug(int debug_level,const char *format , ... );
#define hfdbg_error(...)	HF_Debug(DEBUG_ERROR,"[ %d error %s %d]",hfsys_get_time(),__FUNCTION__,__LINE__); \
						HF_Debug(DEBUG_ERROR,__VA_ARGS__)
#define hfdbg_warn(...)	HF_Debug(DEBUG_WARN,"[warnning %d %s %d]",hfsys_get_time(),__FUNCTION__,__LINE__); \
						HF_Debug(DEBUG_WARN,__VA_ARGS__)
						
void hfdbg_set_level(int level);
int hfdbg_get_level(void);
int hfdbg_check(void);
#else
#define hfdbg_set_level(level)
#define hfdbg_get_level()	DEBUG_LEVEL_CLOSE
#define HF_Debug(a, b , ... )
#define hfdbg_check()	(0)
#define hfdbg_error(...)
#define hfdbg_warn(...)

#endif

#ifdef __BUILD_HSF_SDK__
#define u_printf(...) HF_Debug(DEBUG_LEVEL,__VA_ARGS__)
#else
#define u_printf(...) HF_Debug(DEBUG_LEVEL_USER,__VA_ARGS__)
#endif


#endif
