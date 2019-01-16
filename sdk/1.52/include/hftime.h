
/* hftime.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */


#ifndef HFTIME_H_
#define HFTIME_H_

#ifdef CONFIG_NO_STRUCT_TIMEZONE
struct timezone 
{
	char none;
};
#endif

#if !CONFIG_HAVE_SETTIMEOFDAY
int HSF_API settimeofday(const struct timeval * tv, const struct timezone *tz);
#endif

#if !CONFIG_HAVE_GETTIMEOFDAY
int HSF_API gettimeofday(struct timeval *__p, void *__tz);
#endif



#endif /* HFTIME_H_ */



