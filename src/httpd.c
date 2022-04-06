//#include <unistd.h>
#include "hfeasy.h"

#define HTTP_SERVER_PORT	8080
#define HTTPD_MAX_PAGES		20

#define HTTPD_RECV_TIMEOUT 3000

#define HTTPD_CORS_HEADER "Access-Control-Allow-Origin: *\r\n"



extern int HSF_API hfhttpd_url_callback_register(hfhttpd_url_callback_t callback, int flag);

static const char *http_header = "HTTP/1.1 200 OK\r\n"\
"Content-type: text/%s\r\n%s"\
"Server: HFEASY\r\n"
"Connection: close\r\n\r\n";

static const char *redirect_page	=
	"<!DOCTYPE html><html><head>"\
	"<script language=\"JavaScript\">"\
	"document.write('<meta http-equiv=\"refresh\" content=\"0;url=' + window.location.protocol + '//' + window.location.hostname + ':8080/\"/>');"\
	"</script>"\
	"</head><body>"\
	"</body></html>";

static const char *hf_redirect_page	=
	"<!DOCTYPE html><html><head>"\
	"<script language=\"JavaScript\">"\
	"document.write('<meta http-equiv=\"refresh\" content=\"0;url=' + window.location.protocol + '//' + window.location.hostname + '/%s\"/>');"\
	"</script>"\
	"</head><body>"\
	"</body></html>";


static const char *auth_page =
	"HTTP/1.0 401 Unauthorized\r\n"\
	"Server: HTTPD\r\n"\
	"Date: Thu, 01 Jan 1970 00:20:05 GMT\r\n"\
	"WWW-Authenticate: Basic realm=\"USER LOGIN\"\r\n"\
	"Pragma: no-cache\r\n"\
	"Cache-Control: no-cache\r\n"\
	"Content-Type: text/html\r\n"\
	"Connection: close\r\n\r\n"\
	"<HTML><HEAD><TITLE>401 Unauthorized</TITLE></HEAD>"\
	"<BODY><H4>401 Unauthorized</H4>"\
	"Authorization required."\
	"</BODY></HTML>";


struct httpd_page {
	const char *url;
	void (*callback) (char *url, char *rsp);
	const char *type;
} httpd_pages[HTTPD_MAX_PAGES] = {
	{ .url = NULL },
};

void b64dec(char *src, char *dst)
{
  int i = 0;
  char tmp[4];
	char *p = src;

	i = 0;
	while ((*p != '\0') || (i != 0)) {
		
		if ((*p == '\0') || (*p == '='))
			tmp[i] = 0;
		else if (*p >= 'a')
			tmp[i] = *p - 'a' + 26;
		else if (*p >= 'A')
			tmp[i] = *p - 'A';
		else
			tmp[i] = *p - '0' + 52;

		if (i == 3) {
			*dst++ = (tmp[0] << 2) | ((tmp[1] & 0x30) >> 4);
      *dst++ = ((tmp[1] & 0xf) << 4) | ((tmp[2] & 0x3c) >> 2);
      *dst++ = ((tmp[2] & 0x3) << 6) | tmp[3];
			i = 0;
		} else {
			i++;
		}
		
		if (*p != '\0')
			p++;
	}
	*dst = '\0';
}

void httpd_check_auth(char *buf, struct sockaddr_in *client)
{
	struct hfeasy_state* state = config_get_state();

	char ans[100];
	char *words[5] = {NULL};

	static u32_t c;
	char *p, *e;
	char tmp[40];
	
	state->is_http_auth = 0;
	
	p = strstr(buf, "Authorization: Basic");
	if (p == NULL)
		return;

/*
	if (c == client->sin_addr.s_addr) {
		state->is_http_auth = 1;
		return;
	}
*/
	p += 21;
	e = strchr(p, '\r');
	if (e != NULL)
		*e = '\0';
	e = strchr(p, '\n');
	if (e != NULL)
		*e = '\0';

	b64dec(p, tmp);
	
	e = strchr(tmp, ':');
	if (e == NULL)
		return;
	*e++ = '\0';
	
	strcpy(ans, "AT+WEBU");
	if (at_cmd(ans, words, 3, 100)) {
		if ((strcmp(tmp, words[1]) == 0) && (strcmp(e, words[2]) == 0)) {
			//c = client->sin_addr.s_addr;
			state->is_http_auth = 1;
		}
	}
}

