 /* hflist.h
 *
 * Copyright (C) 2017 ShangHai High-flying Electronics Technology Co.,Ltd.
 *
 * This file is part of HSF.
 *
 */
 
#ifndef _HF_LIST_H_
#define _HF_LIST_H_


#include <hsf.h>

#ifdef __cplusplus
extern "C"{
#endif

//#define DEBUG_LIST

typedef struct _SLIST_ENTRY_T
{
	struct _SLIST_ENTRY_T* Next;
	void *data;

}SLIST_ENTRY_T,*P_SLIST_ENTRY_T;

typedef void (*LSLIST_FREE_CALLBACK)(P_SLIST_ENTRY_T);

typedef struct _SLIST_HEADER_T {
	SLIST_ENTRY_T Next;
	P_SLIST_ENTRY_T Tail;
	WORD   Depth;
	LSLIST_FREE_CALLBACK FreeCallback;
	hfthread_mutex_t	 Lock;
	void   *ctx;
} SLIST_HEADER_T, *PSLIST_HEADER_T;

#ifndef DEBUG_LIST
#define DebugList(_l)
#endif

void InitializeSListTHead(
  PSLIST_HEADER_T ListHead,
  LSLIST_FREE_CALLBACK FreeCallback
);

PSLIST_HEADER_T  AddHeaderEntrySList(
	PSLIST_HEADER_T ListHead,
	P_SLIST_ENTRY_T Entry
);

PSLIST_HEADER_T  AddTailEntrySList(
	PSLIST_HEADER_T ListHead,
	P_SLIST_ENTRY_T Entry
);

P_SLIST_ENTRY_T  RemoveEntrySList(
	PSLIST_HEADER_T ListHead,
	P_SLIST_ENTRY_T Entry
);

PSLIST_HEADER_T ListCatSList(
	PSLIST_HEADER_T List1,
	PSLIST_HEADER_T List2
	);

#define RemoveFirstEntrySList(ListHead)		RemoveEntrySList(ListHead,(ListHead)->Next.Next)
#define RemoveTailEntrySList(ListHead)		RemoveEntrySList(ListHead,(ListHead)->Tail)


P_SLIST_ENTRY_T RemoveAndFreeEntrySList(
	PSLIST_HEADER_T ListHead,
	P_SLIST_ENTRY_T Entry
);

static inline DWORD QueryDepthSTList(PSLIST_HEADER_T ListHead)
{
	if(ListHead==NULL)
		return 0;
	DebugList(ListHead);
	return ListHead->Depth;
}

static inline int FlushSlist(PSLIST_HEADER_T ListHead)
{
	P_SLIST_ENTRY_T Entry;
	if(ListHead==NULL)
		return -1;

	while((Entry=RemoveFirstEntrySList(ListHead))!=NULL)
	{
		if(ListHead->FreeCallback!=NULL)
			ListHead->FreeCallback(Entry);
	}
	
	return 0;
}

static inline P_SLIST_ENTRY_T  GetFirstEntrySlist(
	PSLIST_HEADER_T ListHead
)
{
	DebugList(ListHead);
	return ListHead->Next.Next;
}

static inline P_SLIST_ENTRY_T  GetTailEntrySlist(
	PSLIST_HEADER_T ListHead
)
{
	DebugList(ListHead);
	return ListHead->Tail;
}


#define IsEmptySlist(_head)			(QueryDepthSTList(_head)<=0)
#define GetNextEntrySlist(Entry)	(Entry)->Next

#define LockList(__ListHead)			hfthread_mutext_lock((__ListHead)->Lock)
#define UnlockList(__ListHead)			hfthread_mutext_unlock((__ListHead)->Lock)

void DestroySlist(PSLIST_HEADER_T ListHead);


#ifdef __cplusplus
}
#endif

#endif

