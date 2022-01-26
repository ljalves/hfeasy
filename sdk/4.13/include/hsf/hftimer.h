/* hftimer.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_TIMER_H_
#define _HF_TIMER_H_

#ifdef __cplusplus
extern "C"{
#endif


int HSF_IAPI hftimer_init(void);

typedef void * hftimer_handle_t;

#define HFTIMER_FLAG_HARDWARE_TIMER				(0x00000001)

/* Define the prototype to which timer callback functions must conform. */
typedef void (*hf_timer_callback)( hftimer_handle_t htimer );


enum HFTIMER_DRIVEN_TYPE
{
	HFTIMER_DRIVEN_BY_TICK=0,
	HFTIMER_DRIVEN_BY_THREAD=1
};

/**
 * @brief set the driver for the timer, default is tick driven, this function is not necessary.
 *
 * @param[in] type: 'HFTIMER_DRIVEN_BY_TICK' or 'HFTIMER_DRIVEN_BY_THREAD'
 * @return[out] None
 * @see None.
 * @note must call this function before use hftimer functions, it is recommended to call in the function 'app_init'.
 */
void HSF_API hftimer_set_driven_type(enum HFTIMER_DRIVEN_TYPE type);

/**
 * @brief set the stack size of timer thread when hftimer is driven by thread.
 *
 * @param[in] stack_size: the size of the stack, real size is [stack_size*4], default is 256
 * @return[out] None
 * @see None.
 * @note must call this function before use hftimer functions, it is recommended to call in the function 'app_init'.
 */
void HSF_API hftimer_set_thread_stack_size(int stack_size);		 
 
/**
 * @brief create a timer.
 *
 * @param[in] name: no use
 *			 period: the period of timer, software timer is in milliseconds, hardware timer is in microsecond must >=50us
 *			 auto_reload: when timer expired whether or not restart
 *			 timer_id: timer id, use to distinguish different timers
 *			 p_callback: timer callback function pointer
 *			 flags: 0 is software timer, HFTIMER_FLAG_HARDWARE_TIMER is hardware timer
 * @return[out] the handle of timer, if NULL mean create failed
 * @see None.
 * @note None.
 */
hftimer_handle_t HSF_API hftimer_create( const char *name, int32_t period, bool auto_reload,uint32_t  timer_id, hf_timer_callback p_callback,uint32_t flags );

/**
 * @brief get timer id.
 *
 * @param[in] htimer: the handle of timer
 * @return[out] timer id
 * @see None.
 * @note None.
 */
uint32_t HSF_API hftimer_get_timer_id( hftimer_handle_t htimer );

/**
 * @brief delete timer.
 *
 * @param[in] htimer: the handle of timer
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hftimer_delete(hftimer_handle_t htimer);

/**
 * @brief start timer.
 *
 * @param[in] htimer: the handle of timer
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hftimer_start(hftimer_handle_t htimer);

/**
 * @brief stop timer.
 *
 * @param[in] htimer: the handle of timer
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hftimer_stop(hftimer_handle_t htimer);

/**
 * @brief change the period of timer.
 *
 * @param[in] htimer: the handle of timer
 *			 new_period: the new period of timer, software timer is in milliseconds, hardware timer is in microsecond
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hftimer_change_period(hftimer_handle_t htimer, int32_t new_period);


#ifdef __cplusplus
}
#endif

#endif

