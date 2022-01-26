/* hsf.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HSF_H_
#define _HSF_H_

#define HSF_API
#define HSF_IAPI HSF_API
#define USER_FUNC

#ifdef __cplusplus
#include <mbed.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif
#include <stdint.h>

#ifndef BYTE
typedef unsigned char	BOOL;
typedef unsigned char	BYTE;
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;
typedef int				INT;
typedef unsigned int	UINT;
typedef long			LONG;
typedef unsigned long	DWORD;
#endif

#ifndef FALSE
#define FALSE			((BOOL) 0)
#define TRUE			((BOOL) 1)
#endif /* FALSE */

#define HF_SUCCESS 		0

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC	extern
typedef unsigned char           bool;
#define true                    1
#define false					0
#endif

#include "hferrno.h"
#include "hfconfig.h"
#include "hfat.h"
#include "hfsys.h"
#include "hfflash.h"
#include "hfthread.h"
#include "hfflashlayout.h"
#include "hffile.h"
#include "hfnet.h"
#include "hf_debug.h"
#include "hfgpio.h"
#include "hfuart.h"
#include "hftimer.h"
#include "hfupdate.h"
#include "hfwifi.h"
#include "hfsniffer.h"
#include "hftime.h"
#include "hfsmtlk.h"
#include "hfmsgq.h"
#include "hfdns.h"
#include "hfspi.h"
#include "hfcrypto.h"
#include "hfntp.h"
#include "hfsmtaplk.h"


#define GLOBAL_VER 				"4.13"

#ifdef CONFIG_FLASH_SIZE_1MB
#define GLOBAL_LVER				"31 (2020-11-06 17:00 1M)"
#elif defined CONFIG_FLASH_SIZE_2MB
#define GLOBAL_LVER				"31 (2020-11-06 17:00 2M)"
#elif defined CONFIG_FLASH_SIZE_4MB
#define GLOBAL_LVER				"31 (2020-11-06 17:00 4M)"
#endif


#endif