static void USER_FUNC convert_ascii(char *str)
{
	char *in, *out, c, s[3] = {0, 0, 0};
	
	in = str;
	out = str;
	
	while (*in != '\0') {
		if (*in == '+') {
			*(out++) = ' ';
			in++;
		} else if (*in == '%') {
			memcpy(s, in + 1, 2);
			c = strtol(s, NULL, 16);
			if (c > 127)
				c = '?';
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


int USER_FUNC httpd_add_page(const char *url, void (*callback)(char *url, char *rsp), char *type)
{
	int i;
	
	for (i = 0; i < HTTPD_MAX_PAGES; i++) {
		if (strcmp(httpd_pages[i].url, url) == 0)
			break;
		if (httpd_pages[i].url == NULL)
			break;
	}
	if (i == HTTPD_MAX_PAGES)
		return -1;
	
	httpd_pages[i].url = url;
	httpd_pages[i].callback = callback;
	httpd_pages[i].type = type;
	log_printf("httpd(%d) %s", i, (char*)url);
	
	return 0;
}


static int USER_FUNC httpd_callback(char *url, char *rsp)
{
	struct hfeasy_state* state = config_get_state();
	struct httpd_page *p = httpd_pages;
	char buf[50], *a;
	char *r = rsp;
	int s;
	
	/* change internal page url to /hf */
	if (strcmp(url, "/hf") == 0) {
		strcpy(url, "/");
		return -1;
	}
	
	if (strcmp(url, "/") == 0) {
		s = sprintf(r, http_header, "html", state->cfg.httpd_settings & HTTPD_CORS ? HTTPD_CORS_HEADER : "");
		r += s;
		strcat(r, redirect_page);
		return 0;
	}
	
	/*
	
	while (p->url != NULL) {
		strncpy(buf, url, 50);
		buf[49] = '\0';
		a = strchr(buf, '?');
		if (a != NULL)
			*a = '\0';
		if (strcmp(p->url, buf) == 0) {
			if (p->type != NULL)
				s = sprintf(r, http_header, p->type, state->cfg.httpd_settings & HTTPD_CORS ? HTTPD_CORS_HEADER : "");
			else
				s = sprintf(r, http_header, "html", state->cfg.httpd_settings & HTTPD_CORS ? HTTPD_CORS_HEADER : "");
			r += s;
			p->callback(url, r);
			u_printf("'%s' size=%d\r\n", url, strlen(rsp));
			return 0;
		}
		p++;
	}*/
	
	return -1;
}

static int http_get_alldata_len(char *data)
{
	char *p1 = NULL, *p2 = NULL;
	int headLen=0, bodyLen = 0;
	
	p1 = strstr(data,"\r\n\r\n");
	if(p1 == NULL)
		return -1;

	headLen = p1 - data + 4;
	
	p1 = strstr(data, "Content-Length");
	if(p1 == NULL)
		p1 = strstr(data, "Content-length");
	if(p1 == NULL)
			return -1;

	p2 = p1 + strlen("Content-Length") + 2; 
	bodyLen = atoi(p2);
	
	return headLen + bodyLen;
}

static int https_recv_data(int fd, char *buffer, int len, int timeout_ms, struct sockaddr_in *s)
{
	struct hfeasy_state* state = config_get_state();
	fd_set fdR;
	struct timeval timeout;
	int ret, tmpLen, recvLen = 0;
	int contenLen=0;

	while(1) {
		if(recvLen <= 0) {
			timeout.tv_sec = timeout_ms / 1000;
			timeout.tv_usec = timeout_ms % 1000 * 1000;
		} else {
			timeout.tv_sec = 0;
			timeout.tv_usec = 100 * 1000;
		}
		FD_ZERO(&fdR);
		FD_SET(fd, &fdR);
		ret = select(fd + 1, &fdR, NULL, NULL, &timeout);
		if (ret <= 0) {
			break;
		} else if (FD_ISSET(fd, &fdR)) {
			tmpLen = read(fd, buffer + recvLen, len - recvLen);
			//log_printf("read... %d %d", recvLen, tmpLen);

			if(tmpLen <= 0)
				break;

			recvLen += tmpLen;

			contenLen = http_get_alldata_len(buffer);
			if((contenLen > 0) && (recvLen >= contenLen))
				break;
		}
	}

	log_printf("\r\n******* contentLen=%d recvLen=%d \r\n", contenLen, recvLen);
	buffer[recvLen++] = '\0';
	
	
	//ignore favicon.ico
	//if((recvLen > strlen("GET /favicon.ico")) && (memcmp(buffer, "GET /favicon.ico", strlen("GET /favicon.ico"))==0))
	//	return 0;
	//u_printf("buffer='%s' size=%d\r\n", buffer, strlen(buffer));

	if (recvLen > 5) {
		struct httpd_page *p = httpd_pages;
		char buf[201], *a;
		char *r = buffer;
		char url[201];
		char *sp1, *sp2;

		/* basic auth */
		if (state->cfg.httpd_settings & HTTPD_AUTH) {
			httpd_check_auth(buffer, s);
			if (!state->is_http_auth) {
				write(fd, auth_page, strlen(auth_page));
				return 0;
			}
		}

		sp1 = strchr(buffer, ' ');
		if (sp1 == NULL) {
			return -1;
		}
		sp2 = strchr(sp1+1, ' ');
		if (sp2 == NULL) {
			return -1;
		}
		
		if (memcmp(buffer, "POST", 4) == 0) {
			char *dt = strstr(buffer, "\r\n\r\n");
			if (dt != NULL) {
				dt += 4;
				*sp2++ = '?';
				strcpy(sp2, dt);
			} else {
				*sp2 = '\0';
			}
		} else {
			*sp2 = '\0';
		}		
		
		strncpy(url, sp1+1, 200);
		
		log_printf("url='%s' size=%d\r\n", url, strlen(url));
		
		while (p->url != NULL) {
			int s;
			struct hfeasy_state* state = config_get_state();
			strncpy(buf, url, 200);
			buf[200] = '\0';
			a = strchr(buf, '?');
			if (a != NULL)
				*a = '\0';
			if (strcmp(p->url, buf) == 0) {
				if (p->type != NULL)
					s = sprintf(r, http_header, p->type, state->cfg.httpd_settings & HTTPD_CORS ? HTTPD_CORS_HEADER : "");
				else
					s = sprintf(r, http_header, "html", state->cfg.httpd_settings & HTTPD_CORS ? HTTPD_CORS_HEADER : "");
				r += s;
				p->callback(url, r);
				write(fd, buffer, strlen(buffer));
				log_printf("'%s' size=%d\r\n", url, strlen(buffer));
				msleep(1000);
				return 0;
			}
			p++;
		}
	}
	return -1;
}

static void hf_http_server(void)
{
	char *https_recv_buf;
	struct sockaddr_in local_addr, remote_addr;
	int listenfd, remotefd;
	
	https_recv_buf = hfmem_malloc(HTTPD_RECV_BUF_LEN);
	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		u_printf("HTTP: socket creat failed!\r\n");
		hfmem_free(https_recv_buf);
		hfthread_destroy(NULL);
		return;
	}
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(HTTP_SERVER_PORT);
	local_addr.sin_len = sizeof(local_addr);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(listenfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		u_printf("HTTP: socket bind failed!\r\n");
		hfmem_free(https_recv_buf);
		hfthread_destroy(NULL);
		return ;
	}
	
	if(listen(listenfd, 10) == -1) {
		u_printf("HTTP: socket listen failed!\r\n");
		hfmem_free(https_recv_buf);
		hfthread_destroy(NULL);
		return;
	}

	u_printf("HTTP: https_server start, port=%d\r\n", HTTP_SERVER_PORT);
	
	int len = sizeof(remote_addr);
	while(1) {
		remotefd = accept(listenfd, (struct sockaddr *)&remote_addr, (socklen_t *)(&len));
		if(remotefd >= 0) {
			memset(https_recv_buf, 0, HTTPD_RECV_BUF_LEN);
			https_recv_data(remotefd, https_recv_buf, HTTPD_RECV_BUF_LEN-1, HTTPD_RECV_TIMEOUT, &remote_addr);
		}

		close(remotefd);
	}

	//u_printf("HTTPD: https_server exit!\r\n");
	//hfmem_free(https_recv_buf);
	//hfthread_destroy(NULL);
	return;
}

#endif








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

void iweb_redirect_cbk(char *url, char *rsp)
{
	sprintf(rsp, hf_redirect_page, "iweb.html");
}

void hf_redirect_cbk(char *url, char *rsp)
{
	sprintf(rsp, hf_redirect_page, "hf");
}

void USER_FUNC httpd_init(void)
{
	struct hfeasy_state* state = config_get_state();

	if(hfnet_start_httpd(HFTHREAD_PRIORITIES_MID)!=HF_SUCCESS) {
		log_printf("start httpd fail");
	}
	
#if 1
	if(hfthread_create((PHFTHREAD_START_ROUTINE)hf_http_server, "hfhttp_server", 512, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS)
		u_printf("error starting http server thread\r\n");
#endif
	
	/* register url handler callback */
	if (hfhttpd_url_callback_register(httpd_callback, state->cfg.httpd_settings & HTTPD_AUTH) != HF_SUCCESS)
		u_printf("error registering url callback\r\n");
	
	httpd_add_page("/styles.css", styles_cbk, "css");
	httpd_add_page("/iweb.html", iweb_redirect_cbk, "html");
	httpd_add_page("/hf", hf_redirect_cbk, "html");
}
