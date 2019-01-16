////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <hsf.h>

#define CFG_SYS_STACK_SIZE				(1024)

static unsigned char sys_stack_heap[CFG_SYS_STACK_SIZE] __attribute__((section("MSP"))) = {0xA5,};

extern void svcall_interrupt(void);
extern void pendsv_interrupt(void);
extern void systick_interrupt(void);
void reset_handler(void);
void nmi_handler(void);
void hardfault_handler(void);
void memmanage_handler(void);
void busfault_handler(void);
void usagefault_handler(void);

void GpioInterrupt(void) __attribute__((weak));
void RtcInterrupt(void) __attribute__((weak));
void IrInterrupt(void) __attribute__((weak));
//void FuartInterrupt(void) __attribute__((weak));
void FuartInterruptHf(void) __attribute__((weak));
void BuartInterrupt(void) __attribute__((weak));
void PwcInterrupt(void) __attribute__((weak));
void Timer0Interrupt(void) __attribute__((weak));
void UsbInterrupt(void) __attribute__((weak));
void DmaCh0Interrupt(void) __attribute__((weak));
void DmaCh1Interrupt(void) __attribute__((weak));
void audio_decoder_interrupt_handler(void) __attribute__((weak));
void SpisInterrupt(void) __attribute__((weak));
void SdInterrupt(void) __attribute__((weak));
void SpimInterrupt(void) __attribute__((weak));
void SarAdcInterrupt(void) __attribute__((weak));
void I2cInterrupt(void) __attribute__((weak));
void Timer1Interrupt(void) __attribute__((weak));
void WatchDogInterrupt(void) __attribute__((weak));

static void (*const vect_table[])(void) __attribute__((section("EXCEPT_VECTS"))) = {
    (void(*)(void))(0x20000000 + CFG_SYS_STACK_SIZE),	//   Top of Stack
    reset_handler,				//#1 :Reset Handler
    nmi_handler,				//#2 :NMI Handler
    hardfault_handler,			//#3 :Hard Fault Handler
    memmanage_handler,			//#4 :MPU Fault Handler
    busfault_handler,			//#5 :Bus Fault Handler
    usagefault_handler,			//#6 :Usage Fault Handler
    0,							//#7 :Reserved
    0,							//#8 :Reserved
    0,							//#9 :Reserved
    0,							//#10:Reserved
    svcall_interrupt,			//#11:SVCall Handler
    0,							//#12:Debug Monitor Handler
    0,							//#13:Reserved
    pendsv_interrupt,			//#14:PendSV Handler
    systick_interrupt,			//#15:SysTick Handler

    //-----------External Interrupts---------------------
    GpioInterrupt, 				//#16: GPIO 
    RtcInterrupt,				//#17: RTC
    IrInterrupt,		    	//#18: IR
    FuartInterruptHf,				//#19: FUART
    BuartInterrupt,				//#20: BUART 
    PwcInterrupt,				//#21: PWC 
    Timer0Interrupt,			//#22: TIMER0 
    UsbInterrupt,				//#23: USB 
    DmaCh0Interrupt,   			//#24: DMA CH0 
    DmaCh1Interrupt,		    //#25: DMA CH1 
    audio_decoder_interrupt_handler,      		//26: DECODER 
    SpisInterrupt,              //#27: SPIS
    SdInterrupt,				//#28: SDIO
    SpimInterrupt,              //#29: SPIM
    Timer1Interrupt,			//#30: TIMER1
    WatchDogInterrupt,			//#31: WatchDog
};

//******************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.
//
//******************************************************************************

