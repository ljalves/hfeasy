/******************************************************************************
 * @file     wifi_arch.h
 * @brief    Architecture specific WiFi interface, must be implemented by each target
 * @version: V1.0
 * @date:    25. July 2016

 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef WIFIARCH_H_
#define WIFIARCH_H_

#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t wifi_arch_enetif_init(struct netif *netif);

#ifdef __cplusplus
}
#endif

#endif /* WIFIARCH_H_ */

