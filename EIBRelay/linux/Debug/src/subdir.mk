################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/yosi/workspace/eibsuite/EIBRelay/src/EIBRelayServer.cpp \
/home/yosi/workspace/eibsuite/EIBRelay/src/Main.cpp \
/home/yosi/workspace/eibsuite/EIBRelay/src/RelayHandler.cpp \
/home/yosi/workspace/eibsuite/EIBRelay/src/RelayServerConfig.cpp 

OBJS += \
./src/EIBRelayServer.o \
./src/Main.o \
./src/RelayHandler.o \
./src/RelayServerConfig.o 

CPP_DEPS += \
./src/EIBRelayServer.d \
./src/Main.d \
./src/RelayHandler.d \
./src/RelayServerConfig.d 


# Each subdirectory must supply rules for building sources it contributes
src/EIBRelayServer.o: /home/yosi/workspace/eibsuite/EIBRelay/src/EIBRelayServer.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I"/home/yosi/workspace/eibsuite/jtc/include" -I"/home/yosi/workspace/eibsuite/EIBStdLib/include" -I"/home/yosi/workspace/eibsuite/EIBRelay/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Main.o: /home/yosi/workspace/eibsuite/EIBRelay/src/Main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I"/home/yosi/workspace/eibsuite/jtc/include" -I"/home/yosi/workspace/eibsuite/EIBStdLib/include" -I"/home/yosi/workspace/eibsuite/EIBRelay/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/RelayHandler.o: /home/yosi/workspace/eibsuite/EIBRelay/src/RelayHandler.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I"/home/yosi/workspace/eibsuite/jtc/include" -I"/home/yosi/workspace/eibsuite/EIBStdLib/include" -I"/home/yosi/workspace/eibsuite/EIBRelay/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/RelayServerConfig.o: /home/yosi/workspace/eibsuite/EIBRelay/src/RelayServerConfig.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I"/home/yosi/workspace/eibsuite/jtc/include" -I"/home/yosi/workspace/eibsuite/EIBStdLib/include" -I"/home/yosi/workspace/eibsuite/EIBRelay/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


