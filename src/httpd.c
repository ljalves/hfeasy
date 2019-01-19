
#include "hfeasy.h"

#define HTTPD_MAX_PAGES		10


struct httpd_page {
	const char *url;
	void (*callback) (char *url, char *rsp);
} httpd_pages[HTTPD_MAX_PAGES] = {
	{ .url = NULL },
};

static void USER_FUNC convert_ascii(char *str)
{
	char *in, *out, c, s[3] = {0, 0, 0};
	
	in = str;
	out = str;
	
	while (*in != '\0') {
		if (*in == '%') {
			memcpy(s, in + 1, 2);
			c = strtol(s, NULL, 16);
			*(out++) = c;
			in += 3;
		} else {
			*(out++) = *(in++);
		}
	}
	*out = '\0';
}

int USER_FUNC httpd_arg_find(char *url, char *arg, char *val)
{
	char *a, *s;
	char buf[256];
	
	a = strchr(url, '?');
	if (a == NULL)
		return 0;

	strcpy(buf, a + 1);
	convert_ascii(buf);
	a = strtok(buf, "&");
	while (a != NULL) {
		s = strchr(a, '=');
		if (s != NULL)
			*s = '\0';
		if (strcmp(arg, a) == 0) {
			if (s == NULL || val == NULL)
				return 2;
			strcpy(val, s + 1);
			return 1;
		}
		a = strtok(NULL, "&");
	}
	return 0;
}


int USER_FUNC httpd_add_page(const char *url, void (*callback)(char *url, char *rsp))
{
	int i;
	
	for (i = 0; i < HTTPD_MAX_PAGES; i++) {
		if (httpd_pages[i].url == NULL)
			break;
	}
	if (i == HTTPD_MAX_PAGES)
		return -1;
	
	httpd_pages[i].url = url;
	httpd_pages[i].callback = callback;
	
	return 0;
}


static int USER_FUNC httpd_callback(char *url, char *rsp)
{
	struct httpd_page *p = httpd_pages;
	char buf[50], *a;
	
	while (p->url != NULL) {
		strncpy(buf, url, 50);
		buf[49] = '\0';
		a = strchr(buf, '?');
		if (a != NULL)
			*a = '\0';
		if (strcmp(p->url, buf) == 0) {
			p->callback(url, rsp);
			return 0;
		}
		p++;
	}
	return -1;
}


void USER_FUNC httpd_init(void)
{
	/* register url handler callback */
	if (hfhttpd_url_callback_register(httpd_callback, 0) != HF_SUCCESS)
		u_printf("error registering url callback\r\n");
}
