-include ../../Makefile.rules

INCLUDE_DIRS := ../include ../include/xml ../../jtc/include 
INCLUDE_LIBS := ../../jtc/linux
OUT_LIB := libEIBStdLib.so

# All of the sources participating in the build are defined here
-include objects.mk
DEPS := $(OBJS:%.o=%.d)
-include $(DEPS)

# All Target
all: $(OUT_LIB)

xml/%.o: ../src/xml/%.cpp
	@mkdir -p xml
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CPP_COMPILER) $(INCLUDE_DIRS:%=-I"%") -D$(ARCH) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $@'
	@echo 

%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CPP_COMPILER) $(INCLUDE_DIRS:%=-I"%") -D$(ARCH) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $@'
	@echo 

$(OUT_LIB): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CPP_LINKER) $(INCLUDE_LIBS:%=-L"%") -shared -o"$(OUT_LIB)" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
install:
	@$(CP) $(OUT_LIB) $(INSTALL_DIR)

clean:
	-$(RM) $(OBJS) $(OBJS:%.o=%.d) $(OUT_LIB)
	-$(RM) xml
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

