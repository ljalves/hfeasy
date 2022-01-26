/* hftime.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef HFTIME_H_
#define HFTIME_H_

#ifdef __cplusplus
extern "C"{
#endif


#ifdef CONFIG_NO_STRUCT_TIMEZONE
struct timezone 
{
	char none;
};
#endif

#if !CONFIG_HAVE_SETTIMEOFDAY
int settimeofday(const struct timeval * tv, const struct timezone *tz);
#endif

#if !CONFIG_HAVE_GETTIMEOFDAY
int gettimeofday(struct timeval *__p, void *__tz);
#endif


#ifdef __cplusplus
}
#endif

#endif

