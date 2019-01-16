
/* hftimer.h
 *
 * Copyright (C) 2013 High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef HFTIMER_H_
#define HFTIMER_H_

typedef void * hftimer_handle_t;

#define HFTIMER_FLAG_HARDWARE_TIMER				(0x00000001)

/* Define the prototype to which timer callback functions must conform. */
typedef void (*hf_timer_callback)( hftimer_handle_t htimer );

float HSF_IAPI hftimer_get_timer_adjust_factor(void);

hftimer_handle_t HSF_API  hftimer_create( const char *name, int32_t period, bool auto_reload,uint32_t  timer_id, hf_timer_callback p_callback,uint32_t flags );

uint32_t  HSF_API hftimer_get_timer_id( hftimer_handle_t htimer );

void HSF_API  hftimer_delete(hftimer_handle_t htimer);

int HSF_API hftimer_start(hftimer_handle_t htimer);

void HSF_API  hftimer_stop(hftimer_handle_t htimer);

void HSF_API hftimer_change_period(hftimer_handle_t htimer,int32_t new_period);

uint32_t HSF_API hftimer_get_counter(hftimer_handle_t htimer);

int HSF_IAPI hftimer_auto_adjust(void);

bool HSF_IAPI hftimer_is_adjust(void);


#endif /* HFTIMER_H_ */


