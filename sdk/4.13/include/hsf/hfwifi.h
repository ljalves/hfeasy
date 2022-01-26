/* hfwifi.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_WIFI_H__
#define _HF_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif


#define AUTH_TYPE_OPEN				0
#define AUTH_TYPE_SHARED			1
#define AUTH_TYPE_WPAPSK			2
#define AUTH_TYPE_WPA2PSK			3
#define AUTH_TYPE_WPAPSKWPA2PSK	4

#define ENC_TYPE_NONE				0
#define ENC_TYPE_WEP				1
#define ENC_TYPE_TKIP				2
#define ENC_TYPE_AES				3
#define ENC_TYPE_TKIPAES			4
#define ENC_TYPE_INVALID			255

#define WIFI_STATE_UNINIT			0
#define WIFI_STATE_START			1
#define WIFI_STATE_STOPPING			2
#define WIFI_STATE_STOPPED			3

#define HF_MAX_WIFI_SCAN_RESULT		50

extern int m2m_sta_linked;

typedef struct _WIFI_SCAN_RESULT_ITEM
{
	uint8_t auth;
	uint8_t encry;
	uint8_t channel;
	uint8_t rssi;
	char    ssid[32+1];
	uint8_t mac[6];
	int       rssi_dbm;
	int       sco;
}WIFI_SCAN_RESULT_ITEM,*PWIFI_SCAN_RESULT_ITEM;

typedef struct _WIFI_AP_JOIN_ITEM
{
	uint8_t mac[6];
	unsigned int ip;
}WIFI_AP_JOIN_ITEM,*PWIFI_AP_JOIN_ITEM;

typedef int (*hfwifi_scan_callback_t)(PWIFI_SCAN_RESULT_ITEM);
typedef int (*hfwifi_scan_callback_ex_t)(PWIFI_SCAN_RESULT_ITEM,void*);

void HSF_IAPI hfwifi_init(void);

int HSF_IAPI hfwifi_is_scanning(void);

int HSF_IAPI hfwifi_start_softap(char *ap_ssid, char *ap_key, int channel);

int HSF_IAPI hfwifi_tcpip_ps(int enable);

int HSF_IAPI hfwifi_power_save(int enable);

int HSF_IAPI hfwifi_wps_main(void);

int HSF_IAPI hfwifi_is_stopping(void);

int HSF_IAPI hfwifi_write_sta_mac_address(uint8_t *mac);

int HSF_IAPI hfwifi_find_best_signal_router(char *ssid, int auth, int encry, uint8_t *bssid);

int HSF_IAPI hfwifi_connect_ap_with_security(int enable);

/**
 * @brief launch a wifi scan operation.
 *
 * @param[in] p_callback: pass ssid info(scan result) to this callback one by one
 * @return[out] the number of wifi
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_scan(hfwifi_scan_callback_t p_callback);

/**
 * @brief launch a wifi scan operation.
 *
 * @param[in] p_callback: pass ssid info(scan result) to this callback one by one
 *			 ctx: a pointer to a variable to be passed to the p_callback
 *			 ch: range is 1-13, scan channel, 0 means 1-13
 *			 scan_time: totol scan time, in seconds, default is 3s
 * @return[out] the number of wifi
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_scan_ex(hfwifi_scan_callback_ex_t p_callback, void *ctx, unsigned char ch, unsigned char scan_time);

/**
 * @brief check whether or not connected to a AP.
 *
 * @param[in] None
 * @return[out] 0-not connected, 1-connected
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_sta_is_connected(void);

/**
 * @brief get the rssi of current connected AP.
 *
 * @param[in] None
 * @return[out] the rssi of percentage, such as: 70
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_sta_get_rssi(void);

/**
 * @brief get the rssi and dBm of current connected AP.
 *
 * @param[in] dBm: a pointer to dBm number, dBm is negative, such as: -60
 * @return[out] the rssi of percentage, such as: 70
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_sta_get_current_rssi(int *dBm);

/**
 * @brief transform dBm ro rssi.
 *
 * @param[in] dBm: dBm number, is negative, such as: -60
 * @return[out] the rssi of percentage, such as: 70
 * @see None.
 * @note conversion formula approximately equal to: rssi = (dBm+95) * 2
 */
int HSF_API hfwifi_transform_rssi(int rssi_dbm);

/**
 * @brief get the bssid of current connected AP.
 *
 * @param[in] bssid: a pointer to bssid
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_sta_get_current_bssid(uint8_t *bssid);

/**
 * @brief get the channel of current connected AP.
 *
 * @param[in] channel: a pointer to channel number
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_sta_get_current_channel(uint8_t *channel);

/**
 * @brief get the moudle's mac address in Station mode.
 *
 * @param[in] mac: a pointer to mac, such as: 0xac,0xcf,0x23,0xff,0x88,0x88
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_read_sta_mac_address(uint8_t *mac);

/**
 * @brief get the moudle's mac address in AP mode.
 *
 * @param[in] mac: a pointer to mac, such as: 0xac,0xcf,0x23,0xff,0x88,0x89
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_read_ap_mac_address(uint8_t *mac);

/**
 * @brief get joined sta info.
 *
 * @param[in] sta_list: store the scan result
 *			 num: max number of sta_list
 * @return[out] number of joined sta, <0 is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_get_ap_join_info(PWIFI_AP_JOIN_ITEM sta_list, unsigned int num);

/**
 * @brief deauth station.
 *
 * @param[in] mac: deauth mac
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_ap_deauth_sta(unsigned char *mac);

/**
 * @brief set module to wifi powersave mode.
 *
 * @param[in] enable: 1-enter low power mode, 0-exit low power mode
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note only effective in sta mode.
 */
int HSF_API hfwifi_powersave(int enable);

/**
 * @brief send 802.11 raw data.
 *
 * @param[in] data: the data buffer want send
 *			 len: the size of data buffer
 * @return[out] the size send out
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_send_rawdata(char* data, unsigned int len);

/**
 * @brief start wifi driver.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_start(void);

/**
 * @brief stop wifi driver.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfwifi_stop(void);

/**
 * @brief start WPS config wifi.
 *
 * @param[in] None
 * @return[out] None
 * @see None.
 * @note this function will reboot.
 */
void HSF_API hfwifi_wps_start(void);


#ifdef __cplusplus
}
#endif

#endif

