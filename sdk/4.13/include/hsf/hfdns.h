/* hfdns.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_DNS_H_
#define _HF_DNS_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief start Domain Name Serve and create a dns record.
 *
 * @param[in] name: domain name
 *			 ip: the ip address of domain
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note this function will use a UDP pcb
 */
int HSF_API hfdns_start_server(char *name, unsigned int ip);

/**
 * @brief stop Domain Name Serve.
 *
 * @param[in] none
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None
 */
int HSF_API hfdns_stop_server(void);


#ifdef __cplusplus
}
#endif

#endif

