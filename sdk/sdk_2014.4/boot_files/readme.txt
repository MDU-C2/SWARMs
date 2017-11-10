connect arm hw
source C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/top_hw_platform_0/ps7_init.tcl
ps7_init
ps7_post_config
dow C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/u-boot.elf
dow -data C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/BOOT.bin 0x08000000
con

-----------------

sf probe 0 0 0
sf erase 0 0x01000000
sf write 0x08000000 0 0xFFFFFF


--------- JTAG boot
connect arm hw
source C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/top_hw_platform_0/ps7_init.tcl
ps7_init
ps7_post_config
source C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/stub.tcl
target 64
dow C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/u-boot.elf
con
---
Terminal - Avbryt boot
---
stop
dow -data C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/uImage 0x3000000
dow -data C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/boot_files/uramdisk.image.gz 0x2000000
dow -data C:/Users/cag01/Documents/mdh/vhdl/gimme2a/sdk_2014.4/device_tree_bsp_0/devicetree.dtb 0x2A00000

con
---
bootm 0x3000000 0x2000000 0x2A00000
(root root)

