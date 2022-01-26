 /* hfntp.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_NTP_H_
#define _HF_NTP_H_

#ifdef __cplusplus
extern "C"{
#endif


/**
 * @brief get UTC time from NTP server.
 *
 * @param[in] addr: NTP server address, support domain name
 *			 port: NTP server port, such as "123"
 *			 timeout: recv response timeout, in milliseconds
 * @return[out] >0 UTC time from 1970-01-01 00:00, <0 is failed
 * @see None.
 * @note None.
 */
int HSF_API hfntp_get_time(const char *addr, unsigned short port, int timeout);


#ifdef __cplusplus
}
#endif

#endif

