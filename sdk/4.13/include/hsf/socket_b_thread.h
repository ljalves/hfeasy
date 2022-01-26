/* socket_b_thread.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _SOCKET_B_THREAD_H_
#define _SOCKET_B_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "sockets.h"

#define SOCKETB_MODE_NONE		0
#define SOCKETB_MODE_TCP		1
#define SOCKETB_MODE_UDP		2
#define SOCKETB_MODE_UDPS		3

#define M2M_MAXNETFRAMEB		1500

extern char 	sockb_mode;
extern struct sockaddr_in m2m_udp_toaddrb;
extern char 	*m2m_net_recvb;
extern int 	m2m_connect_socketb;
extern int 	m2m_tcpdisconnectb; //fot AT+TCPDIS
extern int 	m2m_btcpconnectorb;//for AT+TCPLK
extern int		m2m_netpreconnetb;

extern int		sockb_recvnum;
extern int		sockb_readnum;
extern int 	sockb_netnum;

extern char	sockb_ok_form;
extern char       m2m_connect_socketb_active;

/**
 * @brief start socketB.
 *
 * @param[in] uxpriority: the uxpriority od socketa thread
 * @return[out] HF_SUCCESS-successfully, other value is failed
 */
int HSF_IAPI start_socketb(uint32_t uxpriority);

/**
 * @brief check whether or not socketB is started.
 *
 * @return[out] 0-not start, 1-have started
 */
int HSF_IAPI socketb_is_started(void);


#ifdef __cplusplus
}
#endif

#endif

