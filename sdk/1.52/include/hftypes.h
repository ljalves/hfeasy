
#ifndef _HF_TYPES_H_H_
#define _HF_TYPES_H_H_



#ifdef __KEIL_MDK__
	#ifdef __BUILD_HSF_SDK__
#include <common/type.h>
	#else
#define false           (0)
#define true            (1)
#define	FALSE			(0)
#define	TRUE			(1)

typedef unsigned char   BOOL;
typedef unsigned char   bool;
typedef	unsigned int	size_t;
	#endif
#include <stdint.h>

#endif

#endif

