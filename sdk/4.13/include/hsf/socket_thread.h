/* socket_thread.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _SOCKET_THREAD_H_
#define _SOCKET_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "sockets.h"
#include "uart_thread.h"

#define	M2M_MAXNETFRAME 				1500

#define SOCK_TCP_RETRY_TIMES		(6)

extern hfthread_mutex_t socket_mutex;
extern int m2m_tcpdisconnect; //fot AT+TCPDIS
extern int m2m_btcpconnector;//for AT+TCPLK
extern struct sockaddr_in m2m_udp_toaddr;

extern int m2m_netpreconnet;

extern int m2m_connect_socket;
extern int m2m_client_socket_num;

extern char *m2m_net_recv;

extern int netrecvnum;
extern int netreadnum;
extern int netnum;
extern char sock_ok_form;
extern signed int sock_thread_wdg;


#define 	DEFAULT_MAX_SOCK_CLIENT		5

typedef struct _M2M_CLIENT_SOCKET
{
	int fd;
	uint32_t time_to;
	struct sockaddr_in addr;
	char      active;
}M2M_CLIENT_SOCKET;

extern M2M_CLIENT_SOCKET m2m_client_sockets[DEFAULT_MAX_SOCK_CLIENT];

err_t HSF_IAPI netconn_gethostbyname(const char *name, ip_addr_t *addr);
int HSF_IAPI m2m_net_udp_write(int fd, char *ptr, int nbytes, struct sockaddr_in *toaddr);
int HSF_IAPI m2m_tcp_send(int *sock, char *ptr, int nbytes);

/**
 * @brief start socketA.
 *
 * @param[in] uxpriority: the uxpriority od socketa thread
 * @return[out] HF_SUCCESS-successfully, other value is failed
 */
int HSF_IAPI start_socketa(uint32_t uxpriority);

/**
 * @brief check whether or not socketA is started.
 *
 * @return[out] 0-not start, 1-have started
 */
int  HSF_IAPI socketa_is_started(void);

/**
 * @brief check the string is a valid IP address.
 *
 * @param[in] ipaddr: the ip address string
 * @return[out] 0-not a IP address, 1-is a IP address
 * @see None.
 * @note None.
 */
int HSF_IAPI is_ipaddress(const char * ipaddr);

/**
 * @brief check the string is a valid IP broadcast address.
 *
 * @param[in] ipaddr: the ip address string
 *			 maskaddr: the mask address string
 * @return[out] 0-not a IP broadcast address, 1-is a IP broadcast address
 * @see None.
 * @note None.
 */
int HSF_IAPI is_broadcast_ip(const char *ipaddr, const char *maskaddr);

/**
 * @brief get a rand local port.
 *
 * @return[out] local port, the range is [1000-65535]
 * @see None.
 * @note this system is not need.
 */
unsigned short HSF_IAPI sock_get_rand_local_port(void);

/**
 * @brief enable TCP keepalive.
 *
 * @param[in] sock: the handle of socket
 *			 idle: the number of seconds a connection needs to be idle before TCP begins sending out keep-alive probes
 *			 interval: the number of seconds between TCP keep-alive probes
 *			 count: the maximum number of TCP keep-alive probes to send before giving up and killing the connection if no response is obtained from the other end
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_IAPI sock_enable_tcp_keepalive(int sock, int idle, int interval, int count);

/**
 * @brief disable TCP keepalive.
 *
 * @param[in] sock: the handle of socket
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_IAPI sock_disable_tcp_keepalive(int sock);

/**
 * @brief set TCP socket send timeout.
 *
 * @param[in] sock: the handle of socket
 *			 timeout: send timeout, in seconds
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_IAPI sock_enable_tcp_sndtimeo(int sock, int timeout);


#ifdef __cplusplus
}
#endif

#endif

