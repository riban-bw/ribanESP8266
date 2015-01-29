#Define name of library
LIBFILENAME = libribanesp8266.a

#Define toolchain executeables
AR = xtensa-lx106-elf-ar
CC = xtensa-lx106-elf-cc
NM = xtensa-lx106-elf-nm
CPP = xtensa-lx106-elf-cpp
OBJCOPY = xtensa-lx106-elf-objcopy

#Define file paths
ESPSDK = ../esp_iot_rtos_sdk
SRCDIR=./src
OBJDIR := .obj
LDIR=$(ESPSDK)/lib
LIBS=-lfreertos

#Define each object to be created
OBJS := $(addprefix $(OBJDIR)/,esp.o gpi.o uart.o)

#Define compiler and linker flags
INCLUDES = -I./include -I$(ESPSDK)/include -I$(ESPSDK)/include/espressif -I$(ESPSDK)/include/lwip -I$(ESPSDK)/include/lwip/ipv4 -I$(ESPSDK)/include/lwip/ipv6 -I$(ESPSDK)/extra_include -I$(ESPSDK)/include/freertos/
CFLAGS = -c -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -DICACHE_FLASH $(INCLUDES)

LDFLAGS =

#This is the main target to create the library
all: $(OBJS)
	$(AR) ru ./lib/$(LIBFILENAME) $(OBJS)
	-cp  ./lib/$(LIBFILENAME) $(LDIR)
	
#Rule to build each object file from its source file
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
    
#Rule to create obect directory if not exist
$(OBJS): | $(OBJDIR)

#Rule to create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p ./lib

#Rule to create wiki documentation from javadoc comments
docs: ./wiki/*.md
#	javadoc2markdown $< > ./wiki/`basename -s .h $< | tr '[a-z]' '[A-Z]'`-Documentation.md 
	javadoc2markdown ./include/gpio.h > ./wiki/GPI-Documentation.md
	javadoc2markdown ./include/uart.h > ./wiki/UART-Documentation.md
	javadoc2markdown ./include/esp.h > ./wiki/ESP-Documentation.md
	
.PHONY : clean

#Rule to clean (remove) object directory
clean:
	-rm -rf $(OBJDIR) 
	-rm -rf ./lib
	-rm -f $(LDIR)/$(LIBFILENAME)
	
