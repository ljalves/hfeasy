/* hfsys.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _HF_SYS_H_
#define _HF_SYS_H_

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
	HFE_WPS_OK
};


#ifndef msleep
#define msleep(ms) hf_thread_delay(ms)
#endif

#ifdef TOOLCHAIN_GCC_ARM
#define HAVE_BZERO_FUNCTION			1
#else
#define HAVE_BZERO_FUNCTION			0
#endif

#if !defined(bzero)&&(HAVE_BZERO_FUNCTION==0)
#define bzero(_ptr,_size)	memset(_ptr,0,_size)
#endif

#define HFSYS_RESET_REASON_NORMAL			(0)
#define HFSYS_RESET_REASON_ERESET			(0x00000001)
#define HFSYS_RESET_REASON_IRESET0			(0x00000002)
#define HFSYS_RESET_REASON_IRESET1			(0x00000004)
#define HFSYS_RESET_REASON_WPS				(0x00000008)
#define HFSYS_RESET_REASON_SMARTLINK_START	(0x00000010)
#define HFSYS_RESET_REASON_SMARTLINK_OK		(0x00000020)
#define HFSYS_RESET_REASON_WPS_OK			(0x00000080)


typedef int (*hfsys_event_callback_t)( uint32_t /*event_id*/,void * /*param*/);

int HSF_IAPI hfsys_init(void);

void HSF_IAPI  hfsys_set_reset_reason(uint32_t reason);

int HSF_IAPI hfsys_send_event(uint32_t event_id,void *param);

/**
 * @brief get module reset reason.
 *
 * @param[in] None
 * @return[out] 
 * ================================================================================================
 * | HFSYS_RESET_REASON_NORMAL             | 'power off' or 'hardware watchdog' or 'reset button' |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_IRESET0            | 'hfsys_softreset' or 'software watchdog'             |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_IRESET1            | 'hfsys_reset'                                        |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_SMARTLINK_START    | SmartLink start                                      |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_SMARTLINK_OK       | SmartLink OK                                         |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_WPS                | WPS start                                            |
 * |----------------------------------------------------------------------------------------------|
 * | HFSYS_RESET_REASON_WPS_OK             | WPS OK                                               |
 * ================================================================================================
 * @see None.
 * @note None.
 */
uint32_t HSF_API  hfsys_get_reset_reason(void);

/**
 * @brief get SDK Version.
 *
 * @param[in] None
 * @return[out] a pointer to the start address of version, such as: "4.10"
 * @see None.
 * @note None.
 */
const char * HSF_API hfsys_get_sdk_version(void);

/**
 * @brief allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
 *
 * @param[in] size: specify block size in bytes
 * @return[out] a pointer to the beginning of the block
 * @see None.
 * @note None.
 */
void HSF_API *hfmem_malloc(int size);

/**
 * @brief deallocate memory block.
 *
 * @param[in] pv: pointer to a memory block previously allocated with platform_malloc
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfmem_free(void *pv);

/**
 * @brief reallocates a block of size bytes of memory, returning a pointer to the beginning of the block.
 *
 * @param[in] pv: pointer to a memory block previously allocated with platform_malloc
 *			 size: specify block size in bytes
 * @return[out] a pointer to the beginning of the block
 * @see None.
 * @note None.
 */
void HSF_API *hfmem_realloc(void *pv, size_t size);

/**
 * @brief get system free IRAM size, the maximum size you can malloc.
 *
 * @param[in] None
 * @return[out] the size of free memory, in bytes
 * @see None.
 * @note IRAM is heap space.
 */
uint32_t HSF_API hfsys_get_memory(void);

/**
 * @brief get system run mode.
 *
 * @param[in] None
 * @return[out] the follow value:
 * ==========================================
 * | HFSYS_STATE_RUN_THROUGH                |
 * |----------------------------------------|
 * | HFSYS_STATE_RUN_CMD                    |
 * ==========================================
 * @see None.
 * @note None.
 */
int HSF_API hfsys_get_run_mode(void);

/**
 * @brief switch system run mode.
 *
 * @param[in] mode: the specified system mode
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsys_switch_run_mode(int mode);

/**
 * @brief reset system.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfsys_reset(void);

/**
 * @brief softreset system.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note will keep GPIO state.
 */
void HSF_API hfsys_softreset(void);

/**
 * @brief restore the configuration to the factory status.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfsys_reload(void);

/**
 * @brief retrieves the number of milliseconds that have elapsed since the system was boot.
 *
 * @param[in] None
 * @return[out] in millisecond, the running time of the system from start, start from 0
 * @see None.
 * @note None.
 */
uint32_t HSF_API hfsys_get_time(void);

/**
 * @brief set system time, can get by function 'time(NULL)'.
 *
 * @param[in] t: the specified time, in second, such as: 1325376000 mean 2012-01-01 00:00:00
 * @return[out] None
 * @see None.
 * @note do not affect the return value of 'hfsys_get_time'.
 */
void HSF_API hfsys_set_time(uint32_t t);

/**
 * @brief register system callback function which will be called when systen happen the follow cases.
 * ==========================================
 * | HFE_WIFI_STA_CONNECTED                 |
 * |----------------------------------------|
 * | HFE_WIFI_STA_DISCONNECTED              |
 * |----------------------------------------|
 * | HFE_CONFIG_RELOAD                      |
 * |----------------------------------------|
 * | HFE_DHCP_OK                            |
 * |----------------------------------------|
 * | HFE_SMTLK_OK                           |
 * |----------------------------------------|
 * | HFE_WPS_OK                             |
 * ==========================================
 *
 * @param[in] p_callback: a function pointer, called back when system event happened
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsys_register_system_event(hfsys_event_callback_t p_callback);

/**
 * @brief unregister system callback.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsys_unregister_system_event(void);

/**
 * @brief read data from NVRAM.
 *
 * @param[in] nvm_addr: nvram data address, 0-99
 *			 buf: a pointer to a buffer to store read data
 *			 length: the length, in bytes, of the data pointed to by the buf parameter
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsys_nvm_read(uint32_t nvm_addr, char* buf, uint32_t length);

/**
 * @brief write data to NVRAM.
 *
 * @param[in] nvm_addr: nvram data address, 0-99
 *			 buf: a pointer to a buffer to write data
 *			 length: the length, in bytes, of the data pointed to by the buf parameter
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsys_nvm_write(uint32_t nvm_addr, char* buf, uint32_t length);

/**
 * @brief stop system waiting for a few microseconds, no system scheduling, not allow in ISR.
 *
 * @param[in] us: the wait time, in microseconds
 * @return[out] None
 * @see None.
 * @note only support 30us of the multiple.
 */
void HSF_API hfsys_wait_us(uint32_t us);

/**
 * @brief start system watchdog, timeout is 10s, use a hardware timer to feed each second.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfsys_start_watchdog(void);

/**
 * @brief stop system watchdog.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note if stop watchdog, software watchdogs of thread will also be turned off.
 */
void HSF_API hfsys_stop_watchdog(void);

/**
 * @brief get random number.
 *
 * @param[in] None
 * @return[out] a random number
 * @see None.
 * @note None.
 */
unsigned int HSF_API hfsys_random(void);


#ifdef __cplusplus
}
#endif

#endif

