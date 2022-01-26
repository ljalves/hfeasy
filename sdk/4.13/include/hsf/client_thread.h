/* client_thread.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _CLIENT_THREAD_H_
#define _CLIENT_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "socket_thread.h"

/**
 * @brief start sockrtA in TCP client mode.
 *
 */
void HSF_IAPI client_thread(void);


#ifdef __cplusplus
}
#endif

#endif

