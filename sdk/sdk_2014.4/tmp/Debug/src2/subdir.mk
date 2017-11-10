################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/VDMA.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/axi_gpio.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/col_disc.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/exposure_reg.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/histogram.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/mem_op.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/tpg.c \
/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/vasa_udp_client.c 

OBJS += \
./src2/VDMA.o \
./src2/axi_gpio.o \
./src2/col_disc.o \
./src2/exposure_reg.o \
./src2/histogram.o \
./src2/mem_op.o \
./src2/tpg.o \
./src2/vasa_udp_client.o 

C_DEPS += \
./src2/VDMA.d \
./src2/axi_gpio.d \
./src2/col_disc.d \
./src2/exposure_reg.d \
./src2/histogram.d \
./src2/mem_op.d \
./src2/tpg.d \
./src2/vasa_udp_client.d 


# Each subdirectory must supply rules for building sources it contributes
src2/VDMA.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/VDMA.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/axi_gpio.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/axi_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/col_disc.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/col_disc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/exposure_reg.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/exposure_reg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/histogram.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/histogram.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/mem_op.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/mem_op.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/tpg.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/tpg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src2/vasa_udp_client.o: /home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src/vasa_udp_client.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I/home/charles/Mdh/CDT508/gimme2/sdk/sdk_2014.2/src -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


