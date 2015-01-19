#Define toolchain executeables
AR = xtensa-lx106-elf-ar
CC = xtensa-lx106-elf-cc
NM = xtensa-lx106-elf-nm
CPP = xtensa-lx106-elf-cpp
OBJCOPY = xtensa-lx106-elf-objcopy

#Define file paths
SRCDIR=./src
OBJDIR := .obj
LDIR=../lib
LIBS=-lfreertos

#Define each object to be created
OBJS := $(addprefix $(OBJDIR)/,gpio.o test.o)

#Define compiler and linker flags
CFLAGS=-c -Wall -DICACHE_FLASH -I./include -I../include -I../include/espressif -I../include/lwip -I../include/lwip/ipv4 -I../include/lwip/ipv6 -I../extra_include -I../include/freertos/
LDFLAGS= 

#This is the main target to create the library
all: $(OBJS)
	$(AR) rcs $(LDIR)/libribanesp8266.a $(OBJS)

#Rule to build each object file from its source file
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
    
#Rule to create obect directory if not exist
$(OBJS): | $(OBJDIR)

#Rule to create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY : clean

#Rule to clean (remove) object directory
clean:
	-rm -rf $(OBJDIR) 
