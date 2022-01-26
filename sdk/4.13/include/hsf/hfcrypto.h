/* hfcrypto.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HF_CRYPTO_H_
#define _HF_CRYPTO_H_

#ifdef __cplusplus
extern "C" {
#endif


void HSF_IAPI hmac_sha1(unsigned char *key,int key_length,unsigned char *data,int data_length,unsigned char *digest);

/**
 * @brief encrypt data with aes (ecb mode/128bit key).
 *
 * @param[in] key: the key of AES
 *			 data: the data want to encrypt
 *			 data_len: the length of data
 * @return[out] the length of encrypted data
 * @see None.
 * @note None.
 */
int HSF_API hfcrypto_aes_ecb_encrypt(const unsigned char *key, unsigned char *data, int data_len);

/**
 * @brief decrypt data with aes (ecb mode/128bit key).
 *
 * @param[in] key: the key of AES
 *			 data: the data want to decrypt
 *			 data_len: the length of data
 * @return[out] the length of decrypted data
 * @see None.
 * @note None.
 */
int HSF_API hfcrypto_aes_ecb_decrypt(const unsigned char *key, unsigned char *data, int data_len);

/**
 * @brief encrypt data with aes (cbc mode/128bit key).
 *
 * @param[in] key: the key of AES
 *			 iv: the initialization vector of AES
 *			 data: the data want to encrypt
 *			 data_len: the length of data
 * @return[out] the length of encrypted data
 * @see None.
 * @note None.
 */
int HSF_API hfcrypto_aes_cbc_encrypt(const unsigned char *key, const unsigned char *iv, unsigned char *data, int data_len);

/**
 * @brief decrypt data with aes (cbc mode/128bit key).
 *
 * @param[in] key: the key of AES
 *			 iv: the initialization vector of AES
 *			 data: the data want to decrypt
 *			 data_len: the length of data
 * @return[out] the length of decrypted data
 * @see None.
 * @note None.
 */
int HSF_API hfcrypto_aes_cbc_decrypt(const unsigned char *key, const unsigned char *iv, unsigned char *data, int data_len);

/**
 * @brief calculate the MD5 of the data.
 *
 * @param[in] data: the data want to calculate MD5
 *			 len: the length of data, in bytes
 *			 digest: store MD5, must > 16 bytes
 * @return[out] the length of MD5, in bytes
 * @see None.
 * @note None.
 */
int HSF_API hfcrypto_md5(const unsigned char *data, int len, unsigned char *digest);

/**
 * @brief encode data as base64.
 *
 * @param[in] src: input byte array
 *			 srclength: size of input byte array
 *			 target: output byte array
 *			 targsize: size of output byte array
 * @return[out] returns the number of data bytes stored at the target, or -1 on error.
 * @see None.
 * @note encode data as base64, converts characters, three at a time,
 *           starting at src into four base64 characters in the target area
 *           until the entire input buffer is encoded.
 */
int HSF_API hfcrypto_base64_encode(char *src, size_t srclength, char *target, size_t targsize);

/**
 * @brief decode Base64 data.
 *
 * @param[in] src: input byte array
 *			 target: output byte array
 *			 targsize: size of output byte array
 * @return[out] returns the number of data bytes stored at the target, or -1 on error.
 * @see None.
 * @note decode Base64 data, skips all whitespace anywhere.  converts
 *           characters, four at a time, starting at (or after) src from Base64
 *           numbers into three 8 bit bytes in the target area..
 */
int HSF_API hfcrypto_base64_decode(char *src, char *target, size_t targsize);


#ifdef __cplusplus
}
#endif

#endif

