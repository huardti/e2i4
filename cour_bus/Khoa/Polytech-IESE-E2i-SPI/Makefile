SHELL := /bin/bash

TOOLSDIR = ./Debug/devtools/
ELF = ./Debug/Polytech-IESE-E2i-SPI.elf

all: build

build:
	$(MAKE) -C ./Debug clean
	$(MAKE) -C ./Debug

dump : $(ELF)
	@echo "DUMP $<"
	@arm-none-eabi-objdump -h -t -d $(ELF)

load: $(ELF)
	@echo "LOAD $<"
	@$(TOOLSDIR)/stm32-load $<

debug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@chmod +x $(TOOLSDIR)/stm32-debug
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-debug $<

rawdebug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-gdb $<

minicom:
	@minicom -b9600 -D /dev/ttyACM0
