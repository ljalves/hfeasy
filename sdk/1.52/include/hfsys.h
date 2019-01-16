
/* hfsys.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */


#ifndef _H_H_HFSYS_H_
#define _H_H_HFSYS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum HFSYS_RUN_MODE_E
{
	HFSYS_STATE_RUN_THROUGH=0,
	HFSYS_STATE_RUN_CMD=1,
	HFSYS_STATE_RUN_GPIO,
	HFSYS_STATE_RUN_PWM,
	HFSYS_STATE_MAX_VALUE
};

enum HFSYS_EVENT_E
{
	HFE_WIFI_STA_CONNECTED=0,
	HFE_WIFI_STA_DISCONNECTED=1,
	HFE_CONFIG_RELOAD,
	HFE_DHCP_OK,
	HFE_SMTLK_OK,
};

#define HFSYS_RESET_REASON_NORMAL			(0)
#define HFSYS_RESET_REASON_ERESET			(0x00000001)
#define HFSYS_RESET_REASON_IRESET0			(0x00000002)
#define HFSYS_RESET_REASON_IRESET1			(0x00000004)
#define HFSYS_RESET_REASON_WPS				(0x00000008)
#define HFSYS_RESET_REASON_SMARTLINK_START	(0x00000010)
#define HFSYS_RESET_REASON_SMARTLINK_OK		(0x00000020)
#define HFSYS_RESET_REASON_WPS_OK			(0x00000080)

	

typedef int (*hfsys_event_callback_t)( uint32_t /*event_id*/,void * /*param*/);


uint32_t HSF_API  hfsys_get_reset_reason(void);


const char * HSF_API hfsys_get_sdk_version(void);

//memory
/*
 *
*/
void * HSF_API hfmem_malloc(size_t size);

/*
 *
*/
void * HSF_API hfmem_realloc(void *pv, size_t size);

/*
 * 
 */
void HSF_API hfmem_free(void *pv);


void HSF_API *hfmem_realloc(void *pv,size_t size);


/*
 *mode:²Î¿¼HFSYS_RUN_MODE_E
*/
int HSF_API hfsys_switch_run_mode(int mode);

/*
*/
int HSF_API hfsys_get_run_mode(void);

/*
 * reset hf module
*/
void HSF_API hfsys_reset(void);

void HSF_API hfsys_softreset(void);


/*
 * reload config
 */
void HSF_API hfsys_reload(void);

/*
 * Retrieves the number of milliseconds that have elapsed since the system was started;
 */
uint32_t HSF_API hfsys_get_time(void);

/*
 * Monitor memory status.
 */
uint32_t HSF_API hfsys_get_memory(void);
/*
  *
  */
int HSF_IAPI hfsys_send_event(uint32_t event_id,void *param);

/*
 *
 */
int HSF_API hfsys_register_system_event(hfsys_event_callback_t p_callback);

/*

*/
int HSF_API hfsys_unregister_system_event(void);

void HSF_API hfsys_disable_all_soft_watchdogs(void);

void HSF_API hfsys_enable_all_soft_watchdogs(void);

int HSF_API hfsys_nvm_read(uint32_t nvm_addr, char* buf, uint32_t length);

int HSF_API hfsys_nvm_write(uint32_t nvm_addr, char* buf, uint32_t length);


#ifdef __cplusplus
}
#endif

#endif /* HFSYS_H_ */
