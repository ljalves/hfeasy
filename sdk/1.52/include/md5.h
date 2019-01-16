
/* md5.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */

#ifndef _MD5_H_H_H__
#define _MD5_H_H_H__

struct MD5Context {
	unsigned int buf[4];
	unsigned int bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);

void MD5Update(struct MD5Context *context, unsigned char const *buf,
	       unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);

#endif


