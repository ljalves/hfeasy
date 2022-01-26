 /* hfnet.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_NET_H_
#define _HF_NET_H_


#include "hsf.h"
#include "lwip/sockets.h"

#define ASSIS_PORT 	48899
#define HTTP_API_BUF_SIZE	1400

typedef struct _hfnet_stat
{
	int socketa_send_bytes;
	int socketa_recv_bytes;
	int socketa_connect_times;
	int uart_send_bytes;
	int uart_recv_bytes;
	int socketb_send_bytes;
	int socketb_connect_times;
	int socketb_recv_bytes;
	int uart_buffer_full_times;
	int uart_irq_num;
	int uart_thread_run;
	int socketa_buffer_full_times;
	
}hfnet_stat;

extern hfnet_stat g_hfnet_stat;

typedef enum
{
	HFNET_SOCKETA_DATA_READY=0,
	HFNET_SOCKETA_CONNECTED=1,
	HFNET_SOCKETA_DISCONNECTED,
	HFNET_SOCKETB_DATA_READY,
	HFNET_SOCKETB_CONNECTED,
	HFNET_SOCKETB_DISCONNECTED,
	HFNET_UART0_DATA_READY,
	HFNET_UART1_DATA_READY,
	HFNET_ASSIS_DATA_READY
}hfnet_event_id_t;

typedef struct _hfnet_socketa_client
{
	int fd;
	struct sockaddr_in addr;
}hfnet_socketa_client_t,*phfnet_socketa_client_t;

typedef int (*hfnet_callback_t)( uint32_t event,void *data,uint32_t len,uint32_t buf_len);

typedef int (*hfhttpd_nvset_callback_t)( char * cfg_name,int name_len,char* value,int val_len);

typedef int (*hfhttpd_nvget_callback_t)( char *cfg_name,int name_len,char *value,int val_len);

typedef int (*hfhttpd_url_callback_t)(char *url, char *rsp);

typedef int(*hfhttpd_process_cmd_func_t)(const char *name,void *ctx);


#ifdef __cplusplus
extern "C" {
#endif


int ping(char* ip_address);

#define hfnet_ping(_ipadr)		ping(_ipadr)

int HSF_IAPI hfnet_init(void);

int HSF_IAPI hfnet_send_assis_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int  HSF_IAPI hfnet_httpd_set_nvram(char *name,int nmlen,char *value,int vlen);

int  HSF_IAPI hfnet_httpd_get_nvram(char *name,int nmlen,char *value,int vlen);

void HSF_IAPI hfnet_register_socketa_atcmd(void);

void HSF_IAPI hfnet_register_socketb_atcmd(void);

int HSF_IAPI hfnet_send_socketa_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_IAPI hfnet_send_socketb_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_IAPI hfnet_send_uart_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_IAPI netbuf_rcvb_data(char *buf, int left);

int HSF_IAPI netbuf_recv_data(char *buf, int left);


/**
 * @brief start a UDP socket for LAN scaning and handle AT cmd.
 *
 * @param[in] port: the UDP port, default is 48899, it's better not to modify it
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_assis(uint16_t port);

/**
 * @brief start a UDP socket for LAN scaning and handle AT cmd, can set callback function, called back when recv data.
 *
 * @param[in] port: the UDP port, default is 48899, it's better not to modify it
 *			 p_callback: a function pointer, called back when recv data
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_assis_ex(uint16_t port,hfnet_callback_t p_callback);

/**
 * @brief send data through assis UDP socket.
 *
 * @param[in] rep: data you want send
 *			 len: the size of rsp
 *			 addr:  destination ip address
 *			 port:  destination port
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_assis_write(char *rep,int len,uint32_t addr,uint16_t port);

/**
 * @brief start UART0(set by AT+UART), and register callback function which will be called when systen happen the follow cases.
 * ==========================================
 * | HFNET_UART0_DATA_READY               |
 * ==========================================
 *
 * @param[in] uxpriority: priority, default is 'HFTHREAD_PRIORITIES_LOW',  it's better not to modify it
 *			 p_callback: a function pointer, called back when recv data from UART0
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API  hfnet_start_uart(uint32_t uxpriority,hfnet_callback_t p_uart_callback);

/**
 * @brief start UART0(set by AT+UART), and register callback function which will be called when systen happen the follow cases.
 * ==========================================
 * | HFNET_UART0_DATA_READY               |
 * ==========================================
 *
 * @param[in] uxpriority: priority, default is 'HFTHREAD_PRIORITIES_LOW',  it's better not to modify it
 *			 p_callback: a function pointer, called back when recv data from UART0
 *			 stack_size: the size of the stack, real size is [stack_size*4], default is 2048 bytes
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_uart_ex(uint32_t uxpriority,hfnet_callback_t p_callback, int stack_size);

/**
 * @brief start SOCKETA(set by AT+NETP), and register callback function which will be called when systen happen the follow cases.
 * ==========================================
 * | HFNET_SOCKETA_DATA_READY               |
 * |----------------------------------------|
 * | HFNET_SOCKETA_CONNECTED                |
 * |----------------------------------------|
 * | HFNET_SOCKETA_DISCONNECTED             |
 * ==========================================
 *
 * @param[in] uxpriority: priority, default is 'HFTHREAD_PRIORITIES_LOW',  it's better not to modify it
 *			 p_callback: a function pointer, called back when recv data
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_socketa(uint32_t uxpriority,hfnet_callback_t p_callback);

/**
 * @brief start SOCKETB(set by AT+SOCKB), and register callback function which will be called when systen happen the follow cases.
 * ==========================================
 * | HFNET_SOCKETB_DATA_READY               |
 * |----------------------------------------|
 * | HFNET_SOCKETB_CONNECTED                |
 * |----------------------------------------|
 * | HFNET_SOCKETB_DISCONNECTED             |
 * ==========================================
 *
 * @param[in] uxpriority: priority, default is 'HFTHREAD_PRIORITIES_LOW',  it's better not to modify it
 *			 p_callback: a function pointer, called back when recv data
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_socketb(uint32_t uxpriority,hfnet_callback_t p_uart_callback);

/**
 * @brief start web server (port is 80).
 *
 * @param[in] uxpriority: priority, default is 'HFTHREAD_PRIORITIES_LOW',  it's better not to modify it
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_start_httpd(uint32_t uxpriority);

/**
 * @brief check wifi driver is init success.
 *
 * @param[in] None
 * @return[out] 1-init successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_wifi_is_active(void);

/**
 * @brief get the socket handle of SOCKETA.
 *
 * @param[in] None
 * @return[out] the socket handle, if <0 is invalid
 * @see None.
 * @note None.
 */
