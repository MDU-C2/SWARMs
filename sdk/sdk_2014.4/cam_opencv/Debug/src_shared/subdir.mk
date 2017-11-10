################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/VDMA.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/axi_gpio.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/col_disc.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/exposure_reg.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/histogram.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/mem_op.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/tpg.c \
C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/vasa_udp_client.c 

OBJS += \
./src_shared/VDMA.o \
./src_shared/axi_gpio.o \
./src_shared/col_disc.o \
./src_shared/exposure_reg.o \
./src_shared/histogram.o \
./src_shared/mem_op.o \
./src_shared/tpg.o \
./src_shared/vasa_udp_client.o 

C_DEPS += \
./src_shared/VDMA.d \
./src_shared/axi_gpio.d \
./src_shared/col_disc.d \
./src_shared/exposure_reg.d \
./src_shared/histogram.d \
./src_shared/mem_op.d \
./src_shared/tpg.d \
./src_shared/vasa_udp_client.d 


# Each subdirectory must supply rules for building sources it contributes
src_shared/VDMA.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/VDMA.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/axi_gpio.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/axi_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/col_disc.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/col_disc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/exposure_reg.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/exposure_reg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/histogram.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/histogram.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/mem_op.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/mem_op.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/tpg.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/tpg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_shared/vasa_udp_client.o: C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.2/src/vasa_udp_client.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux gcc compiler'
	arm-xilinx-linux-gnueabi-gcc -Wall -O0 -g3 -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include" -I"C:\Xilinx\SDK\2014.4\data\embeddedsw\ThirdParty\opencv\include\opencv" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.2\src" -I"C:\Users\cag01\Documents\mdh\vhdl\gimme2a\sdk_2014.4\fsbl_bsp\ps7_cortexa9_0\include" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


