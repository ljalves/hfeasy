#ifndef __AIRKISS___H_H___
#define __AIRKISS___H_H___

#ifdef __cplusplus
extern "C" {
#endif

//#define HF_AIRKISS_CODE
#define RDA_AIRKISS_CODE

#ifdef HF_AIRKISS_CODE

#define AIRKISS_STATE_FIX_CHANNEL			(0)
#define AIRKISS_STATE_RECV_MAGIC_CODE	(1)
#define AIRKSIS_STATE_RECV_DATA			(2)
#define AIRKISS_STATE_FINISH				(3)

#define MAX_AIRKISS_BUFFER_SIZE			(128)
#define MAX_RECORD_PAKCET_SEQ				(12)

#define MAX_AIRKISS_SEQ_NUM               		(24)
#define MAX_AIRKISS_RECORD					(4)
#define MAX_AIRKISS_SEQ_LEN				(6)
#define MAX_AIRKISS_SEQ_DATA_LEN			(4)

#define MAX_PREFIX_CODE_BUFFER_SIZE			(16)

typedef struct _AIRKISS_SEQUENCE
{
	uint8_t  u8Crc8;
	uint8_t  u8DataLen;
	bool     bValid;
	uint16_t  u16Times;
	uint8_t  u8Data[4];
}AIRKISS_SEQUENCE;


#define IS_PREFIX_CODE(__len)		((!((__len)&0xFF80)/*&&((__len)&0x70)*/))
#define IS_SEQ_HDR_FLAG(__len)			(!((__len)&0x100)&&((__len)&0x80))
#define IS_SEQ_DATA_FLAG(__len)			((__len)&0x100)
#define IS_SEQ_DATA(__len)			(((__len)&0x180)&&(((__len)&0xFE00)==0))

#define DATA_TYPE_PREFIXCODE		(0)
#define DATA_TYPE_SEQHDR		       (1)
#define DATA_TYPE_SEQDATA			(2)

typedef struct _AIRKISS_CONFIG
{
	uint8_t     u8PreRecvDataType[2];
	uint8_t     u8SeqRecvIndex[2];
	uint8_t     szCurSeqData[2][6];
	uint8_t     szSeqDataRecodeIndex[MAX_AIRKISS_SEQ_NUM];
	AIRKISS_SEQUENCE szSeqData[MAX_AIRKISS_SEQ_NUM][MAX_AIRKISS_RECORD];
	uint8_t     szSsidCrcRecvFlags[2];
	uint16_t   u8SsidCrcTemp[2];
	uint8_t     szSsidCrcCnt;
	uint8_t   u8SsidCrc[MAX_AIRKISS_RECORD*2];
	uint8_t     szSsidLengthRecvFlags[2];
	uint16_t   u8SsidLengthTemp[2];
	uint8_t     szSsidLengthCnt;
	uint8_t   u8SsidLength[MAX_AIRKISS_RECORD*2];	
	uint8_t     szPswLengthRecvFlags[2];
	uint16_t   u8PswLengthTemp[2];
	uint32_t     szPswLengthCnt;
	uint8_t   	 u8PswLength[MAX_AIRKISS_RECORD*2];
	uint8_t     szPswCrcBufferIndex[2];
	uint8_t     szPswCrc8[2][MAX_AIRKISS_RECORD];
	uint8_t     szSsidCrcBufferIndex[2];
	uint8_t     szSsidCrc8[2][MAX_AIRKISS_RECORD];
	uint8_t     u8EncryType;
 	int    nPrefixCodeCnt;
 	uint32_t   szPrefixCode[MAX_PREFIX_CODE_BUFFER_SIZE];
	int   nMatchTimes;
}AIRKISS_CONFIG;

void airkiss_reset(void);
int airkiss_check(uint16_t length,bool bFromAp,uint16_t seqno,int index,uint8_t channel);
int airkiss_fix_channel(uint8_t channel);
void airkiss_init(void);
void airkiss_free(void);
#endif

#ifdef RDA_AIRKISS_CODE

//#define AIRKISS_PRINT(fmt, ...) HF_Debug(DEBUG_AIRKISS, fmt, ##__VA_ARGS__)
#define AIRKISS_PRINT(fmt, ...)

typedef enum {
    AIRKISS_STATE_STOPED = 0,
    AIRKISS_STATE_IDLE,
    AIRKISS_STATE_SRC_LOCKED,
    AIRKISS_STATE_MAGIC_CODE_COMPLETE,
    AIRKISS_STATE_PREFIX_CODE_COMPLETE,
    AIRKISS_STATE_COMPLETE
} AIR_KISS_STATE;

#define MAX_GUIDE_RECORD    4
typedef struct {
    unsigned short  length_record[MAX_GUIDE_RECORD + 1];//tods
    unsigned short  length_record_fromds[MAX_GUIDE_RECORD + 1];//fromds
    unsigned short  length_record_ldpc[MAX_GUIDE_RECORD + 1];
    unsigned short  length_11m_from_ds[MAX_GUIDE_RECORD + 1];
} guide_code_record;

#define MAX_MAGIC_CODE_RECORD    4
typedef struct {
    unsigned short record[MAX_MAGIC_CODE_RECORD + 1];
} magic_code_record;
#if 0
#define MAX_PREFIX_CODE_RECORD    4
typedef struct {
    unsigned short record[MAX_PREFIX_CODE_RECORD + 1];
} prfix_code_record;
#endif
#define MAX_SEQ_CODE_RECORD    6
typedef struct {
    unsigned short record[MAX_SEQ_CODE_RECORD + 1];
} seq_code_record;

#define PASSWORD_MAX_LEN          32
#define ESSID_MAX_LEN             32
#define USR_DATA_BUFF_MAX_SIZE    (PASSWORD_MAX_LEN + 1 + ESSID_MAX_LEN)
#define AIRKISS_RESPON_PORT       10000

union airkiss_data {
    guide_code_record guide_code;
    magic_code_record magic_code;
    //prfix_code_record prefix_code;
    seq_code_record   seq_code;
};

typedef struct {
    char* pwd;
    char* ssid;
    unsigned char pswd_len;
    unsigned char ssid_len;
    unsigned char random_num;
    unsigned char ssid_crc; //reserved used as ssid_crc
    //above is airkiss_context_t
    unsigned char usr_data[USR_DATA_BUFF_MAX_SIZE];
    AIR_KISS_STATE airkiss_state;
    unsigned char src_mac[6];
    unsigned char need_seq;
    unsigned char base_len;
    unsigned char total_len;
    unsigned char total_len_confirm;
    unsigned char pswd_lencrc;
    unsigned char recv_len;
    unsigned short seq_success_map;
    unsigned short seq_success_map_cmp;
    char _11m_fromds;
    char ldpc;
    char to_ds;
    union airkiss_data data;
} wland_airkiss;

/*
 * TODO: support AIRKISS_ENABLE_CRYPT
 */
#ifndef AIRKISS_ENABLE_CRYPT
#define AIRKISS_ENABLE_CRYPT 0
#endif

typedef struct {
    unsigned char dummyap[24];
} airkiss_context_t;

typedef enum {
    AIRKISS_STATUS_CONTINUE = 0,

    /* wifi channel is locked */
    AIRKISS_STATUS_CHANNEL_LOCKED = 1,

    /* get result success*/
    AIRKISS_STATUS_COMPLETE = 2

} airkiss_status_t;

void airkiss_reset(void);
int airkiss_recv(const void* frame, unsigned short length);
void airkiss_init(void);
void airkiss_free(void);
void airkiss_clear_each_channel(void);
#endif

/*
 * airlink definition
 */
#define AIRLINK_UDP_PORT		12476
#define DEFAULT_AIRLINK_OUTLEN	200
#define DEFAULT_AIRLINK_NOTIF_TIMER	5000 //5s

typedef enum
{
	/* 提供的数据缓冲区长度不足 */
	AIRKISS_LAN_ERR_OVERFLOW = -5,

	/* 当前版本不支持的指令类型 */
	AIRKISS_LAN_ERR_CMD = -4,

	/* 打包数据出错 */
	AIRKISS_LAN_ERR_PAKE = -3,

	/* 函数传递参数出错 */
	AIRKISS_LAN_ERR_PARA = -2,

	/* 报文数据错误 */
	AIRKISS_LAN_ERR_PKG = -1,

	/* 报文格式正确，但是不需要设备处理的数据包 */
	AIRKISS_LAN_CONTINUE = 0,

	/* 接收到发现设备请求数据包 */
	AIRKISS_LAN_SSDP_REQ = 1,

	/* 数据包打包完成 */
	AIRKISS_LAN_PAKE_READY = 2


} airkiss_lan_ret_t;


typedef enum
{
	AIRKISS_LAN_SSDP_REQ_CMD = 0x1,
	AIRKISS_LAN_SSDP_RESP_CMD = 0x1001,
	AIRKISS_LAN_SSDP_NOTIFY_CMD = 0x1002
} airkiss_lan_cmdid_t;

int start_airlink(const char *type, const char *id);
void stop_airlink();
int airkiss_lan_pack(airkiss_lan_cmdid_t cmd, u8_t* type, u8_t* id,void* data, u16_t* len);
int airkiss_lan_recv(const void* data, u16_t len);

#ifdef __cplusplus
}
#endif

#endif

