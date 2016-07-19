#######################################
# project specific definitions
MCU=attiny85
TARGET=receiver
F_CPU=7372800

# application defintions (verbose, debug, etc.)
#DEFINITIONS=-DUART_LOG
#DEFINITIONS=-DUART_ENABLED

#######################################
# libraries to be linked
#LIBRARIES=eeprom uart adc onewire
#LIBRARIES=uart onewire

#######################################
# directories
SRC=src
OBJDIR=obj

#######################################
# programming ports
PARPORT=/dev/parport0
SERIALPORT=/dev/ttyS0

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

#######################################
# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD=
#-std=gnu99

#######################################
# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS= -DF_CPU=$(F_CPU)
#CFLAGS+= -Wall -Wstrict-prototypes -mcall-prologues
CFLAGS+= -Wall -mcall-prologues
CFLAGS+= -g$(DEBUG)
CFLAGS+= -Os
#CFLAGS+= -v
CFLAGS+= $(CSTANDARD)
CFLAGS+= $(DEFINITIONS)
CFLAGS+= -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS+= -Wa,-adhlns=$(@:.o=.lst)

#######################################
# External memory options

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
#LDFLAGS+= -Wl,--gc-sections
LDFLAGS += $(EXTMEMOPTS)

# Linker flags
# workarounds for Gentoo BUG 147155:
# run #ln -s /usr/i686-pc-linux-gnu/avr/lib/ldscripts /usr/avr/lib/ldscripts
# OR
#LDFLAGS+= -T /usr/lib/binutils/avr/2.24/ldscripts/avr5.x
#LDFLAGS+= -T /usr/i486-pc-linux-gnu/avr/lib/ldscripts

#######################################
# programs
CC=avr-g++
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AR=ar
RM=rm -rf
MKDIR=mkdir
NM=nm
SIZE=avr-size
SIZEM=avr-size-mcu.sh

#######################################
# Fuses

# Frequency range 3.0 - 8.0 MHz
#CKSEL=111
#CKOPT=1
# Crystal oscillator, fast rising power
#SUT=10

######################################
# Define all object files.
SOURCES=$(wildcard $(SRC)/*.cpp)
ASOURCES=$(wildcard $(SRC)/*.S)
OBJECTS=$(patsubst $(SRC)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))
OBJECTS+=$(patsubst $(SRC)/%,$(OBJDIR)/%,$(ASOURCES:.S=.o))

# Define all listing files.
LST = $(OBJECTS:.o=.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Define Messages
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


# Default target.
all: gccversion sizebefore build sizeafter 

build: $(OBJDIR) elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJECTS)
%.elf: $(OBJECTS)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJECTS) --output $@ $(LDFLAGS)

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Display size of file.
#ELFSIZE = $(SIZE) -C  --mcu=$(MCU) $(TARGET).elf
ELFSIZE = $(SIZEM) $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi



# Display compiler version information.
gccversion : 
	@$(CC) --version

test:
	@echo $(SOURCES)
	@echo $(ASOURCES)
	@echo $(OBJECTS)

$(OBJDIR):
	mkdir $(OBJDIR)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : $(SRC)/%.cpp
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Compile: create assembler files from C source files.
$(OBJDIR)/%.s : $(SRC)/%.cpp
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : $(SRC)/%.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


load: load_ser
# you need to erase first before loading the program.
# load (program) the software into the flash:
load_par: $(TARGET).hex
	avrdude -p $(MCU) -c dapa -P $(PARPORT) -U flash:w:$(TARGET).hex

load_ser: $(TARGET).hex
	avrdude -p $(MCU) -c dasa -P $(SERIALPORT) -U flash:w:$(TARGET).hex


# Target: clean project.
clean: clean_list 

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(RM) $(TARGET).hex
	$(RM) $(TARGET).eep
	$(RM) $(TARGET).elf
	$(RM) $(TARGET).map
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).lss
	$(RM) $(OBJECTS)
	$(RM) $(LST)
	$(RM) .dep/*
	$(RM) $(SOURCES:.cpp=.s)
	$(RM) $(SOURCES:.cpp=.d)


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all sizebefore sizeafter gccversion \
build elf hex eep lss sym \
clean clean_list \
load load_ser load_par \
test
