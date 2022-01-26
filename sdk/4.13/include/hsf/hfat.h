 /* hfat.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_AT_H_
#define _HF_AT_H_

#include "hsf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HFAT_MAX_CMD_DATA_LEN	(1024)
#define HFAT_REPLY_FLAG_NOSEND_TAIL		(0x00000001)

//#define HFAT_DELETE_PARTIAL_CMD

typedef int (*at_cmd_reply_func)(const char*,int len,int timeouts,int flags);
typedef int (*at_cmd_read_func)(char*buf,int len,int timeouts,int flags);

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


enum session_id_e
{
	AT_SESSION_UART0=0,
	AT_SESSION_UART1=1,
	AT_SESSION_ASSIST_SOCKET,
	AT_SESSION_API,
};

enum ENCMDOPCODE
{
	OPCODE_NONE,
	OPCODE_EQUA,
	OPCODE_WFLASH,
	OPCODE_QUERY
};

enum ENCMDCODE
{
	CMD_ERR							=-1,
	CMD_NONE							= 0,
};

#define HFAT_TABLE_INDEX_USER		0
#define HFAT_TABLE_INDEX_UTILS	1
#define HFAT_TABLE_INDEX_SOCKA	2
#define HFAT_TABLE_INDEX_SOCKB	3
#define HFAT_TABLE_INDEX_WIFI		4
#define HFAT_TABLE_INDEX_NETWORK	5
#define HFAT_TABLE_INDEX_WIFITEST	6

extern hfat_cmd_category hf_at_cmd_tables[];
extern const hfat_cmd_t user_define_at_cmds_table[];
extern const hfat_cmd_t hf_socketa_at_cmds_table[];
extern const hfat_cmd_t hf_socketb_at_cmds_table[];
extern const hfat_cmd_t hf_network_at_cmds_table[];
extern const hfat_cmd_t hf_wifi_at_cmds_table[];
extern void usart_send(char* Buf,DWORD BufLen);


void HSF_IAPI print_hex(char *rep, unsigned char h);

int HSF_IAPI pars_uart_cmd(char *cmd_line, int len);

void HSF_IAPI cmd_reply(char *reply);

int HSF_IAPI strtowords(char *str,char *words[],int size,char *separators);

int HSF_IAPI hfat_assis_send_cmd(char *cmd_line,int cmd_len);

int HSF_IAPI hfat_init(void);

unsigned char HSF_IAPI str_escape(char *src);


/**
 * @brief enable/disable uart at command.
 *
 * @param[in] enable: 1-enable, 0-disable
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfat_enable_uart_session(char enable);

/**
 * @brief set at command buffer size.
 *
 * @param[in] size: range is [1-4096], in bytes, default is 100
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfat_set_atcmd_buf_size(int size);

 /**
 * @brief parse at command.
 *
 * @param[in] cmd_line: a pointer to a buffer containing the AT command
 *			 cmd_len: the length, in bytes, of the data pointed to by the cmd_line parameter
 *			 rsp: a pointer to a buffer to receive AT command response data
 *			 len: the size of the rsp parameter
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfat_send_cmd(char *cmd_line,int cmd_len,char *rsp,int len);

/**
 * @brief parse at command response.
 *
 * @param[in] str: a pointer to a buffer containing the AT command response data
 *			 words: a pointer to a pointer buffer
 *			 size: the count of words parameter
 * @return[out] the count of words
 * @see None.
 * @note None.
 */
int HSF_API hfat_get_words(char *str,char *words[],int size);


#ifdef __cplusplus
}
#endif

#endif

