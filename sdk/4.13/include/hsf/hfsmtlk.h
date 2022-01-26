/* hfsmtlk.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _HF_SMTLK_H_
#define _HF_SMTLK_H_

#ifdef __cplusplus
extern "C"{
#endif


#include "hfwifi.h"

#define SMARTLINK_STATE_FIX_CHANNEL			(0)
#define SMARTLINK_STATE_RECV_DATA			(1)
#define SMARTLINK_STATE_FINISH				(2)

#define MAX_SMARTLINK_BUFFER_SIZE				(128)
#define MAX_RECORD_PAKCET_SEQNO				(16)

#define SMARTLK_PROTO_HFV3					(0)
#define SMARTLK_PROTO_HFV5					(1)
#define SMARTLK_PROTO_AIRKISS					(2)
#define SMARTLK_PROTO_HFV7					(4)

#define MAX_SCAN_LIST_SIZE						(100)

typedef struct _SMARTLNK_CONFIG
{
	uint32_t   u32State;
	uint16_t   u16SnifferedLengthBufferLen[14];
	uint16_t   u16SnifferedLengthBuffer[14][MAX_SMARTLINK_BUFFER_SIZE];
	uint32_t   u32SniferedPreMatchTime[14];
	uint16_t   u16AppSnifferedLengthBufferLen[14];
	uint16_t   u16AppSnifferedLengthBuffer[14][MAX_SMARTLINK_BUFFER_SIZE];
	uint8_t     u8FixChannel;
	uint8_t     u8FixChannelMC;
	uint32_t   u32BaseLength;
	uint8_t     u8AppMac[6];
	uint8_t     u8Bssid[6];
	uint8_t     u8EncryType;
	uint32_t   u32RecvPktsNumber;
	uint32_t   u32RecvAppPktsNumber;
	uint8_t     u8RecordSeqNoIndex;
	uint16_t   u2RecordSeqNo[MAX_RECORD_PAKCET_SEQNO];
	uint8_t     u8ProtocolType;
	//SMARTLINK_SEQ   sRecordSeqBuffer[MAX_SEQ_RECORD];
	//SMARTLINK_SEQ     szCurSeq[2];
	int32_t      nMaxPktLength;
	int32_t      nMinPktLength;
	void *pProtocolPrivateData;
	void (*reset)( void);
	int (*decode)(uint16_t /*data*/,bool /*fromap*/);
	uint8_t u8TkipAesUseTkip;
	uint8_t u8FixChannelAsFromAp;
}SMARTLINK_CONFIG;

extern SMARTLINK_CONFIG * g_smartlink_config;

#define SMTLK_STATE_NACT			0
#define SMTLK_STATE_SCAN			1
#define SMTLK_STATE_START			2
#define SMTLK_STATE_FIXCHNL		3
#define SMTLK_STATE_KEYSTART		4
#define SMTLK_STATE_KEYEND		5
#define SMTLK_STATE_STOPPED		6
#define SMTLK_STATE_APLINK			7
#define SMTLK_STATE_FINISHED		8
#define SMTLK_STATE_FAILED			9

#define HFSMTLK_FLAG_RECV_MGT_PKT			0x00000001
#define HFSMTLK_FLAG_CHNL_EXT_SCA			0x00010000
#define HFSMTLK_FLAG_CHNL_EXT_SCB			0x00020000

uint8_t HSF_IAPI hfsmtlk_crc8(uint8_t *ptr,int len);

uint8_t HSF_IAPI hfsmtlk_state_get(void);

void HSF_IAPI hfsmtlk_state_set(uint8_t state);

uint8_t HSF_IAPI hfsmtlk_state_init(void);

int HSF_IAPI hfsmtlk_main_loop( PWIFI_SCAN_RESULT_ITEM  scan_list,uint32_t  cnt);

void HSF_IAPI hfsmtlk_fix_channel_by_bssid(uint8_t *bssid, uint8_t *channel);

void HSF_IAPI hfsmtlk_finished_ok(char *key, int key_len, int type, uint8_t *apmac,char *ssid);

char HSF_IAPI hfsmtlk_get_enable_user_info_in_password(void);

