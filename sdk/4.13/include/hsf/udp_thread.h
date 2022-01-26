/* udp_thread.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _UDP_THREAD_H_
#define _UDP_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "socket_thread.h"

/**
 * @brief start sockrtA in UDP mode.
 *
 * @param[in] type: 0-UDP client, 1-UDP server
 */
void HSF_IAPI udp_thread(char type);


#ifdef __cplusplus
}
#endif

#endif

