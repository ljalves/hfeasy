#ifndef _WLAND_OTA_H_
#define _WLAND_OTA_H_
#include "wland_types.h"
#include "rda_ccfg_api.h"

extern u32 flash_size;

#define CRC32_TABLE_ADDR 0xbb5c
#define CRC32_ADDR 0x8dff//u32 crc32(const u8 *p, size_t len)

#define CRC32_TABLE_ADDR_4 0xbbd8
#define CRC32_ADDR_4 0x8e33//u32 crc32(const u8 *p, size_t len)

static inline unsigned int bootrom_crc32(unsigned char *p, unsigned int len)
{
    unsigned int func = CRC32_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = CRC32_ADDR_4;
    }
    return ((unsigned int(*)(unsigned char *, unsigned int))func)(p, len);
}

#endif /*_WLAND_OTA_H_*/
