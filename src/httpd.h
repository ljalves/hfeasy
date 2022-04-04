#ifndef _HTTPD_H_
#define _HTTPD_H_

#include <hsf.h>

#define HTTPD_RECV_BUF_LEN	(1024*4)
#define HTTPD_MAX_PAGE_SIZE	(HTTPD_RECV_BUF_LEN - 128)

int USER_FUNC httpd_add_page(const char *url, void (*callback)(char *url, char *rsp), char *type);
int USER_FUNC httpd_arg_find(char *url, char *arg, char *val);

void USER_FUNC httpd_init(void);

#endif
