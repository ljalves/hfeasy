/* HFeasy

Copyright (c) 2019 Luis Alves

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


	Bit-bang i2c implementation

*/

#include "hfeasy.h"


static int USER_FUNC gpio_i2c_bitbang(uint8_t d)
{
	int i, nack;
	
	for (i = 0; i < 8; i++) {
		if (d & 0x80)
			hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		else
			hfgpio_fset_out_low(GPIO_I2C_SDA);

		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		hfgpio_fset_out_low(GPIO_I2C_SCL);
		d <<= 1;
	}
	/* release SDA */
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	/* read ack */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	nack = gpio_get_state(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	return nack;
}

int USER_FUNC gpio_i2c_send(uint8_t addr, uint16_t data)
{
	int nack;

	/* start */
	hfgpio_fset_out_low(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	/* send slave addr */
	nack = gpio_i2c_bitbang(addr);
	if (nack)
		return nack;
	
	nack = gpio_i2c_bitbang(0x00);
	nack |= gpio_i2c_bitbang(0x80);
	nack |= gpio_i2c_bitbang((uint8_t) data);

	/* stop condition */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	return nack;
}


int USER_FUNC gpio_i2c_recv(uint8_t addr, uint8_t *data)
{
	int i, nack;
	uint8_t d;
	
	/* start */
	hfgpio_fset_out_low(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	/* send slave addr */
	d = addr;
	for (i = 0; i < 8; i++) {
		if (d & 0x80)
			hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		else
			hfgpio_fset_out_low(GPIO_I2C_SDA);

		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		hfgpio_fset_out_low(GPIO_I2C_SCL);
		d <<= 1;
	}
	/* release SDA */
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	/* read ack */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	nack = gpio_get_state(GPIO_I2C_SDA);
	hfgpio_fset_out_low(GPIO_I2C_SCL);

	if (nack == 0 && data != NULL) {
		/* ack received */
		/* receive data */
		d = 0;
		for (i = 0; i < 8; i++) {
			d <<= 1;
			hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
			d |= gpio_get_state(GPIO_I2C_SDA);
			hfgpio_fset_out_low(GPIO_I2C_SCL);
		}
		/* release SDA - nack = stop send data */
		hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
		/* send nack */
		hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
		hfgpio_fset_out_low(GPIO_I2C_SCL);
		*data = d;
	}
	
	/* stop condition */
	hfgpio_configure_fpin(GPIO_I2C_SCL, HFM_IO_TYPE_INPUT);
	hfgpio_configure_fpin(GPIO_I2C_SDA, HFM_IO_TYPE_INPUT);
	return nack;
}