int HSF_API hfnet_socketa_fd(void);

/**
 * @brief send data to SOCKETA.
 *
 * @param[in] data: the data buffer you want send
 *			 len: the size of send buffer
 *			 timeouts: send timeout, in milliseconds
 * @return[out] the size send out by SOCKETA
 * @see None.
 * @note None.
 */
int  HSF_API hfnet_socketa_send(char *data,uint32_t len,uint32_t timeouts);

/**
 * @brief get client socket info when SOCKETA in TCP Server mode.
 *
 * @param[in] cid: the index of client, 0-4
 *			 p_client: a pointer for using to store client info
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_socketa_get_client(int cid, phfnet_socketa_client_t p_client);

/**
 * @brief get the socket handle of SOCKETB.
 *
 * @param[in] None
 * @return[out] the socket handle, if <0 is invalid
 * @see None.
 * @note None.
 */
int HSF_API hfnet_socketb_fd(void);

/**
 * @brief send data to SOCKETB.
 *
 * @param[in] data: the data buffer you want send
 *			 len: the size of send buffer
 *			 timeouts: send timeout, in milliseconds
 * @return[out] the size send out by SOCKETB
 * @see None.
 * @note None.
 */
int HSF_API hfnet_socketb_send(char *data,uint32_t len,uint32_t timeouts);

/**
 * @brief enable support multicast, default is support.
 *
 * @param[in] enable: 1-enable, 0-disable
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_enable_multicast(int enable);

/**
 * @brief get the moudle's mac address in Station mode in string format.
 *
 * @param[in] mac_str: a poniter to mac, such as: "ACCF23FF8888"
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_IAPI hfnet_get_mac_address(char *mac_str);

/**
 * @brief start airlink Lan scaning .
 *
 * @param[in] device_type: device type provided by WeChat, such as: "gh_ba0c25259dca"
  *			 device_id: always is mac address, such as: "ACCF23FF8888"
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note this function will use a UDP and a thread with 1024 bytes stack depth.
 */
int HSF_API hfnet_start_airlink(char *device_type, char *device_id);

/**
 * @brief stop airlink Lan scaning and release resources.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfnet_stop_airlink(void);

/**
 * @brief set httpd callback function, called back when when set/get parameter.
 *
 * @param[in] p_set: a function pointer, called back when set parameter by webpage, if don't use please set NULL
 *			 p_get: a function pointer, called back when get parameter by webpage, if don't use please set NULL
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfnet_httpd_set_get_nvram_callback(hfhttpd_nvset_callback_t p_set,hfhttpd_nvget_callback_t p_get);

/**
 * @brief get the module's hostname (maximum is 20 bytes), the same as MID.
 *
 * @param[in] hostname: buffer for using to store hostname, the buffer size must >21
 * @return[out] a pointer to the start address of hostname
 * @see None.
 * @note None.
 */
char *HSF_API hfnet_get_hostname(char *hostname);

/**
 * @brief get module's WAN ip address offered by DHCP Server or set by AT+ALNN.
 *
 * @param[in] ip: a pointer for using to store ip address, if don't want store please set NULL
 *			 mask: a pointer for using to store mask, if don't want store please set NULL
  *			 gw: a pointer for using to store gatwway address, if don't want store please set NULL
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_get_dhcp_ip (int * ip, int *mask, int *gw);

/**
 * @brief get module's LAN ip address (set by AT+LANN).
 *
 * @param[in] ip: a pointer for using to store ip, if don't want store please set NULL
 *			 mask: a pointer for using to store mask, if don't want store please set NULL
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_get_lan_ip (int * ip, int *mask);

/**
 * @brief get ip address by domain name.
 *
 * @param[in] name: domain name
 *			 addr: a pointer for using to store addr
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_gethostbyname(const char *name, ip_addr_t *addr);

/**
 * @brief check string is ip address.
 *
 * @param[in] ipaddr: ip string
 * @return[out] 1-ip address, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfnet_is_ipaddress(const char * ipaddr);


#ifdef __cplusplus
}
#endif

#endif

