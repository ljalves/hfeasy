/* hfspi.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_SPI_H_
#define _HF_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif


#define HFSPI_MODE_CPOL0_CPHA0		(0)
#define HFSPI_MODE_CPOL1_CPHA0		(2)

/**
 * @brief init SPI master mode.
 *
 * @param[in] mode: SPI mode, (clock polarity, phase, and shift direction)
 *			 clk: SPI frequency, 312.5K-20M, in Hz
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None
 * @note None
 */
int HSF_API hfspi_master_init(int mode, int clk);

/**
 * @brief SPI master send data.
 *
 * @param[in] buf: send data buf
 *			 length: send data length
 * @return[out] >0 - send data length, <0 - send fail
 * @see None
 * @note None
 */
int HSF_API hfspi_master_send_data(unsigned char* buf, int length);

/**
 * @brief SPI master recv data.
 *
 * @param[in] buf: recv data buf
 *			 length: recv data length
 * @return[out] >0 - recv data num, <0 - recv fail
 * @see None.
 * @note None
 */
int HSF_API hfspi_master_recv_data(unsigned char* buf, int length);

/**
 * @brief free SPI master.
 *
 * @param[in] none
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None
 */
int HSF_API hfspi_master_free(void);

/**
 * @brief set SPI CS low.
 *
 * @param None
 * @return None
 * @see None
 * @note None
 */
void HSF_API hfspi_cs_low(void);

/**
 * @brief set SPI CS high.
 *
 * @param None
 * @return None
 * @see None
 * @note None
 */
void HSF_API hfspi_cs_high(void);


#ifdef __cplusplus
}
#endif

#endif

