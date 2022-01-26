
#ifndef __SMARTLINKV7___H_H___
#define __SMARTLINKV7___H_H___

#define SMTLKV7_BASE_LEN 30
#define SMTLKV7_BASE_PKG_LEN 	60
#define SMTLKV7_MAX_DADA_LEN	512
#define SMTLKV7_MIN_FRAME_RECEIVED 2

void smartlinkv7_reset(void);
void smartlinkv7_init(void);
void smartlinkv7_free(void);
void smartlinkv7_finished_ok(uint8_t *data, uint8_t ssid_len, uint8_t pwd_len, uint8_t pmk_len, uint8_t uinfo_len, uint8_t *bssid);
int smartlinkv7_check(uint16_t length,bool bFromAp,uint8_t *dstMac,uint8_t channel, uint8_t *bssid);
uint8_t smartlinkv7_get_fix_channel(void);
int smartlinkv7_fix_channel(uint8_t channel, uint8_t *bssid);

#endif

