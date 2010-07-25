EIB_ROOT = $(shell pwd)

ifeq ($(arch),ARM)
	INSTALL_DIR := $(EIB_ROOT)/bin/arm
else
	INSTALL_DIR := $(EIB_ROOT)/bin
endif

CONFIG_DIR := $(EIB_ROOT)/config

$(shell touch $(EIB_ROOT)/.config)

$(EIB_ROOT)/.config :
	@ecolor "31;1" "ERROR: Please run 'make menuconfig' at least once before building EIB Suite."
	@exit 2

menuconfig:
	make -sC $(CONFIG_DIR)
	@$(CONFIG_DIR)/mconf Tconfig

include $(EIB_ROOT)/.config

#Libraries
EIB_LIB_DIRS-$(CONFIG_JTC_LIB)		+= jtc
EIB_LIB_DIRS-$(CONFIG_EIB_STD_LIB)	+= EIBStdLib
EIB_LIB_DIRS-$(CONFIG_GSM_LIB)      	+= GSM
#Executables
EIB_EXE_DIRS-$(CONFIG_EIB_SERVER)    	+= EIBServer
EIB_EXE_DIRS-$(CONFIG_WEB_SERVER)   	+= WEBServer
EIB_EXE_DIRS-$(CONFIG_AMX_SERVER)   	+= AMXServer
EIB_EXE_DIRS-$(CONFIG_RELAY_SERVER) 	+= EIBRelay
EIB_EXE_DIRS-$(CONFIG_SMS_SERVER)   	+= SMSServer

all: $(EIB_LIB_DIRS-y) $(EIB_EXE_DIRS-y)

$(EIB_LIB_DIRS-y): force_look	
	@ecolor "33;1" 'Building $@'
	make all -C "$(EIB_ROOT)/$@/linux"

$(EIB_EXE_DIRS-y): force_look
	@ecolor "33;1" 'Building $@'
	make all -C "$(EIB_ROOT)/$@/linux"

%_clean:
	@ecolor "33;1" 'Cleaning $(subst _clean,,$@)'
	make clean -C "$(EIB_ROOT)/$(subst _clean,,$@)/linux"

clean: $(EIB_LIB_DIRS-y:%=%_clean) $(EIB_EXE_DIRS-y:%=%_clean) $(EIB_LIB_DIRS-:%=%_clean) $(EIB_EXE_DIRS-:%=%_clean)
	@rm -rf $(INSTALL_DIR)
	@rm -f .config .tmpconfig.h .config.cmd

%_install:
	@ecolor "33;1" 'Installing $(subst _install,,$@)'
	@mkdir -p $(INSTALL_DIR)
	make install -C "$(EIB_ROOT)/$(subst _install,,$@)/linux" INSTALL_DIR=$(INSTALL_DIR)

install: all $(EIB_LIB_DIRS-y:%=%_install) $(EIB_EXE_DIRS-y:%=%_install)
	@ecolor "33;1" 'Installing tools & scripts...'	
	@mkdir -p $(INSTALL_DIR)
	@cp -u ./scripts/run.sh $(INSTALL_DIR)
	@cp -uR templates/ $(INSTALL_DIR)/templates
	@ecolor "32;1" '*************************************************************************'
	@ecolor "32;1" 'EIB Suite Compiled & installed successfully into "$(INSTALL_DIR)"'
	@ecolor "32;1" '*************************************************************************'

force_look:
	true

