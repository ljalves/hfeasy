#ifndef __SSL_COMMON_H__
#define __SSL_COMMON_H__

#include <cyassl/openssl/ssl.h>
#include "hsf.h"
#define UNUSED_PARAM 0

#ifdef __cplusplus
extern "C" {
#endif

int ssl_tcp_accept(int *sockfd, int *clientfd,
		   const char *server, uint16_t port);
void SetDH(SSL *ssl);
void tcp_set_nonblocking(int *sockfd);
int ssl_tcp_connect(int *sockfd, const char *ip, uint16_t port);
void showPeer(SSL *ssl);
void close_tcp_sockets(int *sock_fds, int count);
void close_ssl_connections(SSL **ssl, int count);
void free_ssl_connections(SSL **ssl, int count);

#ifdef __cplusplus
}
#endif


//#define CONFIG_TLS_DEBUG



#ifdef CONFIG_TLS_DEBUG
#define tls_d(...)				HF_Debug(DEBUG_SSL,"[TLS debug Func:%s Line:%d]",__FUNCTION__,__LINE__); \
													HF_Debug(DEBUG_SSL,__VA_ARGS__)

#define tls_e(...)				HF_Debug(DEBUG_SSL,"[TLS error Func:%s Line:%d]",__FUNCTION__,__LINE__); \
													HF_Debug(DEBUG_SSL,__VA_ARGS__)

#define tls_w(...)				HF_Debug(DEBUG_SSL,"[TLS warning Func:%s Line:%d]",__FUNCTION__,__LINE__); \
													HF_Debug(DEBUG_SSL,__VA_ARGS__)
#else
#define tls_e(_fmt_,...)
#define tls_w(_fmt_...)
#define tls_d(_fmt_,...)
#endif


#endif  /* __SSL_COMMON_H__ */
