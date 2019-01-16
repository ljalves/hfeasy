
/* hfir.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 */

#ifndef _HFIR_H_H_
#define _HFIR_H_H_

/*
 *
 **/
bool HSF_API  hfir_is_key_come(void);

/*
 *
 **/
uint32_t  HSF_API  hfir_get_key_code(void);

/*
 *
 **/
bool HSF_API  hfir_is_continue_prs(void);

/*
 *
 **/
void HSF_API  hfir_ignore_lead_header(bool is_ignore);


#endif

