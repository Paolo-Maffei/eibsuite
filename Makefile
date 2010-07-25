include Makefile.rules
-include $(EIB_ROOT)/.config

ifeq ($(CONFIG_CPU_ARM),y)
	INSTALL_DIR := $(EIB_ROOT)/bin/arm
	CC := /home/yosi/Desktop/gcc/bin/arm-none-linux-gnueabi-g++
else ifeq ($(CONFIG_CPU_X86),y)
	INSTALL_DIR := $(EIB_ROOT)/bin
	CC := g++
endif

ecolor := @$(EIB_ROOT)/scripts/ecolor
CONFIG_DIR := $(EIB_ROOT)/config

#Libraries
EIB_LIB_DIRS-$(CONFIG_JTC_LIB)          += jtc
EIB_LIB_DIRS-$(CONFIG_EIB_STD_LIB)      += EIBStdLib
EIB_LIB_DIRS-$(CONFIG_GSM_LIB)          += GSM
#Executables
EIB_EXE_DIRS-$(CONFIG_EIB_SERVER)       += EIBServer
EIB_EXE_DIRS-$(CONFIG_WEB_SERVER)       += WEBServer
EIB_EXE_DIRS-$(CONFIG_AMX_SERVER)       += AMXServer
EIB_EXE_DIRS-$(CONFIG_RELAY_SERVER)     += EIBRelay
EIB_EXE_DIRS-$(CONFIG_SMS_SERVER)       += SMSServer

menuconfig :
	make -sC $(CONFIG_DIR)
	@$(CONFIG_DIR)/mconf Tconfig

clean : $(EIB_LIB_DIRS-y:%=%_clean) $(EIB_EXE_DIRS-y:%=%_clean) $(EIB_LIB_DIRS-:%=%_clean) $(EIB_EXE_DIRS-:%=%_clean)
	$(ecolor) "33;1" 'Cleaning tools & scripts...'
	@rm -rf $(INSTALL_DIR)
	@rm -f .config .tmpconfig.h .config.cmd

all: $(EIB_ROOT)/.config $(EIB_LIB_DIRS-y) $(EIB_EXE_DIRS-y)

$(EIB_LIB_DIRS-y): force_look	
	$(ecolor) "33;1" 'Building $@'
	make all -C "$(EIB_ROOT)/$@/linux" CC=$(CC)

$(EIB_EXE_DIRS-y): force_look
	$(ecolor) "33;1" 'Building $@'
	make all -C "$(EIB_ROOT)/$@/linux" CC=$(CC)

%_clean:
	$(ecolor) "33;1" 'Cleaning $(subst _clean,,$@)'
	make clean -C "$(EIB_ROOT)/$(subst _clean,,$@)/linux"

%_install:
	$(ecolor) "33;1" 'Installing $(subst _install,,$@)'
	@mkdir -p $(INSTALL_DIR)
	make install -C "$(EIB_ROOT)/$(subst _install,,$@)/linux" INSTALL_DIR=$(INSTALL_DIR) CC=$(CC)

install: $(EIB_ROOT)/.config all $(EIB_LIB_DIRS-y:%=%_install) $(EIB_EXE_DIRS-y:%=%_install)
	$(ecolor) "33;1" 'Installing tools & scripts...'	
	@mkdir -p $(INSTALL_DIR)
	@cp -u ./scripts/run.sh $(INSTALL_DIR)
	@cp -uR templates/ $(INSTALL_DIR)/templates
	$(ecolor) "32;1" '**********************************************************************************'
	$(ecolor) "32;1" 'EIB Suite Compiled & installed successfully into "$(INSTALL_DIR)"'
	$(ecolor) "32;1" '**********************************************************************************'

$(EIB_ROOT)/.config : 
	$(ecolor) "31;1" "ERROR: Please run 'make menuconfig' at least once before building EIB Suite."
	@exit 2


force_look:
	true

