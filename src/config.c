
#include "hfeasy.h"


static void USER_FUNC httpd_page_config(char *url, char *rsp)
{
	
	
}


void config_init(void)
{
	httpd_add_page("/config", httpd_page_config);
}
