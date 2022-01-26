/* hfuart.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_UART_H_
#define _HF_UART_H_

#ifdef __cplusplus
extern "C"{
#endif


typedef void * hfuart_handle_t;

#define HFUART0			(hfuart_handle_t)(1)
#define HFUART1			(hfuart_handle_t)(2)

/**
 * @brief open UART.
 *
 * @param[in] uart_id: UART id, as follows:
 * ==========================================
 * | 0             |  data UART             |
 * |----------------------------------------|
 * | 1             |  debug UART            |
 * ==========================================
 * @return[out] the handle of UART, NULL is open failed
 * @see None.
 * @note None.
 */
hfuart_handle_t HSF_API hfuart_open(int uart_id);

/**
 * @brief send data through UART.
 *
 * @param[in] huart: the specified uart handle
 *			 data: a pointer to a buffer to send data
 *			 bytes: the length, in bytes, of the data pointed to by the data parameter
 *			 timeouts: send timeout, in millisecond
 * @return[out] the length of send successfully
 * @see None.
 * @note None.
 */
int HSF_API hfuart_send(hfuart_handle_t huart, char *data, uint32_t bytes, uint32_t timeouts);

/**
 * @brief recv data through UART.
 *
 * @param[in] huart: the specified uart handle
 *			 data: a pointer to a buffer to recv data
 *			 bytes: the length, in bytes, of the data pointed to by the data parameter
 *			 timeouts: recv timeout, in millisecond
 * @return[out] the length of recv successfully
 * @see None.
 * @note None.
 */
int HSF_API hfuart_recv(hfuart_handle_t huart, char *data, uint32_t bytes, uint32_t timeouts);

/**
 * @brief close UART.
 *
 * @param[in] huart: the handle of UART
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfuart_close(hfuart_handle_t huart);

/**
 * @brief check uart TX fifo is empty.
 *
 * @param[in] huart: the specified uart handle
 * @return[out] 0-is not empty, 1-is empty
 * @see None.
 * @note None.
 */
int HSF_API hfuart_txfifo_is_empty(hfuart_handle_t huart);

/**
 * @brief config UART.
 *
 * @param[in] huart: the handle of UART
 *			 baudrate: baud rate, range is [1200-4000000], support non-standard baud rate
 *			 parity: parity bit, support none,odd,even
 *			 databits: data bit, support 5,6,7,8
 *			 stopbits: stop bit, support 1
 *			 fc: flow control
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfuart_config(hfuart_handle_t huart, int baudrate, ENCOMPARITY_E parity, ENCOMBITS_E databits, ENCOMSTOPBITS_E stopbits, ENCOMUARTCTL_E fc);


#ifdef __cplusplus
}
#endif

#endif

