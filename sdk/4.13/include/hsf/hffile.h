/* hffile.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _HF_FILE_H_
#define _HF_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef void * HFFILE;

//begin of the file
#define HFFILE_SEEK_SET		0

//current postion of the file
#define HFFILE_SEEK_CUR		1

//end of the file
//#define HFFILE_SEEK_END	2

HFFILE* HSF_IAPI hffile_open(const char *name,const char *mode);

int HSF_IAPI hffile_close(HFFILE* file);

int HSF_IAPI hffile_read(void *buffer, int size, int count, HFFILE* stream);

int HSF_IAPI hffile_write(const void* buffer, int size, int count, HFFILE* stream);

int HSF_IAPI hffile_seek(HFFILE* stream, long offset, int fromwhere);

void HSF_IAPI hffile_flush(HFFILE* stream);

int HSF_IAPI hffile_fs_init(void);


/**
 * @brief get the size, in bytes, of the userbin.
 *
 * @param[in] None
 * @return[out] the size of userbin, in bytes
 * @see None.
 * @note userbin is a space similar to flash, no need erase before write action, and have a backup function.
 */
int HSF_API hffile_userbin_size(void);

/**
 * @brief write data to userbin.
 *
 * @param[in] offset: the relative address of userbin, start from 0
 *			 data: a pointer to data
 *			 len: the len of data
 * @return[out] the length of write success
 * @see None.
 * @note None.
 */
int HSF_API  hffile_userbin_write(uint32_t offset,char *data,int len);

/**
 * @brief read data from userbin.
 *
 * @param[in] offset: the relative address of userbin, start from 0
 *			 data: a pointer to data
 *			 len: the len of data
 * @return[out] the length of read success
 * @see None.
 * @note None.
 */
int HSF_API  hffile_userbin_read(uint32_t offset,char *data,int len);

/**
 * @brief set the userbin area to zero.
 *
 * @param[in] None
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hffile_userbin_zero(void);

/**
 * @brief write data to userbin,Internal use for socka mqtt config.
 *
 * @param[in] offset: the relative address of userbin, start from 0
 *			 data: a pointer to data
 *			 len: the len of data
 * @return[out] the length of write success
 * @see None.
 * @note None.
 */
int  HSF_IAPI  hffile_user_write(uint32_t offset,char *data,int len);

#ifdef __cplusplus
}
#endif

#endif