__asm void __bootup_stubs(void)
{
	PRESERVE8
	THUMB

__initial_sp	EQU		(0x20000000 + CFG_SYS_STACK_SIZE)
	EXPORT		__initial_sp	

	//image total size in bytes
	AREA    |.ARM.__at_0x00000080|, CODE, READONLY
IMAGE_SIZE	DCD	0xFFFFFFFF

	//product ID & Check sum
	AREA    |.ARM.__at_0x00000084|, CODE, READONLY
PIDCHKSUM	FILL 8, 0xFF

	//Constant data offset
	AREA    |.ARM.__at_0x0000008C|, CODE, READONLY
CONSTDATA	DCD	0xFFFFFFFF

	//update date offset in Flash/SDRAM
	AREA    |.ARM.__at_0x00000090|, CODE, READONLY
UPDATEDATA	DCD	0xFFFFFFFF

	//SDK version
	AREA    |.ARM.__at_0x00000094|, CODE, READONLY
SDK_VERSION_CHAIP	DCB	0x12
RSDK_VERSION_OTHER	FILL 3,0xFF

	//code memory(exclude of this 4 bytes) CRC
	AREA    |.ARM.__at_0x00000098|, CODE, READONLY
SDK_CODECRC	DCD	0xFFFFFFFF

	//reserved for alignment when linking
	AREA    |.ARM.__at_0x0000009C|, CODE, READONLY
RESV4B		FILL 4,0xFF

	AREA    |.text|, CODE, READONLY

	IMPORT	|Region$$Table$$Base|
	IMPORT	|Region$$Table$$Limit|

	IMPORT  main
	IMPORT	mmm_pool_top  

		//the follow code crack the uvision startup code -huangyucai20111018
__mv_main
		//set up the system stack	
		LDR		SP,=mmm_pool_top 
		SUB		SP,SP,#0x1000
		LDR		R0,=__initial_sp
		SUB		R0,R0,#CFG_SYS_STACK_SIZE
		LDR		R0,[R0]
		PUSH	{R0}

mv_main	PROC
		EXPORT	mv_main	
		IMPORT  __hf_mem_clear
		BL		__hf_mem_clear	
		//get the load region layout table
		LDR		R4,=|Region$$Table$$Base|
		LDR		R5,=|Region$$Table$$Limit|
__NEXT_REGION
		CMP		R4,R5
		//everything is ok
		BCS		__REGION_DECOMP_OK
		LDM		R4,{R0-R3}
		//decompress the data following the compress algorithm as compiling method
		STMDB	R13!,{R4,R5}
		ORR		R3,R3,#0x01
		BLX 	R3
		LDMIA	R13!,{R4,R5}
		ADD		R4,R4,#0x10
		B		__NEXT_REGION
__REGION_DECOMP_OK
#ifdef __MICROLIB
		/*
		 * TO DO
		 */
#else
		IMPORT	__rt_lib_init
		BL		__rt_lib_init
#endif//__MICROLIB

		//fill the system stack space with debug symbol for debug only -huangyucai20111121
#ifdef CFG_SHELL_DEBUG
		LDR		R2,=CFG_SYS_STACK_SIZE
		LDR		R3,=__initial_sp
		SUB		R3,R3,#1
		MOV		R4,#0xA5
AGAIN
		STRB	R4,[R3],#-0x01
		SUBS	R2,R2,#0x01
		BHI		AGAIN
#endif //CFG_SHELL_DEBUG

		LDR		SP,=__initial_sp
		LDR		R0,=main
		BX		R0

		ENDP
		ALIGN

reset_handler\
		PROC
		EXPORT  reset_handler               [WEAK]					    
		MOV		R0,R0
		LDR		R0,=__mv_main
		BX    	R0
		ENDP

;Dummy Exception Handlers (infinite loops which can be modified) 
nmi_handler\
		PROC
        EXPORT  nmi_handler					[WEAK]					    
        B       .
        ENDP

hardfault_handler\
        PROC
		IMPORT hf_trapfault_handler_dumpstack
        EXPORT  hardfault_handler			[WEAK]					    
		TST		LR,	#4
		MRSEQ	R0,	MSP
		MRSNE	R0,	PSP
		PUSH	{R4-R11}
		MRS		R1,	MSP
		B		hf_trapfault_handler_dumpstack
		BX		LR
		ENDP
 
memmanage_handler\
        PROC
		IMPORT hf_trapfault_handler_dumpstack
        EXPORT  memmanage_handler			[WEAK]					    
		TST		LR,	#4
		MRSEQ	R0,	MSP
		MRSNE	R0,	PSP
		PUSH	{R4-R11}
		MRS		R1,	MSP
		B		hf_trapfault_handler_dumpstack
		BX		LR
        ENDP

busfault_handler\
        PROC
		IMPORT hf_trapfault_handler_dumpstack
        EXPORT  busfault_handler			[WEAK]					    
		TST		LR,	#4
		MRSEQ	R0,	MSP
		MRSNE	R0,	PSP
		PUSH	{R4-R11}
		MRS		R1,	MSP
		B		hf_trapfault_handler_dumpstack
		BX		LR
        ENDP

usagefault_handler\
        PROC
		IMPORT hf_trapfault_handler_dumpstack
        EXPORT  usagefault_handler			[WEAK]					    
		TST		LR,	#4
		MRSEQ	R0,	MSP
		MRSNE	R0,	PSP
		PUSH	{R4-R11}
		MRS		R1,	MSP
		B		hf_trapfault_handler_dumpstack
		BX		LR
		ENDP

		ALIGN

//******************************************************************************
//
// Some code in the normal code section for initializing the heap and stack.
//
//******************************************************************************
//
//
//******************************************************************************
//
// The function expected of the C library startup code for defining the stack
// and heap memory locations.  For the C library version of the startup code,
// provide this function so that the C library initialization code can find out
// the location of the stack and heap.
// 
//******************************************************************************
    IF :DEF: __MICROLIB
        EXPORT  __initial_sp
        EXPORT  __heap_base
        EXPORT  __heap_limit
    ELSE
        EXPORT  __initial_sp
        ;EXPORT  __heap_base
        ;EXPORT  __heap_limit
    	ENDIF

//******************************************************************************
//
// Make sure the end of this section is aligned.
//
//******************************************************************************
       ALIGN
//
//******************************************************************************
//
// Tell the assembler that we're done.
//
//******************************************************************************
        END
}

void __hf_mem_clear(void)
{
	int *p_data = (int*)(0x20000000+CFG_SYS_STACK_SIZE);
	int  i;
	for(i=0;i<30*1024;i++)
	{
		p_data[i]=0;
	}
	return;
}
