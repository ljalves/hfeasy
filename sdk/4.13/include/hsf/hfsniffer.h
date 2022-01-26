 /* hfsniffer.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_SNIFFER_H_
#define _HF_SNIFFER_H_


#include "hfwifi.h"

#ifdef __cplusplus
extern "C" {
#endif


#define HFSNIFFER_RECE_MODE_BEACON_MANAG		0x0001
#define HFSNIFFER_RECE_MODE_BROADCAST_DATA		0x0002
#define HFSNIFFER_RECE_MODE_TO_DS				0x0004
#define HFSNIFFER_RECE_MODE_FROM_DS				0x0008


typedef void (*hfsniffer_recv_callback_t)(uint8_t* /*pkt*/,uint32_t /*pkt_len*/);

/**
 * @brief start wifi sniffer mode and register a callback function which will be called when wifi receive a frame.
 *
 * @param[in] mode: which pakcet mode want to sniffer, the following parameters are used in combination
 * ==========================================
 * | HFSNIFFER_RECE_MODE_BEACON_MANAG       |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_BROADCAST_DATA     |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_TO_DS              |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_FROM_DS            |
 * ==========================================
 *			 cb: a function pointer, called back when wifi receive a frame
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_start(uint32_t mode, hfsniffer_recv_callback_t cb);

/**
 * @brief stop wifi sniffer mode.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_stop(void);

/**
 * @brief set the filter which packet want to receive.
 *
 * @param[in] mode: the filter mode, the following parameters are used in combination
 * ==========================================
 * | HFSNIFFER_RECE_MODE_BEACON_MANAG       |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_BROADCAST_DATA     |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_TO_DS              |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_FROM_DS            |
 * ==========================================
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_set_filter(uint32_t mode);

/**
 * @brief switch to specific wifi channel.
 *
 * @param[in] ch: the specific channel
 *			 second_ch: no use
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_set_channel(uint8_t ch,uint8_t second_ch);

/**
 * @brief set the filter and switch to specific wifi channel.
 *
 * @param[in] ch: the specific channel
 *			 second_ch: no use
 *			 mode: the filter mode, the following parameters are used in combination
 * ==========================================
 * | HFSNIFFER_RECE_MODE_BEACON_MANAG       |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_BROADCAST_DATA     |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_TO_DS              |
 * |----------------------------------------|
 * | HFSNIFFER_RECE_MODE_FROM_DS            |
 * ========================================== 
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_set_channel_filter(uint8_t ch,uint8_t second_ch, uint32_t mode);

 /**
 * @brief check whether or not in wifi sniffer mode .
 *
 * @param[in] None
 * @return[out] 0-not in wifi sniffer mode, 1-in wifi sniffer mode
 * @see None.
 * @note None.
 */
int HSF_API hfsniffer_is_run(void);

 /**
 * @brief register a callback function which will be called when wifi receive a frame in normal mode(AP/STA/AP+STA).
 *
 * @param[in] cb: a function pointer, called back when wifi receive a frame
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see must use 'AT+DISPS=yes' close low power mode.
 * @note must call this function after wifi connected, cannot be called in system event callback function registered by 'hfsys_register_system_event'.
 */
int HSF_API hfsniffer_enable_recv(hfsniffer_recv_callback_t cb);

 /**
 * @brief disable recv frame in normal mode(AP/STA/AP+STA).
 *
 * @param[in] none
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note must call this function to stop receive wifi frame after wifi disconnected.
 */
int HSF_API hfsniffer_disable_recv(void);


#ifdef __cplusplus
}
#endif

#endif

