#ifndef _RDA_AES_API_H_
#define _RDA_AES_API_H_

#include "rda_dma.h"

#if RDA_HW_ACCELERATE_ENABLE
#define RDA_HW_AES_ENABLE   1
#endif

#define RDA_AES_ENC_MODE AES_ENC_MODE
#define RDA_AES_DEC_MODE AES_DEC_MODE

#ifdef __cplusplus
extern "C" {
#endif


extern int rda_aes_crypt( unsigned int *RK_buf,
                    unsigned int mode, /* DEC/ENC*/
                    unsigned int length, /* bytes */
                    unsigned char iv[16], /*CBC only*/
                    const unsigned char *input,
                    unsigned char *output );


#ifdef __cplusplus
}
#endif

#endif
