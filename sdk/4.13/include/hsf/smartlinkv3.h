
#ifndef __SMARTLINKV3___H_H___
#define __SMARTLINKV3___H_H___


#define SMARTLINK_START_KEY			'\r'
#define SMARTLINK_END_KEY				'\n'

// for smartlink
#define SMARTLINK_PREFIX_CODE0		(76)
//#define SMARTLINK_PREFIX_CODE1		(40)

#define BASE_LEN_MIN			100
#define BASE_LEN_MAX			200


#define MAX_SMARTLINK_PSW_SEQ_SIZE		(64)
#define MAX_SMARTLINK_PSW_SEQ_RECORD	(4)
#define MAX_PREFIX_CODE_BUFFER_SIZE		(32)

typedef struct _SMARTLINK_SEQ_V3
{
	uint8_t u8StartKeyCnts;
	uint8_t u8EndKeyCnts;
	uint8_t u8Length;
	uint8_t u8Data[MAX_SMARTLINK_PSW_SEQ_SIZE];
	uint8_t u8SeqVerifyCnts;
	uint8_t u8SeqVerify[16];
}SMARTLINK_SEQ_V3;

typedef struct _SMARTLNK_CONFIG_V3
{
 	int    nPrefixCodeCnt;
 	uint32_t   szPrefixCode[MAX_PREFIX_CODE_BUFFER_SIZE];
	uint8_t     u8RecordPswSeqDataBufferIndex;
	SMARTLINK_SEQ_V3   sRecordPswSeqData[MAX_SMARTLINK_PSW_SEQ_RECORD];
}SMARTLINK_CONFIG_V3;

void smartlinkv3_init(void);
void smartlinkv3_free(void);
void smartlinkv3_reset(void);
int smartlinkv3_check(uint16_t length,bool bFromAp,uint16_t seqno,int index,uint8_t channel);
int smartlinkv3_fix_channel(uint8_t channel);

#endif

