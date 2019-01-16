/*
 *  Copyright 2012-2016, by L.B.
 *  All Rights Reserved.
 */

/** ntpclient.h: Functions for NTP
 */
 
#ifndef _NTPCLIENT_H_
#define _NTPCLIENT_H_

struct ntp_timestamp
{
	unsigned int secondsFrom1900;//1900年1月1日0时0分以来的秒数
	unsigned int fraction;//微秒的4294.967296(=2^32/10^6)倍
};

struct ntp_header
{
	union
	{
	   struct 
	   {
		    char local_precision;//表示本机时钟精度为2^local_precision秒。local_precision通常为负数。
		    char poll_intervals;//表示测试间隔为2^poll_intervals秒。
		    unsigned char stratum;//NTP服务器阶级，0表示不指定，1表示校准用原子钟。应该为0。
		    unsigned char mode : 3;//通信模式。应该为3，表示是client。
		    unsigned char version_number : 3;//NTP协议版本号。应该为3。
		    unsigned char leap_indicator : 2;//闰秒指示，一般填0。
	   }s;
	   int noname;
	}u;
	int root_delay;//可正可负。2^16表示一秒。具体含义参见RFC1305。
	int root_dispersion;//只可为正。2^16表示一秒。具体含义参见RFC1305。
	int reference_clock_identifier;//具体含义参见RFC1305。一般填0。
};//没有错误的话，ntp_header的大小应该为16字节。

typedef struct
{
	struct ntp_header header;
	//以下四个时间均为本地时间。即考虑了时区位置的。
	struct ntp_timestamp reference;//具体含义参见RFC1305。一般填0。
	struct ntp_timestamp originate;//上次发出时刻
	struct ntp_timestamp receive;//接收时刻
	struct ntp_timestamp transmit;//发出时刻
}ntp_packet;//没有错误的话，ntp_header的大小应该为48字节。

int NTP_update_time(void);

#endif/*_NTPCLIENT_H_*/
