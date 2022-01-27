
#include "hfeasy.h"

#define HTTP_SERVER_PORT	81
#define HTTPD_MAX_PAGES		20

#define HTTPS_RECV_TIMEOUT 3000
#define HTTPS_RECV_BUF_LEN 2048

static char https_recv_buf[HTTPS_RECV_BUF_LEN];

#define HttpRspDataFormat      "HTTP/1.1 200 OK\r\n"\
"Content-type: text/html\r\n"\
"Content-length: 72\r\n"\
"\r\n"\
"<html><body>Hello World, System RunTime:  %02d:%02d:%02d !</body></html>"



static const char *http_header = "HTTP/1.1 200 OK\r\n"\
"Content-type: text/%s\r\n"\
"Server: HFEASY\r\n"
"Connection: close\r\n\r\n";


struct httpd_page {
	const char *url;
	void (*callback) (char *url, char *rsp);
	const char *type;
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
		} else if (*in == '+') {
			*(out++) = ' ';
			in++;
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
	struct httpd_page *p = httpd_pages;
	char buf[50], *a;
	char *r = rsp;
	int s;
	
	/* change internal page url to /hf */
	if (strcmp(url, "/hf") == 0) {
		strcpy(url, "/");
		return -1;
	}
	
	while (p->url != NULL) {
		strncpy(buf, url, 50);
		buf[49] = '\0';
		a = strchr(buf, '?');
		if (a != NULL)
			*a = '\0';
		if (strcmp(p->url, buf) == 0) {
			if (p->type != NULL)
				s = sprintf(r, http_header, p->type);
			else
				s = sprintf(r, http_header, "html");
			r += s;
			p->callback(url, r);
			u_printf("'%s' size=%d\r\n", url, strlen(rsp));
			return 0;
		}
		p++;
	}
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
	
	p1 = strstr(data,"Content-Length");
	if(p1 == NULL)
		p1 = strstr(data,"Content-length");
	if(p1 == NULL)
			return -1;

	p2 = p1 + strlen("Content-Length") + 2; 
	bodyLen = atoi(p2);
	
	return headLen + bodyLen;
}

static int https_recv_data(int fd, char *buffer, int len, int timeout_ms)
{
	fd_set fdR;
	struct timeval timeout;
	int ret, tmpLen, contenLen=0, recvLen = 0;
	
	while(1) {
		FD_ZERO(&fdR);
		FD_SET(fd, &fdR);
		if(recvLen <= 0) {
			timeout.tv_sec = timeout_ms / 1000;
			timeout.tv_usec = timeout_ms % 1000 * 1000;
		} else {
			//fast close
			timeout.tv_sec = 0;
			timeout.tv_usec = 500 * 1000;
		}
		
		ret = select(fd + 1, &fdR, NULL, NULL, &timeout);
		if (ret <= 0) {
			break;
		} else if (FD_ISSET(fd, &fdR)) {
			tmpLen = read(fd, buffer + recvLen, len - recvLen);
			if(tmpLen <= 0)
				break;
			recvLen += tmpLen;

			contenLen = http_get_alldata_len(buffer);
			if((contenLen > 0) && (recvLen >= contenLen)) 
				break;
		}
	}

	u_printf("\r\n******* contentLen=%u recvLen=%u \r\n", contenLen, recvLen);
	
	buffer[recvLen] = '\0';
	
	u_printf("size=%d\r\n", strlen(buffer));

	
	
	int i;
	char buffer2[101];
	for (i = 0; i < recvLen; i+=100) {
		strncpy(buffer2, buffer+i, recvLen-i < 100 ? recvLen-i : 100);
		buffer2[100] = '\0';
		u_printf("%s", buffer2);
	}
	u_printf("\r\n-----------------------\r\n");
	msleep(1000);

	char *sp1, *sp2;
	sp1 = strchr(buffer, ' ');
	if (sp1 == NULL) {
		u_printf("sp1 not found!\r\n");
		return -1;
	}
	sp2 = strchr(sp1+1, ' ');
	if (sp2 == NULL) {
		u_printf("sp2 not found!\r\n");
		return -1;
	}
	*sp2 = '\0';
	
	//ignore favicon.ico
	//if((recvLen > strlen("GET /favicon.ico")) && (memcmp(buffer, "GET /favicon.ico", strlen("GET /favicon.ico"))==0))
	//	return 0;

	//u_printf("buffer='%s' size=%d\r\n", buffer, strlen(buffer));

	if (recvLen > 5) {
		struct httpd_page *p = httpd_pages;
		char buf[50], *a;
		char *r = buffer;
		char url[50];

		strncpy(url, sp1+1, 50);
		
		u_printf("url='%s' size=%d\r\n", url, strlen(url));
		
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
				write(fd, buffer, strlen(buffer));
				u_printf("'%s' size=%d\r\n", url, strlen(buffer));
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
	struct sockaddr_in local_addr, remote_addr;
	int listenfd, remotefd;
	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		u_printf("HTTP: socket creat failed!\r\n");
		hfthread_destroy(NULL);
		return;
	}
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(HTTP_SERVER_PORT);
	local_addr.sin_len = sizeof(local_addr);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(listenfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		u_printf("HTTP: socket bind failed!\r\n");
		hfthread_destroy(NULL);
		return ;
	}
	
	if(listen(listenfd, 10) == -1) {
		u_printf("HTTP: socket listen failed!\r\n");
		hfthread_destroy(NULL);
		return;
	}

	u_printf("HTTP: https_server start, port=%d\r\n", HTTP_SERVER_PORT);
	
	int len = sizeof(remote_addr);
	while(1) {
		remotefd = accept(listenfd, (struct sockaddr *)&remote_addr, (socklen_t *)(&len));
		if(remotefd >= 0) {
			memset(https_recv_buf, 0, HTTPS_RECV_BUF_LEN);
			https_recv_data(remotefd, https_recv_buf, HTTPS_RECV_BUF_LEN-1, HTTPS_RECV_TIMEOUT);
		}

		close(remotefd);
	}

	//u_printf("HTTPS: https_server exit!\r\n");
	//hfthread_destroy(NULL);
	return;
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

	if(hfnet_start_httpd(HFTHREAD_PRIORITIES_MID)!=HF_SUCCESS) {
		log_write("start httpd fail");
	}
	
#if 0
	if(hfthread_create((PHFTHREAD_START_ROUTINE)hf_http_server, "hfhttp_server", 2048, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS) {
		u_printf("error starting http server thread\r\n");
#endif
	
	/* register url handler callback */
	if (hfhttpd_url_callback_register(httpd_callback, state->cfg.http_auth) != HF_SUCCESS)
		u_printf("error registering url callback\r\n");
	
	httpd_add_page("/styles.css", styles_cbk, "css");
}
