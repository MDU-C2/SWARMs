GIMME2 commands
===============


On host computer:
Login using ssh, 192.168.1.10, root:root
Launch display software, CVSharp


On GIMME2 (ssh):
The executable is called tmp.elf and can be found on the SD-card. To run it type:
cd /media/card
./tmp.elf

Keyboard shortcuts:
q - quit
w - toggle write (starts/stops writing images to the SD-card)
t - toggle camera (for display)
a,s,d,f - increase exposure time (1000,100,10,1 time units)
z,x,c,v - decrease exposure time (1000,100,10,1 time units)
e - toggle automatic exposure (default on)
g - increase gain
b - decrease gain
h - toggle Harris
r - toggle Stereo (presidence over h)
j - increase Harris corner threshold
m - decrease Harris corner threshold
k - increase Harris edge threshold
, - decrease Harris edge threshold


SDK Project
===========

The Xilinx SDK (Software Development Kit) is part of the Vivato suite desiged for software development for the Zynq (the ARM processor). The Vivado design flow can be devided into the following steps:
1. Hardware design - through coding (VHDL, Verilog) or block design (graphical programming)
2. Processing system - this is a special block in block design to describe the interface between the programmable logic (FPGA) and the processing system (CPU).
3. Software design - in SDK, eihter linux applications of stand alone. If a hardware desing includes a processing system, a hand off file is exported from Vivado, dictating the base settings for the software project.

For this project, the hardware is defined, and the system is configured to run linux. The main file is sdk_2014.4/tmp/src/view_dump_thread.c. It is designed to interface the hardware through virtual GPIO and 3 image streams; left, right and vision - where vision is a down-sampled, potentially processed, image stream to be sent over UDP.
  