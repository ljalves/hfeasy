#ifndef _I2C_H_
#define _I2C_H_

#include <hsf.h>


int USER_FUNC gpio_i2c_send(uint8_t addr, uint16_t data);
int USER_FUNC gpio_i2c_recv(uint8_t addr, uint8_t *data);

#endif
