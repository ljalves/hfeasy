#ifndef _RDA5981_OTA_H_
#define _RDA5981_OTA_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#if 0
#define VERSION_SZ          24
typedef struct firmware_info
{
    uint32_t magic;
    uint8_t  version[VERSION_SZ];

    uint32_t addr;
    uint32_t size;
    uint32_t crc32;
}firmware_info_t;

typedef struct image_header
{
    uint16_t magic;
    uint8_t  encrypt_algo;
    uint8_t  resv[1];

    uint8_t  version[VERSION_SZ];

    uint32_t crc32;
    uint32_t size;
}image_header_t;
#endif
extern const unsigned int RDA_FW_INFO_ADDR;
extern const unsigned int RDA_UPGRADE_ADDR;

//#define RDA5981_PARTITION_0_ADDR 0x18010000
//#define RDA5981_PARTITION_1_ADDR 0x18080000
//#define PARTION_SIZE   (RDA5981_PARTITION_1_ADDR - RDA5981_PARTITION_0_ADDR)

/*
 * function: get the current available partion addr
 * return: the available partition address, NULL error
 */
//unsigned int rda5981_get_next_partition_addr(unsigned int part0_addr, unsigned int part1_addr);

/*
 * function: start to wirte a partition. this func will erase given flash region
 * @addr:    partition start address, must be 4k alignment
 * @img_len: length of image getted from OTA server, must be 4k alignment
 * return: 0:success, else:fail
 */
int rda5981_write_partition_start(unsigned int addr, unsigned int img_len);

/*
 * function: write image to flash, without erase.
 * the write region must be inside of the area given by func rda5981_write_partition_start
 * the write region must be in order, otherwise the end function will return crc error.
 * the maximum length could be write once time is 0x1000
 * @offset: offset from image inital position, must be 1k alignment
 * @buf: data to be written
 * @len: buffer len, max #0x1000, must be 1k alignment
 * return: 0:success, else:fail
 */
int rda5981_write_partition(unsigned int offset, const unsigned char *buf, unsigned int len);

/*
 * function: end of writing partition
 * return: 0:crc32 check success, else:fail
 */
int rda5981_write_partition_end(void);

#ifdef __cplusplus
}
#endif

#endif /*_RDA5981_OTA_H_*/
