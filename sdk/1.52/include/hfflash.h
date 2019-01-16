
/* hfflash.h
 *
 * Copyright (C) 2013 High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_FLASH_H_H_H_
#define _HF_FLASH_H_H_H_


int HSF_API hfuflash_erase_page(uint32_t addr, int pages);

int HSF_API hfuflash_write(uint32_t addr, char *data, int len);

int HSF_API hfuflash_read(uint32_t addr, char *data, int len);

int HSF_IAPI hfflash_copy(uint32_t dstaddr,uint32_t srcaddr,uint32_t len);

void flash_lock(void);
void flash_unlock(void);
int  flash_write(unsigned int addr, char *data, int len, int no_used);
int  flash_erase_page(unsigned int addr, int pages);
int  flash_read(unsigned int addr, char *data, int len, int no_used);

int  flash_write_no_lock(unsigned int addr, char *data, int len, int no_used);

int  flash_erase_page_no_lock(unsigned int addr, int pages);

#define HFFLASH_PAGE_SIZE			(4*1024)

#endif

