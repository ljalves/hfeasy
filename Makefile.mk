
GCC_PATH=/opt/gcc-arm-none-eabi-7-2018-q2-update

CROSS_COMPILE ?= $(GCC_PATH)/bin/arm-none-eabi-
CROSS ?= $(CROSS_COMPILE)

### Compiler definitions
CC       = $(CROSS)gcc
LD       = $(CROSS)gcc
AS       = $(CROSS)as
NM       = $(CROSS)nm
OBJCOPY  = $(CROSS)objcopy
STRIP    = $(CROSS)strip
OBJDUMP  = $(CROSS)objdump

AROPTS = rcf
