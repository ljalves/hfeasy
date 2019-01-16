#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <hsf.h>

int USER_FUNC httpd_add_page(const char *url, void *callback);
int USER_FUNC httpd_arg_find(char *url, char *arg, char *val);

void USER_FUNC httpd_init(void);

#endif
