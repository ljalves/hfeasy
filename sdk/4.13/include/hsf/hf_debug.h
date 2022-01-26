/* hf_debug.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef HF_DEBUG
#define HF_DEBUG


#define DEBUG_WIFI				DEBUG_LEVEL_LOW
#define DEBUG_LWIP				DEBUG_LEVEL_CLOSE
#define DEBUG_LWIP_ND			DEBUG_LEVEL_CLOSE
#define DEBUG_TCPIP				DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI_TEST		DEBUG_LEVEL_CLOSE
#define DEBUG_DHCPD			DEBUG_LEVEL_CLOSE
#define DEBUG_PING				DEBUG_LEVEL_CLOSE
#define DEBUG_BOOTUP			DEBUG_LEVEL_LOW
#define DEBUG_SECU				DEBUG_LEVEL_CLOSE
#define DEBUG_DNS				DEBUG_LEVEL_CLOSE
#define DEBUG_SMTLK			DEBUG_LEVEL_MID
#define DEBUG_SMTAPLK			DEBUG_LEVEL_MID
#define DEBUG_AIRKISS			DEBUG_LEVEL_MID
#define DEBUG_CONFIG			DEBUG_LEVEL_LOW
#define DEBUG_ATCMD			DEBUG_LEVEL_LOW
#define DEBUG_ASSIS				DEBUG_LEVEL_CLOSE
#define DEBUG_SOCKA			DEBUG_LEVEL_LOW
#define DEBUG_SOCKB			DEBUG_LEVEL_LOW
#define DEBUG_HTTPD			DEBUG_LEVEL_LOW
#define DEBUG_WEB_LOGIN		DEBUG_LEVEL_CLOSE
#define DEBUG_HTTPC			DEBUG_LEVEL_CLOSE
#define DEBUG_GPIO				DEBUG_LEVEL_LOW
#define DEBUG_SPI				DEBUG_LEVEL_CLOSE
#define DEBUG_WIFI_HEX			DEBUG_LEVEL_CLOSE
#define DEBUG_APSTA			DEBUG_LEVEL_CLOSE
#define DEBUG_UPGRADE			DEBUG_LEVEL_MID
#define DEBUG_WIFICOMPLETE	DEBUG_LEVEL_CLOSE	
#define DEBUG_WPS				DEBUG_LEVEL_MID
#define DEBUG_WPA				DEBUG_LEVEL_CLOSE
#define DEBUG_SCAN				DEBUG_LEVEL_LOW
#define DEBUG_FLASH			DEBUG_LEVEL_LOW
#define DEBUG_HFFILE			DEBUG_LEVEL_LOW
#define DEBUG_MSGQ				DEBUG_LEVEL_LOW
#define DEBUG_SNIFFER			DEBUG_LEVEL_LOW
#define DEBUG_NTP				DEBUG_LEVEL_LOW

#define DEBUG_LEVEL_NON		-1
#define DEBUG_LEVEL_CLOSE   	0
#define DEBUG_LEVEL_LOW		1
#define DEBUG_LEVEL_MID		2
#define DEBUG_LEVEL_HI			3
#define DEBUG_LEVEL_USER		10
#define DEBUG_LEVEL				DEBUG_LEVEL_HI

#define DEBUG_WARN				(DEBUG_LEVEL_USER-2)
#define DEBUG_ERROR			(DEBUG_LEVEL_USER-2)

#ifdef __cplusplus
extern "C" {
#endif


int HSF_IAPI hfdbg_check(void);

/**
 * @brief set debug level.
 *
 * @param[in] level: <=0 means close debug information
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfdbg_set_level(int level);

/**
 * @brief get debug level.
 *
 * @param[in] None
 * @return[out] the debug level
 * @see None.
 * @note None.
 */
int HSF_API hfdbg_get_level(void);

/**
 * @brief get debug uart port.
 *
 * @param[in] None
 * @return[out] the debug uart port
 * @see None.
 * @note None.
 */
int HSF_API hfdbg_get_uartno(void);

/**
 * @brief upload debug information to HF debug server.
 *
 * @param[in] server_port: server port
 *			 format: printf format, max support 1400 bytes
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note return HF_SUCCESS does not mean that the server has been received, is affected by the network environment.
 */
int HSF_API hfdbg_upload_debug_info(unsigned short server_port, const char *format , ... );


#define __HF_DEBUG
#ifdef __HF_DEBUG
/**
 * @brief print debug information through debug uart.
 *
 * @param[in] debug_level: the level of print information, if larger than debug level will print through debug uart
 *			 format: printf format, max support 1500 bytes
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API HF_Debug(int debug_level, const char *format, ... );
#define hfdbg_error(...)	HF_Debug(DEBUG_ERROR,"[ %d error %s %d]",hfsys_get_time(),__FUNCTION__,__LINE__); \
						HF_Debug(DEBUG_ERROR,__VA_ARGS__)
#define hfdbg_warn(...)	HF_Debug(DEBUG_WARN,"[warnning %d %s %d]",hfsys_get_time(),__FUNCTION__,__LINE__); \
						HF_Debug(DEBUG_WARN,__VA_ARGS__)
#define u_printf(...)		HF_Debug(DEBUG_LEVEL_USER,__VA_ARGS__)
#else
#define HF_Debug(a, b , ... )
#define hfdbg_error(...)
#define hfdbg_warn(...)
#define u_printf(...)
#endif


#ifdef __cplusplus
}
#endif

#endif

