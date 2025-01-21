ARCH = armv7-a
MCPU = cortex-a7 # 추후 3B+로 실제 보드에 올릴 때 수정해야함.

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./linker.ld
MAP_FILE = build/pirtos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

VPATH = boot 			\
        hal				\
		lib

C_SRCS  = $(notdir $(wildcard boot/*.c))
C_SRCS += $(notdir $(wildcard hal/*.c))
C_SRCS += $(notdir $(wildcard lib/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))

INC_DIRS  = -I include 			\
            -I hal	   			\
            -I lib

CFLAGS = -c -g -std=c11

LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc

pirtos = build/pirtos.axf
pirtos_bin = build/pirtos.bin

.PHONY: all clean run debug gdb

all: $(pirtos)

clean:
	@rm -fr build
	
run: $(pirtos)
	qemu-system-arm -M raspi2b -kernel $(pirtos) -nographic
	
debug: $(pirtos)
	qemu-system-arm -M raspi2b -kernel $(pirtos) -S -gdb tcp::1234,ipv4
	
gdb:
	gdb-multiarch $(pirtos)

kill:
	kill -9 `ps aux | grep 'qemu' | awk 'NR==1{print $$2}'`
	
$(pirtos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(pirtos) $(ASM_OBJS) $(C_OBJS) -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OC) -O binary $(pirtos) $(pirtos_bin)
	
build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<
	
build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<