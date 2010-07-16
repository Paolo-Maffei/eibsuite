-include ../../Makefile.rules

INCLUDE_DIRS := ../include ../include/conf ../../jtc/include ../../EIBStdLib/include
INCLUDE_LIBS := ../../jtc/linux ../../EIBStdLib/linux
OUT_EXE := AMXServer

# All of the sources participating in the build are defined here
-include objects.mk

# All Target
all: $(OUT_EXE)

%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(INCLUDE_DIRS:%=-I"%") -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $@'
	@echo 

$(OUT_EXE): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) $(INCLUDE_LIBS:%=-L"%") -o"$(OUT_EXE)" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
install:
	@$(CP) $(OUT_EXE) $(INSTALL_DIR)

clean:
	-$(RM) $(OBJS) $(OBJS:%.o=%.d) $(OUT_EXE)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

