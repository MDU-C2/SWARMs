path to bootgen on win64:
C:\Xilinx\SDK\2014.4\bin\unwrapped\win64.o\bootgen

bit-reverse files (on host), where boot.bif defines the paths
> C:\Xilinx\SDK\2014.4\bin\unwrapped\win64.o\bootgen -image boot.bif -split bin -o i BOOT.bin

The resulting set of files
2016-01-20  14:46            87 872 boot.bin
2016-01-20  14:46            20 480 devicetree.dtb.bin
2016-01-20  14:46         4 045 568 top.bit.bin
2016-01-20  14:46           388 544 u-boot.elf.bin
2016-01-20  14:46         3 866 624 uImage.bin
2016-01-20  14:46         5 834 968 uramdisk.image.gz.bin

Download top.bit.bin to Zynq

Running application before programming
root@zynq:~# cd /media/card/demo/
root@zynq:/media/card/demo# ./cam.elf
Bus error
root@zynq:/media/card/demo#

Program FPGA
root@zynq:/media/card# cat top.bit.bin > /dev/xdevcfg
root@zynq:/media/card#

Running application after programming
root@zynq:/media/card/demo# ./cam.elf

Map memory for Frame Buffer
Mapped memory for Frame Buffer
Map memory for Frame Buffer
Mapped memory for Frame Buffer
Map memory for Frame Buffer


root@zynq:/sys/firmware/devicetree/base/amba/devcfg@f8007000#