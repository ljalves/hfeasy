#ifndef __MQTTCLI_H__
#define __MQTTCLI_H__

#include "hfeasy.h"

void USER_FUNC mqttcli_init(void);
void USER_FUNC mqttcli_initcfg(void);
void USER_FUNC mqttcli_publish(char *value, char *sufix);

#endif