int HSF_IAPI hfsmtlnk_check_uinfo_separators_char(char ch);

int HSF_IAPI hfsmtlk_success_time(void);

void HSF_IAPI hfsmtlk_d2d_parse(int type, unsigned char *data);

typedef int (*hfsmtlk_recv_callback_t)( void * /*pkt_data*/,uint32_t /*pkt_length*/,uint8_t /*channel*/);

typedef int (*hfsmtlk_main_callback_t)( PWIFI_SCAN_RESULT_ITEM /*ap_list*/,uint32_t /*ap_cnt*/);


#define HFSMTLK_SNIFFER_FILTER_RECV_MGM_FRAME					(0x01)
#define HFSMTLK_SNIFFER_FILTER_RECV_TO_DS_FRAME					(0x02)
#define HFSMTLK_SNIFFER_FILTER_RECV_FROM_DS_FRAME				(0x03)

/**
 * @brief start smartlink.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note this function will reboot.
 */
void HSF_API hfsmtlk_start(void);

/**
 * @brief stop sniffer wireless packet.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsmtlk_stop(void);

/**
 * @brief stop sniffer wireless packet and exit smartlink.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsmtlk_abort(void);

/**
 * @brief check whether or not in smartlink or smart aplink.
 *
 * @param[in] None
 * @return[out] 0-not in smartlink, 1-in smartlink
 * @see None.
 * @note None.
 */
int HSF_API hfsmtlk_is_start(void);

/**
 * @brief register callback function which will be called sniffered wireless packet.
 *
 * @param[in] recv_callback: a function pointer, called back when sniffered wireless packet
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfsmtlk_register(hfsmtlk_recv_callback_t recv_callback);

/**
 * @brief switch to specific wifi channel and capture direction.
 *
 * @param[in] channel: wifi channel, 0-13
 *			 second_channel: no use
 *			 flags: 
 * @return[out] None
 * @see None.
 * @note None.
 */
void HSF_API hfsmtlk_set_sniffer_filter(uint8_t channel,uint8_t second_channel, uint32_t flags);

/**
 * @brief get wifi info by index.
 *
 * @param[in] index: start from 0
 * @return[out] a pointer to PWIFI_SCAN_RESULT_ITEM, if NULL mean overflow
 * @see None.
 * @note None.
 */
PWIFI_SCAN_RESULT_ITEM HSF_API hfsmtlk_list_get_item(int index);

/**
 * @brief get wifi info by BSSID.
 *
 * @param[in] bssid: a pointer to wifi BSSID
 * @return[out] a pointer to PWIFI_SCAN_RESULT_ITEM, if NULL mean no match was found
 * @see None.
 * @note None.
 */
PWIFI_SCAN_RESULT_ITEM HSF_API hfsmtlk_list_find_ap_by_bssid(uint8_t *bssid);

/**
 * @brief get wifi info by SSID's crc8.
 *
 * @param[in] crc8: crc8
 * @return[out] a pointer to PWIFI_SCAN_RESULT_ITEM, if NULL mean no match was found 
 * @see None.
 * @note None.
 */
PWIFI_SCAN_RESULT_ITEM HSF_API hfsmtlk_list_find_ap_by_ssidcrc8(uint8_t crc8);

/**
 * @brief lock sniffer channel by 'channel' or 'bssid'.
 *
 * @param[in] channel: the channel number
  *			 bssid: a pointer to wifi BSSID
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note this function will find router info by bssid, and lock the channel of router, if not found, will lock 'channel'.
 */
int HSF_API hfsmtlk_lock_channel(uint8_t channel, uint8_t *bssid);

/**
 * @brief enable/disable user info in password and set user info separators, default is enable, separator is 0x1b.
 *
 * @param[in] enable: 1-enable, 0-disable
 *			 separators: user info separators
 *			 size: max size is 4 
 *			 need_separators: 1-callback with separators, 0-callback without separators, default is 1
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note must call this function before smartlink success, it is recommended to call in the function 'app_init'.
 */
int HSF_API hfsmtlk_enable_user_info_in_password(char enable, char *separators, int size, int need_separators);


#ifdef __cplusplus
}
#endif

#endif

