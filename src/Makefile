include ../Makefile.mk

OBJECTDIR=../projects/build_lpxx30

USERAPP_SRC = \
app_main.c \
config.c gpio.c   i2c.c  mqttcli.c  network.c  \
buzzer.c    dimmer.c  httpd.c  led.c  mqttlib.c  relay.c    timer.c

APPCFLAGS = -DSUPPORT_UART_THROUGH -D__LPXX30__

USERAPP_OBJS=${addprefix $(OBJECTDIR)/,$(USERAPP_SRC:.c=.o) $(USERAPP_SRC:.c=.o)}

all:
	@mkdir -p $(OBJECTDIR)
	make userapps.a

userapps.a: $(USERAPP_OBJS)
	echo "$(AR) $(AROPTS) $@ $^"
	$(AR) $(AROPTS) $@ $^

print:
	echo "USERAPP_SRC=$(USERAPP_SRC)"
	echo "USERAPP_OBJS=$(USERAPP_OBJS)"

$(OBJECTDIR)/%.o: %.c | $(OBJECTDIR)
	@echo "-----------------make target:$@ $(makefn3)-----------------"
	$(CC) $(CFLAGS) $(APPCFLAGS) -c $< -o $@

clean:
#	/bin/rm -f *.o *.elf *.a *.bin *.asm 
	/bin/rm -rf $(OBJECTDIR)

