 /* hfthread.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_THREAD_H_
#define _HF_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


#define HFTHREAD_PRIORITIES_LOW		(1)
#define HFTHREAD_PRIORITIES_MID		(2)
#define HFTHREAD_PRIORITIES_NORMAL	(3)
#define HFTHREAD_PRIORITIES_HIGH		(4)
#define HFTHREAD_MAX_PRIORITIES		(5)


typedef void * hfthread_hande_t;
typedef void (*PHFTHREAD_START_ROUTINE)( void * );


int HSF_IAPI hfthread_module_init(void);

int HSF_IAPI hfthread_softwatchdog_check(void);

/**
 * @brief create a thread.
 *
 * @param[in] routine: a pointer to the application-defined function to be executed by the thread, not allow in ISR.
 *			 name: thread name, maximum length is 32
 *			 stack_depth: the size of the stack, real size is [stack_depth*4]
 *			 parameters: a pointer to a variable to be passed to the start_routine
 *			 uxpriority: the uxpriority of thread, as follows, in order to ensure the stability of the system , please use 'HFTHREAD_PRIORITIES_LOW' and 'HFTHREAD_PRIORITIES_MID'
 * ==========================================
 * | HFTHREAD_PRIORITIES_LOW                |
 * |----------------------------------------|
 * | HFTHREAD_PRIORITIES_MID                |
 * |----------------------------------------|
 * | HFTHREAD_PRIORITIES_NORMAL             |
 * |----------------------------------------|
 * | HFTHREAD_PRIORITIES_HIGH               |
 * ==========================================
 *			 created_thread: the new thread handle
 *			 stack_buffer: the thread statck buf used by new thread, generally set NULL, system will malloc automatically
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note if you want exit thread, must call 'hfthread_destroy'.
 */
int HSF_API hfthread_create(PHFTHREAD_START_ROUTINE routine,const char * const name, uint16_t stack_depth, void *parameters, uint32_t uxpriority, hfthread_hande_t *created_thread, uint32_t *stack_buffer);

/**
 * @brief destroy a thread, not allow in ISR.
 *
 * @param[in] thread: the thread handle, NULL mean itself
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_destroy(hfthread_hande_t thread);

/**
 * @brief sleep thread itself.
 *
 * @param[in] ms: the time interval for which execution is to be suspended, in milliseconds
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hf_thread_delay(uint32_t ms);

/**
 * @brief show state of threads create by 'hfthread_create'.
 *
 * @param[in] rsp: a buf to store thread state, if NULL, will printf to UART
 *			 rsp_len: the size of rsp
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_show(char *rsp, int rsp_len);

/**
 * @brief get current thread handle, not allow in ISR.
 *
 * @param[in] None
 * @return[out] the handle of current thread
 * @see None.
 * @note None.
 */
hfthread_hande_t HSF_API hfthread_get_current_handle(void);

/**
 * @brief get current taskid thread handle, not allow in ISR.
 *
 * @param[in] None
 * @return[out] the taskid of current thread
 * @see None.
 * @note None.
 */
int HSF_API hfthread_get_current_taskid(void);

/**
 * @brief enable watchdog of specified thread.
 *
 * @param[in] thread: the thread handle, NULL mean itself
 *			 timeout: watchdog timeout, in seconds
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note if enable softwatchdog, must periodically call 'hfthread_reset_softwatchdog' to feed the watchdog, and the cycle is less than 'timeout'.
 */
int HSF_API hfthread_enable_softwatchdog(hfthread_hande_t thread, uint32_t timeout);

/**
 * @brief disable watchdog of specified thread.
 *
 * @param[in] thread: the thread handle, NULL mean itself
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_disable_softwatchdog(hfthread_hande_t thread);

/**
 * @brief reset watchdog of specified thread.
 *
 * @param[in] thread: the thread handle, NULL mean itself
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_reset_softwatchdog(hfthread_hande_t thread);

/**
 * @brief suspend the task scheduler.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_suspend_all(void);

/**
 * @brief resume the task scheduler.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_resume_all(void);


#ifndef msleep
#define msleep(ms)	hf_thread_delay(ms)
#endif

#ifndef sleep
#define sleep(_s)	msleep((_s)*1000)
#endif

typedef void* hfthread_mutex_t;

#define NULL_MUTEX	(hfthread_mutex_t)0

/**
 * @brief create a mutex, not allow in ISR.
 *
 * @param[in] mutex: a pointer to mutex handle
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_mutext_new(hfthread_mutex_t *pmutex);

/**
 * @brief free a mutex, not allow in ISR.
 *
 * @param[in] pmutex: the specified mutex handle
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_mutext_free(hfthread_mutex_t mutex);

/**
 * @brief releases ownership of the specified mutex object, not allow in ISR.
 *
 * @param[in] mutex: the specified mutex handle
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_mutext_unlock(hfthread_mutex_t mutex);

/**
 * @brief waits until the specified mutex is in the signaled state, not allow in ISR.
 *
 * @param[in] mutex: the specified mutex handle
 *			 timeout: timeout interval in millisecond, 0xFFFFFFFF mean the function will return only when the mutex is signaled
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_mutext_wait(hfthread_mutex_t mutex, uint32_t timeout);

/**
 * @brief try to wait a mutex, not allow in ISR.
 *
 * @param[in] mutex: the specified mutex handle
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_mutext_trylock(hfthread_mutex_t mutex);

/**
 * @brief wait a mutex until the mutex is signaled.
 */
#define hfthread_mutext_lock(_mu)	hfthread_mutext_wait(_mu,0xFFFFFFFF)

typedef void* hfthread_sem_t;

/**
 * @brief create a semaphore, not allow in ISR.
 *
 * @param[in] psem: a pointer to semaphore handle
 *			 cnt: the count of the semaphore, maximum is 255
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfthread_sem_new(hfthread_sem_t *psem,uint8_t cnt);

/**
 * @brief free a semaphore, not allow in ISR.
 *
 * @param[in] sem: the specified semaphore handle
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_sem_free(hfthread_sem_t sem);

/**
 * @brief increases the count of the specified semaphore object by 1.
 *
 * @param[in] sem: the specified semaphore handle
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfthread_sem_signal(hfthread_sem_t sem);

/**
 * @brief wait until the specified mutex is in the signaled state or the time-out interval elapses, not allow in ISR.
 *
 * @param[in] sem: the specified semaphore handle
 *			 timeout: timeout interval in millisecond, 0xFFFFFFFF mean the function will return only when the semaphore is signaled
 * @return[out] 1-the state of the specified object is signaled, 0-the time-out interval elapsed, and the object's state is nonsignaled
 * @see None.
 * @note None.
 */
int HSF_API hfthread_sem_wait(hfthread_sem_t sem, uint32_t timeout);


#ifdef __cplusplus
}
#endif

#endif

