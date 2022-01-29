### DEfine the CPU directory
include Makefile.mk

BUILD_PATH = $(shell pwd)
SDK_PATH = $(BUILD_PATH)/sdk/4.13
LINKERSCRIPT = $(SDK_PATH)/libs/LPBX3.ld

PROJECT_NAME=HFEASY

DBGFLAGS   = 	-g
OPTFLAGS   = 	-Os -fomit-frame-pointer
INCLUDE    =  -I$(SDK_PATH)/include/ -I$(SDK_PATH)/include/hsf/ -I$(SDK_PATH)/include/hal/api -I$(SDK_PATH)/include/hal/api
INCLUDE    += -I$(SDK_PATH)/include/hal/targets/cmsis -I$(SDK_PATH)/include/rtos/rtx/TARGET_CORTEX_M -I$(SDK_PATH)/include/lwip-wifi/arch -I$(SDK_PATH)/include/lwip-sys

TOOLS_CFLAGS = -I$(SDK_PATH)/include

#-std=gnu++98 -fno-rtti

CPPFLAGS  += $(INCLUDE) $(OPTFLAGS) $(DBGFLAGS)
ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS    +=  $(CPPFLAGS) $(ARCH_FLAGS)  -Werror=implicit-function-declaration -Werror=maybe-uninitialized
CFLAGS +=  -Wvla -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections 
CFLAGS += -fdata-sections  -funsigned-char -MMD -fno-delete-null-pointer-checks -Os
CFLAGS += -D__LPT230__ -DHF_OTA_SUPPORT -DTOOLCHAIN_GCC_ARM -DLWIP_TIMEVAL_PRIVATE=0 -include $(SDK_PATH)/include/mbed_config.h


LDFLAGS += -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_calloc_r -mcpu=cortex-m4 
LDFLAGS += -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Wl,-L $(SDK_PATH)/libs/ -lLPBX3Kernel -L $(SDK_PATH)/../Cyassl -lLPBx30SSL -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -T $(LINKERSCRIPT) -Wl,-Map,$(BUILD_DIR)/$(PROJECT_NAME).map 

APPDIR = src

BUILD_DIR=./projects/build_lpxx30

### Compilation rules

# Don't treat %.elf %.bin as an imtermediate file!
.PRECIOUS: %.elf %.bin

all:
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(APPDIR)
	make $(PROJECT_NAME).bin
	./tools/gen_upgrade_bin $(BUILD_DIR)/$(PROJECT_NAME).bin $(BUILD_DIR)/$(PROJECT_NAME)_UPGRADE.bin ./tools/imgpkt

#%.bin:%.elf
$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
		@echo "-----------------make target:$@ 1st-dep:$< $(makefn3)-----------------"
		$(OBJCOPY) -O binary $(BUILD_DIR)/$< $(BUILD_DIR)/$@
		@arm-none-eabi-objdump -h $(BUILD_DIR)/$(PROJECT_NAME).elf > $(BUILD_DIR)/dump.log
		@echo
		@echo "BUILD COMPLETE :: LPB130 serial upload binary at: $(BUILD_DIR)/$(PROJECT_NAME).bin"
		@echo "BUILD COMPLETE :: LPB130 web upload binary at: $(BUILD_DIR)/$(PROJECT_NAME)_UPGRADE.bin"
		@echo

clean:
	cd $(APPDIR) && make clean

#startup_LPBX3GCC.o:	$(SDK_PATH)/libs/startup_LPBX3GCC.S
#	$(CC) $(CFLAGS) -c $(SDK_PATH)/libs/startup_LPBX3GCC.S -o startup_LPBX3GCC.o

%.o: %.c
		@echo "-----------------make target:$@ $(makefn3)-----------------"
		$(CC) $(CFLAGS) $< -c

asm:
	$(OBJDUMP) -D -S $(PROJECT_NAME).elf > $(PROJECT_NAME).asm

print:
		echo $(CONTIKI_OBJ)

$(PROJECT_NAME).elf: $(APPDIR)/userapps.a $(LINKERSCRIPT)
	/bin/mv $(APPDIR)/userapps.a $(BUILD_DIR)/
	$(CC) $(LDFLAGS) $(BUILD_DIR)/userapps.a -L $(SDK_PATH)/libs/ -lLPBX3Kernel -o $(BUILD_DIR)/$(PROJECT_NAME).elf
