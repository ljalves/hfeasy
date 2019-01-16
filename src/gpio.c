
#include "hfeasy.h"


static void USER_FUNC switch_state_page(char *url, char *rsp)
{
	char val[20];
	int ret;

	ret = httpd_arg_find(url, "sw", val);
	
	sprintf(rsp, "<html><head><title>SWITCH STATE</title></head>" \
							 "<body>SWITCH STATE WEB PAGE<br>" \
							 "ret=%d, sw='%s'" \
							 "</body></html>", ret, val);

	u_printf("ret=%d, sw='%s', page '%s' served\r\n", ret, val);
}

void USER_FUNC gpio_init(void)
{
	/* tmp */
	#define HFGPIO_F_BUZZER		         (HFGPIO_F_USER_DEFINE+0)
	hfgpio_fset_out_low(HFGPIO_F_BUZZER);
	
	httpd_add_page("/state", switch_state_page);
}