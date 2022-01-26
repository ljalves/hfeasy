/* hfsmtaplk.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _HF_SMTAPLK_H_
#define _HF_SMTAPLK_H_

#ifdef __cplusplus
extern "C"{
#endif


int HSF_IAPI hfsmtaplk_start_ap(void);

int HSF_IAPI hfsmtaplk_main_loop(void);

int HSF_IAPI hfsmtaplk_response(void);


enum SMT_APLINK_CODE
{
	GET_VER_REQ = 30001,
	GET_VER_RESP = 30002,
	GET_SCAN_REQ = 30003,
	GET_SCAN_RESP = 30004,
	SET_CONFIG_REQ = 30005,
	SET_CONFIG_RESP = 30006,
	RESTART_REQ = 30007,
	RESTART_RESP = 30008
};

/**
 * @brief start smart aplink.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note this function will reboot.
 */
void HSF_API hfsmtaplk_start(void);

/**
 * @brief check whether or not in smart aplink.
 *
 * @param[in] None
 * @return[out] 0-not in smart aplink, 1-in smart aplink
 * @see None.
 * @note None.
 */
int HSF_API hfsmtaplk_is_start(void);


#ifdef __cplusplus
}
#endif

#endif

