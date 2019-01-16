/* hfat.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */
#ifndef _AT_H_H_H_
#define _AT_H_H_H_

#define HFAT_MAX_CMD_DATA_LEN	(255)

#define HFAT_REPLY_FLAG_NOSEND_TAIL		(0x00000001)

typedef int (*at_cmd_reply_func)(const char*,int len,int timeouts,int flags);
typedef int (*at_cmd_read_func)(char*buf,int len,int timeouts,int flags);

enum session_id_e
{
	AT_SESSION_UART0=0,
	AT_SESSION_UART1=1,
	AT_SESSION_ASSIST_SOCKET,
	AT_SESSION_API,
};

typedef struct _at_session
{
	at_cmd_reply_func pfreply;
	at_cmd_read_func pfread;
	int sid;
	char *rsp;
	int rsp_len;
	int cmd_id;
}at_session_t,*pat_session_t;

typedef pat_session_t at_session_handle;

typedef struct _at_cmd
{
	const char * name;
	int	(*func)(pat_session_t,int, char** ,char *,int);	// the main func , or init
	const char * doc;
	int	(*callhook)(pat_session_t,int, char** ,char *,int);
} hfat_cmd_t,*phfat_cmd_t;

typedef struct _at_cmd_category
{
	const char *name;
	phfat_cmd_t  item;
}hfat_cmd_category,*phfat_cmd_category;
/*
 *return value: <0 fail,>=0 success,the length of response
*/
int HSF_API send_at_cmd(at_session_handle handle,char *cmd_line, int len,char *response,int rsplen);

int HSF_API hfat_init(void);

int HSF_API hfat_send_cmd(char *cmd_line,int cmd_len,char *rsp,int len);

int HSF_API hfat_get_words(char *str,char **words,int size);

#endif


