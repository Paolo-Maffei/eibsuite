ifeq ($(arch),ARM)
	INSTALL_DIR := $(shell pwd)/bin/arm
else
	INSTALL_DIR := $(shell pwd)/bin
endif

EXE_PROJECTS := EIBServer WEBServer AMXServer EIBRelay SMSServer
LIB_PROJECTS := jtc EIBStdLib GSM
PWD = $(shell pwd)

all: $(LIB_PROJECTS) $(EXE_PROJECTS)

$(LIB_PROJECTS): force_look	
	@echo 'Building $@'
	make all -C "$(PWD)/$@/linux"

$(EXE_PROJECTS): force_look
	@echo 'Building $@'
	make all -C "$(PWD)/$@/linux"

%_clean:
	@echo 'Cleaning $(subst _clean,,$@)'
	make clean -C "$(PWD)/$(subst _clean,,$@)/linux"

clean: $(LIB_PROJECTS:%=%_clean) $(EXE_PROJECTS:%=%_clean)
	@rm -rf $(INSTALL_DIR)

%_install:
	@mkdir -p $(INSTALL_DIR)
	make install -C "$(PWD)/$(subst _install,,$@)/linux" INSTALL_DIR=$(INSTALL_DIR)

install: all $(LIB_PROJECTS:%=%_install) $(EXE_PROJECTS:%=%_install)
	@echo 'Installing tools & scripts'	
	@cp -u ./scripts/run.sh $(INSTALL_DIR)
	@cp -uR templates/ $(INSTALL_DIR)/templates
	@echo '*************************************************************************'
	@echo 'EIB Suite installed successfully into "$(INSTALL_DIR)"'
	@echo '*************************************************************************'	

force_look:
	true

