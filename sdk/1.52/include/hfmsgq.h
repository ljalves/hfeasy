

/* hfmsgq.h
 *
 * Copyright (C) 2014 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_MSGQ_H_H_H_H_
#define _HF_MSGQ_H_H_H_H_

typedef int*  HFMSGQ_HANDLE;

#define MSGQ_DELAY_INFINITE			(0xFFFFFFFF)

HFMSGQ_HANDLE hfmsgq_create(uint32_t  size,uint32_t item_size );

void	hfmsgq_destroy(HFMSGQ_HANDLE msgq);

int hfmsgq_send(HFMSGQ_HANDLE msgq,void *msg,uint32_t timeouts,uint32_t flags);

int hfmsgq_recv(HFMSGQ_HANDLE msgq,void *msg,uint32_t timeouts,uint32_t flags);

#endif


