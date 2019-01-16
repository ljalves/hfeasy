/* 
 *hfuart.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */
#ifndef _UART_H_H_H_
#define _UART_H_H_H_

typedef void * hfuart_handle_t;

#define	HFUART0			(hfuart_handle_t)9
#define 	HFUART1			(hfuart_handle_t)10

hfuart_handle_t HSF_API  hfuart_open(int uart_no);

int HSF_API  hfuart_close(hfuart_handle_t huart);


int HSF_API hfuart_send(hfuart_handle_t huart,char *data,uint32_t bytes,uint32_t timeouts);

int HSF_API hfuart_recv(hfuart_handle_t huart,char *recv,uint32_t bytes,uint32_t timeouts);

int HSF_API
hfuart_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
            struct timeval *timeout);

#endif

