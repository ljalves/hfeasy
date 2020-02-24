
#include "hfeasy.h"

#define HTTPD_MAX_PAGES		10

static const char *http_header = "HTTP/1.1 200 OK\r\n"\
"Server: HTTPD\r\n"
"Connection: close\r\n\r\n";


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
	char *r = rsp;
	
	while (p->url != NULL) {
		strncpy(buf, url, 50);
		buf[49] = '\0';
		a = strchr(buf, '?');
		if (a != NULL)
			*a = '\0';
		if (strcmp(p->url, buf) == 0) {
			strcpy(r, http_header);
			r += strlen(http_header);
			p->callback(url, r);
			u_printf("'%s' size=%d\r\n", url, strlen(rsp));
			return 0;
		}
		p++;
	}
	return -1;
}


static const char *css_page = "* {font-family:sans-serif; font-size:12pt;}\r\n"\
  "h1 {font-size:16pt; color:black;}\r\n"\
  "h6 {font-size:10pt; color:black; text-align:center;}\r\n"\
  ".button-menu {background-color:#ffffff; color:blue; margin: 10px; text-decoration:none}\r\n"\
  ".button-link {padding:5px 15px; background-color:#0077dd; color:#fff; border:solid 1px #fff; text-decoration:none}\r\n"\
  ".button-menu:hover {background:#ddddff;}\r\n"\
  ".button-link:hover {background:#369;}\r\n"\
  "th {padding:10px; background-color:black; color:#ffffff;}\r\n"\
  "td {padding:7px;}\r\n"\
  "table {color:black;}\r\n";

void styles_cbk(char *url, char *rsp)
{
	strcpy(rsp, css_page);
}


void USER_FUNC httpd_init(void)
{
	struct hfeasy_state* state = config_get_state();

	/* register url handler callback */
	if (hfhttpd_url_callback_register(httpd_callback, state->cfg.http_auth) != HF_SUCCESS)
		u_printf("error registering url callback\r\n");
	
	httpd_add_page("/styles.css", styles_cbk);
}
