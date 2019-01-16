
#ifndef _HF_SPI_H_H_H_
#define _HF_SPI_H_H_H_

#define	HFSPIM_CLK_DIV_24M		0x0		/**< SPI master clock Div 24MHz*/
#define	HFSPIM_CLK_DIV_12M		0x1		/**< SPI master clock Div 12MHz*/
#define	HFSPIM_CLK_DIV_6M			0x2		/**< SPI master clock Div 6MHz*/
#define	HFSPIM_CLK_DIV_3M			0x3		/**< SPI master clock Div 3MHz*/
#define	HFSPIM_CLK_DIV_1M5		0x4		/**< SPI master clock Div 1.5MHz*/
#define	HFSPIM_CLK_DIV_750K		0x5		/**< SPI master clock Div 750KHz*/
#define	HFSPIM_CLK_DIV_325K		0x6		/**< SPI master clock Div 325KHz*/

#define HFSPI_MODE_CPOL0_CPHA0		(0)
#define HFSPI_MODE_CPOL0_CPHA1		(1)
#define HFSPI_MODE_CPOL1_CPHA0		(2)
#define HFSPI_MODE_CPOL1_CPHA1		(3)


int HSF_API hfspi_master_init(uint8_t mode, uint8_t clk_div);

int HSF_API hfspi_master_send_data(char* send_buf, uint32_t length);

int HSF_API hfspi_master_recv_data(char* buf,uint32_t length);

void HSF_API hfspi_cs_low(void);

void HSF_API hfspi_cs_high(void);

#endif


