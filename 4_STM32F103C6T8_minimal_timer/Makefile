TARGET = main

# Default target chip.
MCU ?= STM32F103x6
MCU_SPEC = cortex-m3
MCU_LINKER_FILE  = STM32F103x6_FLASH.ld

# Toolchain definitions (ARM bare metal defaults)
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -Os
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -Os
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs
CFLAGS += -D$(MCU)

# Linker directives.
LSCRIPT = ./ld/$(MCU_LINKER_FILE)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nano.specs
LFLAGS += -Os
LFLAGS += -Wl,--gc-sections,--relax
LFLAGS += -Wl,--start-group -lc -lgcc -lm -lstdc++ -Wl,--end-group
LFLAGS += -T$(LSCRIPT)

AS_SRC   =  ./startup/startup_stm32f103x6.s
C_SRC    = ./startup/system_stm32f1xx.c
C_SRC    += ./src/main.c

INCLUDE  =  -I./device_headers

OBJS  = $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

flash: $(TARGET).bin
	st-flash --connect-under-reset write $(TARGET).bin 0x08000000


.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
