
#ifndef _HSFNVM_INTERNAL_H_H_
#define _HSFNVM_INTERNAL_H_H_
#include <hsf.h>

#define HSFNVM_SIZE					(500)

#define HSFNVM_MAGIC_NUM			(0x0102Faac)

#define HSFNVM_MAGIC_NUM_OFFSET				(0)
#define HSFNVM_MAGIC_NUM_SIZE				(4)

#define HSFNVM_RESET_REASON_OFFSET			(4)
#define HSFNVM_RESET_REASON_SIZE				(4)

#define HSFNVM_TIME_OFFSET					(8)
#define HSFNVM_TIME_SIZE						(8)

#define HSFNVM_SMARTLINK_FLAGS_OFFSET		(16)
#define HSFNVM_SMARTLINK_FLAGS_SIZE			(4)

#define HSFNVM_WPS_FLAGS_OFFSET				(20)
#define HSFNVM_WPS_FLAGS_SIZE					(4)

#define HSFNVM_RESET_TIMES_OFFSET			(24)
#define HSFNVM_RESET_TIMES_SIZE				(4)

#define HSFNVM_PWM_FLAGS_OFFSET				(28)
#define HSFNVM_PWM_STATE_OFFSET				(32)
#define HSFNVM_PWM_STATE_SIZE				(28)

#define HSFNVM_AIRKISS_RANDOM					(60)
#define HSFNVM_AIRKISS_RANDOM_SIZE			(1)
#define HSFNVM_SMARTLINK_PROTO				(61)
#define HSFNVM_SMARTLINK_PROTO_SIZE			(1)

#define HSFNVM_OTA_MODE						(62)
#define HSFNVM_OTA_MODE_SIZE					(1)

#define HSFNVM_RANDOM_NUM					(63)
#define HSFNVM_RANDOM_NUM_SIZE				(4)

#define HSFNVM_MDCH_MODE						(67)
#define HSFNVM_MDCH_MODE_SIZE				(1)


#define HSFNVM_USER_OFFSET					(256)


int HSF_IAPI hfnvm_write_int(uint16_t offset,int value);

int HSF_IAPI hfnvm_read_int(uint16_t offset);

int HSF_IAPI hfnvm_write_data(uint16_t offset,uint8_t *data,int size);

int HSF_IAPI hfnvm_read_data(uint16_t offset,uint8_t *data,int size);

void HSF_IAPI hfnvm_reset(void);

#endif



