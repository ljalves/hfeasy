/*! \file wm-tls.h
 *  \brief TLS API's
 *
 *  Copyright 2008-2012, Marvell International Ltd.
 */

#ifndef __WM_TLS_H__
#define __WM_TLS_H__


#ifdef CONFIG_ENABLE_TLS

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The tls_init_config_t paramter in the tls_session_init function takes a
 * pointer to the certificate buffer. The certificate buffer can either
 * have a single (or a chain of) X.509 PEM format certificate(s).
 * If the server uses a self-signed certificate, the same certificate needs
 * to be present in this buffer. If the server uses a CA signed
 * certificate, you need to have either of the following:
 * - the root CA certificate only
 * - the chain of certificates (entire or partial starting with the Root CA
 * certificate)
 * Note: The system time needs to be set correctly for successful
 * certificate validation.
 */

typedef int tls_handle_t;

typedef enum {
	/* TLS server mode */
	/* If this flag bit is zero client mode is assumed.*/
	TLS_SERVER_MODE = 0x01,
	TLS_CHECK_CLIENT_CERT = 0x02,

	/* TLS Client mode */
	TLS_NO_SERVER_CERT = 0x00,
	TLS_CHECK_SERVER_CERT = 0x04,
} tls_session_flags_t;

/*
 * Practical observation: It was observed that when the certificate is sent
 * its size send should be 1 less that sizeof(array in which it is
 * present.). Needs to be checked again.
 */

typedef struct {
	int flags;
	union {
		struct {
			/* 
			 * Needed if the server mandates verification of client
			 * certificate. Otherwise set to NULL 
			 */
			const unsigned char *client_cert;
			/* Size of client_cert */
			int client_cert_size;
			/* 
			 * Needed if the client wants to verify
			 * certificate set by the server. Recommended. Otherwise
			 * set to NULL
			 */
			const unsigned char *server_cert;
			/* Size of server_cert */
			int server_cert_size;
		} client;
		struct {
			/* Mandatory. Will be sent to the client */
			const unsigned char *server_cert;
			/* Size of server_cert */
			int server_cert_size;
			/* 
			 * Server private key. Mandatory.
			 * For the perusal of the server 
			 */
			const unsigned char *server_key;
			/* Size of server_key */
			int server_key_size;
			/* 
			 * Needed if the server wants to verify client
			 * certificate. Otherwise set to NULL.
			 */
			const unsigned char *client_cert;
			/* Size of client_cert */
			int client_cert_size;
		}server;
	} tls;
} tls_init_config_t;

/**
 * Init TLS library
 *
 * @return Standard WMSDK return codes.
 */
int tls_lib_init(void);

/**
 * Start an TLS session on the given socket.
 *
 * @param[out] h Pointer to the handle,
 * @param[in] sockfd A socket descriptor on which 'connect' is called
 * before. This parameter is not to be given for TLS server init. The
 * client socket is to be 
 * @param[in] cfg TLS configuration request structure. To be allocated and
 * populated by the caller.
 *
 * @return Standard WMSDK return codes.
 */
int tls_session_init(tls_handle_t *h, int sockfd, const tls_init_config_t *cfg);


int tls_server_set_clientfd(tls_handle_t h, int clientfd);

/**
 * Send data over and existing TLS connection.
 *
 * @param[in] h Handle returned from a previous call to TLS_session_init
 * @param[in] buf Buffer to send.
 * @param[in] len Length to write
 *
 * @return Amount data written to the network
 */
int tls_send(tls_handle_t h, const void *buf, int len);

/**
 * Receive data from an existing TLS connection.
 *
 * @param[in] h Handle returned from a previous call to TLS_session_init
 * @param[in] buf Buffer to receive data.
 * @param[in] max_len Max length of the buffer.
 *
 * @return Amount data read from the network
 */
int tls_recv(tls_handle_t h, void *buf, int max_len);

/**
 * Close the tls connection.
 *
 * This function will not close the socket. It will only terminate TLS
 * connection over it.
 *
 * @param[in,out] h Handle returned from a previous call to
 *                                  TLS_session_init. Will set it to NULL
 *                                  before returning.
 */
void tls_close(tls_handle_t *h);

#ifdef __cplusplus
}
#endif

#endif

#define	isdigit(x)	((x) >= '0' && (x) <= '9')
#define	isupper(x)	(((unsigned)(x) >= 'A') && ((unsigned)(x) <= 'Z'))
#define	islower(x)	(((unsigned)(x) >= 'a') && ((unsigned)(x) <= 'z'))
#define	isalpha(x)	(isupper(x) || islower(x))
#define	toupper(x)	(isupper(x) ? (x) : (x) - 'a' + 'A')

#define net_socket(domain, type, protocol) socket(domain, type, protocol)
#define net_select(nfd, read, write, except, timeout) \
			select(nfd, read, write, except, timeout)
#define net_bind(sock, addr, len) bind(sock, addr, len)
#define net_listen(sock, backlog) listen(sock, backlog)
#define net_accept(sock, addr, len) accept(sock, addr, len)
#define net_shutdown(c, b) shutdown(c, b)
#define net_connect(sock, addr, len) connect(sock, addr, len)
#define net_read(sock, data, len) read(sock, data, len)
#define net_write(sock, data, len) write(sock, data, len)
#ifdef WIN32
#define os_thread_sleep(_ms_)	Sleep(_ms_)
#define os_msec_to_ticks(_ms_)	((_ms_))
#define net_get_sock_error(_fd)	WSAGetLastError()
#define net_close(c) closesocket((c))
#else
#define os_thread_sleep(_10ms_)	msleep(_10ms_)
#define os_msec_to_ticks(_ms_)	((_ms_))
#define net_get_sock_error(_fd)	(errno)
#define net_close(c) close((c))
#endif
#define os_mem_alloc(_size)		malloc(_size)
#define os_mem_free(_data)		free(_data)
#define os_mem_realloc(_mem,_newsize)  realloc(_mem,_newsize)


#ifndef ASSERT
#define ASSERT(_cond_)
#endif

#endif  /* __WM_TLS_H__ */
