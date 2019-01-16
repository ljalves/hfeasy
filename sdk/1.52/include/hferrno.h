/* 
 *hferrno.h
 *
 * Copyright (C) 2013-2020 ShangHai High-flying Electronics Technology Co.,Ltd.
 *				  	All rights reserved.
 *
 * This file is part of HSF.
 *
 *
 */


#ifndef _HFERRNO_H_
#define _HFERRNO_H_

/* Get the module index number from error code (4th byte from LSB)*/
#define get_module_base(code) ((code&0xFF0000)>>16)

/* Get module notifier code (2nd and 1st byte from LSB)*/
#define get_code(code) (code&0xFF)

#define MOD_ERROR_START(x)  ((x << 16) | 0)

/* Create Module index */
#define MOD_GENERIC    0
/** HTTPD module index */
#define MOD_HTTPDE     1
/** HTTP-CLIENT module index */
#define MOD_HTTPC      2
/** WPS module index */
#define MOD_WPS        3
/** WLAN module index */
#define MOD_WLAN       4
/** USB module index */
#define MOD_USB        5

#define MOD_USER_DEFINE		(0x80)

/* Globally unique success code */
#define HF_SUCCESS 0

enum hf_errno {
	/* First Generic Error codes */
	HF_GEN_E_BASE = MOD_ERROR_START(MOD_GENERIC),
	HF_FAIL,
	HF_E_PERM,   /* Operation not permitted */
	HF_E_NOENT,  /* No such file or directory */
	HF_E_SRCH,   /* No such process */
	HF_E_INTR,   /* Interrupted system call */
	HF_E_IO,     /* I/O error */
	HF_E_NXIO,   /* No such device or address */
	HF_E_2BIG,   /* Argument list too long */
	HF_E_NOEXEC, /* Exec format error */
	HF_E_BADF,   /* Bad file number */
	HF_E_CHILD,  /* No child processes */
	HF_E_AGAIN,  /* Try again */
	HF_E_NOMEM,  /* Out of memory */
	HF_E_ACCES,  /* Permission denied */
	HF_E_FAULT,  /* Bad address */
	HF_E_NOTBLK, /* Block device required */
	HF_E_BUSY,   /* Device or resource busy */
	HF_E_EXIST,  /* File exists */
	HF_E_XDEV,   /* Cross-device link */
	HF_E_NODEV,  /* No such device */
	HF_E_NOTDIR, /* Not a directory */
	HF_E_ISDIR,  /* Is a directory */
	HF_E_INVAL,  /* Invalid argument */
	HF_E_NFILE,  /* File table overflow */
	HF_E_MFILE,  /* Too many open files */
	HF_E_NOTTY,  /* Not a typewriter */
	HF_E_TXTBSY, /* Text file busy */
	HF_E_FBIG,   /* File too large */
	HF_E_NOSPC,  /* No space left on device */
	HF_E_SPIPE,  /* Illegal seek */
	HF_E_ROFS,   /* Read-only file system */
	HF_E_MLINK,  /* Too many links */
	HF_E_PIPE,   /* Broken pipe */
	HF_E_DOM,    /* Math argument out of domain of func */
	HF_E_RANGE,  /* Math result not representable */
	HF_E_DEADLK,    /*Resource deadlock would occur*/
};


#endif /* HFERRNO_H_ */

