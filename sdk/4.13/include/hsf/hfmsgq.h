/* hfmsgq.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_MSGQ_H_
#define _HF_MSGQ_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef int*  HFMSGQ_HANDLE;

#define MSGQ_DELAY_INFINITE			(0xFFFFFFFF)

/**
 * @brief create a message queue.
 *
 * @param[in] size: the total size of messages
 *			 item_size: the size of each message
 * @return[out] NULL-failed, other value is the msg handle
 * @see None.
 * @note this function will use about [2*size*item_size] RAM, one mutex and one semaphore.
 */
HFMSGQ_HANDLE HSF_API hfmsgq_create(uint32_t  size,uint32_t item_size );

/**
 * @brief destroy a message queue.
 *
 * @param[in] msgq: the handle of msg
 * @return[out] none
 * @see None.
 * @note None.
 */
void	HSF_API hfmsgq_destroy(HFMSGQ_HANDLE msgq);

/**
 * @brief send messages to message queue.
 *
 * @param[in] msgq: the specified message queue
 *			 msg: the message want to send, will send [item_size] size
 *			 timeout_ms: send timeout, in millisecond, if message queue is full, will wait 'timeout' or until send success, 'MSGQ_DELAY_INFINITE' will block until send success
 *			 flags: no use
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfmsgq_send(HFMSGQ_HANDLE msgq,void *msg,uint32_t timeout_ms,uint32_t flags);

/**
 * @brief recv messages from message queue.
 *
 * @param[in] msgq: the specified message queue
 *			 msg: a buffer to recv message, will recv [item_size] size
 *			 timeout_ms: recv timeout, in millisecond, MSGQ_DELAY_INFINITE will block until recv success
 *			 flags: no use
 * @return[out] HF_SUCCESS-successfully, other value is failed or timeout
 * @see None.
 * @note None.
 */
int HSF_API hfmsgq_recv(HFMSGQ_HANDLE msgq,void *msg,uint32_t timeout_ms,uint32_t flags);

/**
 * @brief send the specified size messages to message queue.
 *
 * @param[in] msgq: the specified message queue
 *			 msg: the message want to send
 *			 msg_len: the size want to send
 *			 timeout_ms: send timeout, in millisecond, if message queue is full, will wait 'timeout' or until send success, 'MSGQ_DELAY_INFINITE' will block until send success
 *			 flags: no use
 * @return[out] HF_SUCCESS-successfully, other value is failed
 * @see None.
 * @note None.
 */
int HSF_API hfmsgq_send_ex(HFMSGQ_HANDLE msgq,void *msg,int msg_len,uint32_t timeout_ms,uint32_t flags);

/**
 * @brief recv the specified size messages from message queue.
 *
 * @param[in] msgq: the specified message queue
 *			 msg: a buffer to recv message
 *			 msg_len: the size want to recv
 *			 timeout_ms: recv timeout, in millisecond, MSGQ_DELAY_INFINITE will block until recv success
 *			 flags: no use
 * @return[out] HF_SUCCESS-successfully, other value is failed or timeout
 * @see None.
 * @note None.
 */
int HSF_API hfmsgq_recv_ex(HFMSGQ_HANDLE msgq,void *msg,int msg_len,uint32_t timeout_ms,uint32_t flags);


#ifdef __cplusplus
}
#endif

#endif

