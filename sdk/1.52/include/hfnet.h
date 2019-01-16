

/* hfnet.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */

#ifndef _HF_NET_H_H_H_
#define _HF_NET_H_H_H_

#include "hsf.h"
#include "lwip/sockets.h"
#include "httpc/httpc.h"

#define ASSIS_PORT 	48899
#define HTTP_API_BUF_SIZE	1400

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

void HSF_API hfnet_httpd_set_get_nvram_callback(hfhttpd_nvset_callback_t p_set,hfhttpd_nvget_callback_t p_get);

int HSF_API hfhttpd_url_callback_register(hfhttpd_url_callback_t callback, int flag);

int HSF_API hfhttpd_url_callback_cancel(void);

int HSF_API hfhttpd_send_data(char *data, int len);


int HSF_API hfnet_set_httpd_process_cmd_func(hfhttpd_process_cmd_func_t pfunc);

int  HSF_API hfnet_get_dhcp_ip (int * ip, int *mask, int *gw);

int ping(char* ip_address);

#define hfnet_ping(_ipadr)		ping(_ipadr)

int HSF_IAPI hfnet_send_assis_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_API hfnet_start_assis_ex(uint16_t port,hfnet_callback_t p_callback);

int HSF_API hfnet_assis_write(char *rep,int len,uint32_t addr,uint16_t port);

int HSF_API hfnet_gethostbyname(const char *name, ip_addr_t *addr);

int HSF_API hfnet_start_assis(uint16_t port);

int HSF_API hfnet_start_httpd(uint32_t uxpriority);

int HSF_API hfnet_start_socketb(uint32_t uxpriority,hfnet_callback_t p_uart_callback);

int HSF_API hfnet_start_socketa(uint32_t uxpriority,hfnet_callback_t p_uart_callback);

int HSF_API  hfnet_start_uart(uint32_t uxpriority,hfnet_callback_t p_uart_callback);

int HSF_API hfnet_start_uart_ex(uint32_t uxpriority,hfnet_callback_t p_callback, int stack_size);

int  HSF_API hfnet_socketa_send(char *data,uint32_t len,uint32_t timeouts);

int  HSF_API hfnet_socketa_eanble_clrtimeout_in_send(char b_enable);

int HSF_API hfnet_socketb_send(char *data,uint32_t len,uint32_t timeouts);

int HSF_IAPI hfnet_send_socketa_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_IAPI hfnet_send_socketb_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_API hfnet_socketa_fd(void);

int HSF_API hfnet_socketb_fd(void);

int HSF_API hfnet_socketa_get_client(int cid,phfnet_socketa_client_t p_client);

int HSF_IAPI hfnet_send_uart_event(uint32_t event,void *data,uint32_t len,uint32_t buf_len);

int HSF_IAPI hfnet_init(void);

int HSF_API hfnet_wifi_is_active(void);

int HSF_API  hfnet_wifi_up(void);

int HSF_API hfnet_wifi_down(void);

int HSF_API hfnet_set_udp_broadcast_port_valid(uint16_t start_port,uint16_t end_port);

int HSF_API hfnet_recvfrom(int s, void *mem, size_t len, int flags,
        struct sockaddr *from, socklen_t *fromlen,char * mac);

int HSF_API hfnet_get_mac_address_by_ip(uint32_t ipaddr,uint8_t *mac);

int HSF_API hfnet_get_smac(char *mac);

#endif


