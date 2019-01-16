
/* hfthread.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _H_HF_THREAD_H_H_
#define _H_HF_THREAD_H_H_

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

void hf_thread_delay(uint32_t ms);

int HSF_IAPI hfthread_module_init(void);


int HSF_API hfthread_create(PHFTHREAD_START_ROUTINE routine,const char * const name, uint16_t stack_depth, void *parameters, uint32_t uxpriority, hfthread_hande_t *created_thread, uint32_t *stack_buffer);

void HSF_API hfthread_destroy(hfthread_hande_t thread);

int HSF_API hfthread_enable_softwatchdog(hfthread_hande_t thread,uint32_t time);

int HSF_API hfthread_disable_softwatchdog(hfthread_hande_t thread);

int HSF_API hfthread_reset_softwatchdog(hfthread_hande_t thread);

int HSF_IAPI hfthread_softwatchdog_check(void);

void HSF_API hfthread_suspend_all(void);

void HSF_API hfthread_resume_all(void);


#ifndef msleep
#define msleep(ms)	hf_thread_delay(ms)
#endif

#ifndef sleep
#define sleep(_s)	msleep((_s)*1000)
#endif

typedef void* hfthread_mutex_t;
typedef void* hfthread_sem_t;

#define NULL_MUTEX	(hfthread_mutex_t)0

int HSF_API hfthread_mutext_new(hfthread_mutex_t *mutex);
void HSF_API hfthread_mutext_free(hfthread_mutex_t mutex);
void HSF_API hfthread_mutext_unlock(hfthread_mutex_t mutex);
int HSF_API hfthread_mutext_wait(hfthread_mutex_t mutex, uint32_t timeout);
int HSF_API hfthread_mutext_trylock(hfthread_mutex_t mutex);


int HSF_API hfthread_sem_new(hfthread_sem_t *psem,uint8_t cnt);
void HSF_API hfthread_sem_free(hfthread_sem_t sem);
void HSF_API hfthread_sem_signal(hfthread_sem_t sem);
int HSF_API hfthread_sem_wait(hfthread_sem_t sem, uint32_t timeout);


#define hfthread_mutext_lock(_mu)	hfthread_mutext_wait(_mu,0)



#ifdef __cplusplus
}
#endif

#endif
