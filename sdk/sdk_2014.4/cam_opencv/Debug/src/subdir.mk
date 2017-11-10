################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/udp_test.c 

OBJS += \
./src/udp_test.o 

C_DEPS += \
./src/udp_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


