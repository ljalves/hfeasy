

/* hfconfig.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */

#ifndef _HF_CONFIG_H_H_H_
#define _HF_CONFIG_H_H_H_

#define __USE_HSF_API__

#ifdef _ATMEL_ASF_
#define CONFIG_HAVE_MALLINFO		1
#define CONFIG_HAVE_TIME_FUNCS	1
#define CONFIG_HAVE_SETTIMEOFDAY 	0
#define CONFIG_HAVE_GETTIMEOFDAY 	1
//#define CONFIG_NO_STRUCT_TIMEZONE
#define CONFIG_CHIP_SAM4SXX
#define CONFIG_CHIP_SAM4S16B
//#define CONFIG_CHIP_SAM4SA168
//#define CONFIG_CHIP_SAM4S8B
#define CONFIG_MODULE_LPB
#define CONFIG_FREERTOS
#endif


#ifdef __KEIL_MDK__
#define CONFIG_HAVE_MALLINFO		0
#define CONFIG_HAVE_TIME_FUNCS	1
#define CONFIG_HAVE_SETTIMEOFDAY 	0
#define CONFIG_HAVE_GETTIMEOFDAY 	0
#define CONFIG_NO_STRUCT_TIMEZONE
#define CONFIG_CHIP_HFO18
#define CONFIG_MODULE_LPB100
#define CONFIG_FREERTOS
//#define CONFIG_USE_018_SDK_MSGQ
#endif



#endif

