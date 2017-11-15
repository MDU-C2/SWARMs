################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5cmp.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5hl.c 

OBJS += \
./src2/libjpeg-turbo/md5/md5.o \
./src2/libjpeg-turbo/md5/md5cmp.o \
./src2/libjpeg-turbo/md5/md5hl.o 

C_DEPS += \
./src2/libjpeg-turbo/md5/md5.d \
./src2/libjpeg-turbo/md5/md5cmp.d \
./src2/libjpeg-turbo/md5/md5hl.d 


# Each subdirectory must supply rules for building sources it contributes
src2/libjpeg-turbo/md5/md5.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/libjpeg-turbo/md5/md5cmp.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5cmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/libjpeg-turbo/md5/md5hl.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/libjpeg-turbo/md5/md5hl.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


