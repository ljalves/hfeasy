
/* hsf.h
 *
 * Copyright (C) 2013 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HSF_H_H_H_H_H_
#define _HSF_H_H_H_H_H_

#include "hfconfig.h"

#ifdef _ATMEL_ASF_
#include <asf.h>
#endif

#if defined   ( __CC_ARM   )
#	define USER_FUNC	//__attribute__ ((section(".appmain")))
#	define HSF_API		//__attribute__ ((section(".appmain")))
#elif defined (  __GNUC__  )
#   define USER_FUNC __attribute__((section(".appmain")))
#	define HSF_API		__attribute__ ((section(".appmain")))
#else
#define USER_FUNC
#endif

#define HSF_IAPI		HSF_API

#define _HSF_INLINE_	inline

#ifndef bzero
#define bzero(_ptr,_size)	memset(_ptr,0,_size)
#endif

#ifdef __BUILD_HSF_SDK__

#ifdef CONFIG_CHIP_HFO18
#include <os/os.h>
#include "driver/gpio.h"
#endif

#ifdef CONFIG_FREERTOS
#include <FreeRTOS.h>

	#ifndef NORMAL_FREERTOS
#define pvPortMalloc(size) OSMalloc(size, MMM_ALLOC_NORMAL)
#define vPortFree(ptr) OSFree(ptr)
	#endif
	
#endif

#endif

#include "hftypes.h"
#include "hfsys.h"
#include "hferrno.h"
#include "hf_debug.h"
#include "hfat.h"
#include "hfgpio.h"
#include "hfspi.h"
#include "hfthread.h"
#include "hfnet.h"
#include "hfuart.h"
#include "hfusb.h"
#include "hftimer.h"
#include "hftime.h"
#include "hffile.h"
#include "hfupdate.h"
#include "hfflash.h"
#include "hfflashlayout.h"
#include <hfir.h>
#include <hfwifi.h>
#include <hfsmtlk.h>
#ifdef __LPB100U__
#include <hfusbhosthcd.h>
#include <hfaudio.h>
#endif

#endif


