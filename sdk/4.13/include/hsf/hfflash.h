/* hfflash.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_FLASH_H_
#define _HF_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif


int HSF_IAPI hfflash_init(void);

int HSF_IAPI flash_write(unsigned int addr, unsigned char *data, int len, int no_used);

int HSF_IAPI flash_erase_write(unsigned int addr, unsigned char *data, int len, int no_used);

int HSF_IAPI flash_erase_page(unsigned int addr, int pages);

int HSF_IAPI flash_read(unsigned int addr, unsigned char *data, int len, int no_used);

int HSF_IAPI hfflash_copy(unsigned int dstaddr, unsigned int srcaddr,int len);

int HSF_IAPI flash_enable(int enable);

#define hfflash_write					flash_write
#define hfflash_erase_write			flash_erase_write
#define hfflash_erase_page			flash_erase_page
#define hfflash_read					flash_read
#define hfflash_enable				flash_enable


#define HFFLASH_PAGE_SIZE			(4*1024)

/**
 * @brief get the size of uflash, in bytes.
 *
 * @param[in] None
 * @return[out] the size of uflash, in bytes
 * @see None.
 * @note None.
 */
int HSF_API hfuflash_size(void);

/**
 * @brief erase uflash.
 *
 * @param[in] addr: the address of flash, start from 0
 *			 pages: the number of pages to be erased, must <= 8, one page is 4096 bytes
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfuflash_erase_page(uint32_t addr, int pages);

/**
 * @brief write data to uflash.
 *
 * @param[in] addr: the address of flash, start from 0
 *			 data: a pointer to data
  *			 len: the len of data
 * @return[out] the length of write success
 * @see None.
 * @note None.
 */
int HSF_API hfuflash_write(uint32_t addr, char *data, int len);

/**
 * @brief read data from uflash.
 *
 * @param[in] addr: the address of flash, start from 0
 *			 data: a pointer to data
 *			 len: the len of data
 * @return[out] the length of read success
 * @see None.
 * @note None.
 */
int HSF_API hfuflash_read(uint32_t addr, char *data, int len);


#ifdef __cplusplus
}
#endif

#endif

