/*
 * VDMA.c
 *
 *  Created on: 23 maj 2013
 *      Author: Emil
 */

//#include "linux_drivers.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "VDMA.h"
#include "hw_base.h"
#include <stdbool.h>
#include "mem_op.h"

#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>

extern int errno;

uint32_t WIDTH = 640;
uint32_t HEIGHT = 480;
uint32_t DEADBEEF = 0xDEADBEEF;

#define BMPFILEHEADER_SIZE 14
#define BMPINFOHEADER_SIZE 40
#define BMPINFOHEADER_SIZE_32 108

uint32_t BPP = 4;


int SaveBmpToSd = 0;
int SaveJpgToSd = 0;


//#define BPP 4 //bytes per pixel
//#define bpp 3 //bytes per pixel


void SendTcpImageFromServer(int socket, char* filename){

FILE *picture;
int size, read_size, stat, stat2, packet_index;
char send_buffer[10240], read_buffer[256];
packet_index = 1;
printf("FILENAME: %s\n",filename);
picture = fopen(filename, "r");
printf("Getting Picture Size\n");

if(picture == NULL) {
     printf("Error Opening Image File"); }

fseek(picture, 0, SEEK_END);
size = ftell(picture);
fseek(picture, 0, SEEK_SET);
printf("Total Picture size: %i\n",size);

//Send Picture Size
printf("Sending Picture Size\n");
write(socket, (void *)&size, sizeof(int));

//Send Picture as Byte Array
printf("Sending Picture as Byte Array\n");

do { //Read while we get errors that are due to signals.
   stat=read(socket, &read_buffer , 255);   //read verification of size
   printf("Bytes read: %i\n",stat);
} while (stat < 0);


void *p = filename;
int bytes_read = 18; //this is the length of the filename
printf("bytes_read %i\n",bytes_read);


	int fn_check = send(socket, filename, bytes_read,0);
	printf("fn_check: %i\n",fn_check);
	if (fn_check <= 0){
		printf("something wrong in server!\n");
	}

	printf("sending!\n");


char read_veri[256];
//read the verification that the name is received
do{
	stat2 = read(socket, &read_veri, 255);
	printf("verification of filename received: %i\n", stat2);
}while(stat2 < 0);


printf("Received data in socket\n");

while(!feof(picture)) {
   //Read from the file into our send buffer
   read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

   //Send data through our socket
   do{
     stat = write(socket, send_buffer, read_size);
   }while (stat < 0);

   printf("Packet Number: %i\n",packet_index);
   printf("Packet Size Sent: %i\n",read_size);
   printf(" \n");
   printf(" \n");


   packet_index++;

   //Zero out the send buffer
   bzero(send_buffer, sizeof(send_buffer));
  }

char im_veri[256];
do {
	   stat2 = read(socket, &im_veri, 255);
}while(stat2 < 0);
printf("Image received on the server side, sending next pic\n");
}



void SendTcpImgDataFromServer(int socket, unsigned char *memo_data, char *name, unsigned long mem_size){

char *pic = NULL;
int read_size, stat,stat2, packet_index;
char send_buffer[10240], read_buffer[256];
packet_index = 1;

pic = memo_data;
printf("Getting Picture Size\n");

if(pic == NULL) {
     printf("Error opening pic\n"); }


size_t size = mem_size;
printf("Total Picture size: %i\n",size);

//Send Picture Size
printf("Sending picture size\n");
write(socket, (void *)&size, sizeof(int)); //sending size of the data

//Send Picture as Byte Array
printf("Sending picture as byte array\n");

do { //Read while we get errors that are due to signals.
   stat=read(socket, &read_buffer , 255); //read verification of size
   printf("Bytes read: %i\n",stat);
} while (stat < 0);

printf("Received data in socket\n");


void *p = name;
int bytes_read = 18; //this is the length of the filename
printf("bytes_read %i\n",bytes_read);


	int fn_check = send(socket, name, bytes_read,0);
	printf("fn_check: %i\n",fn_check);
	if (fn_check <= 0){
		printf("something wrong in server!\n");
	}
	bytes_read -= fn_check;
	p += fn_check;
	printf("sending!\n");


char read_veri[256];
//read the verification that the name is received
do{
	stat2 = read(socket, &read_veri, 255);
	printf("verification of filename received: %i\n", stat2);
}while(stat2 < 0);


int offs = 10240;
int index=0;
while (index < size){
   //Send data through the socket

	if(size - index < offs){
		offs = size - index;
	}
	int sent_bytes = send(socket, memo_data+index, offs,0 );
index = index + offs;

   packet_index++;

}

   //Zero out our send buffer
   bzero(send_buffer, sizeof(send_buffer));

char im_veri[256];
do {
	   stat2 = read(socket, &im_veri, 255);
}while(stat2 < 0);
printf("Image received on the server side, sending the next pic\n");

 }




int InitTcpImgDataServer(unsigned char *memory_data, char *name, unsigned long mem_size)

{
  int socket_desc, new_socket, size_struct;
  struct sockaddr_in server, client;

  //Create a socket
  socket_desc = socket(PF_INET , SOCK_STREAM , 0); //ipv4 and tcp. should work with AF_INET as well
  if (socket_desc == -1)
  {
     printf("Could not create socket");
  }

  printf("socket returns socket_desc: %d\n",socket_desc);

  int optval;
  // set the SO_REUSEADDR on the socket to true
  optval = 1;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);//reuses the address between the sending of image pairs



  //Assign the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 8765 );


  int bind_result = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
  if (bind_result < 0){
	  puts("bind failed");
	  int errnum = errno; //assign the error nr
	  fprintf(stderr, "value of errno: %d\n", errno);
	  perror("the reason of errno");
	  fprintf(stderr, "error binding: %s\n", strerror(errnum));
	  return 1;
  }

 puts("bind done");


 //Listen to socket
 listen(socket_desc , 3);


  puts("Waiting for incoming connections...");
  size_struct = sizeof(struct sockaddr_in);

  //Accept the connection for client
 if((new_socket = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&size_struct))){
puts("Connection accepted");
     }

fflush(stdout);

if (new_socket<0)
{
  perror("Accept Failed");
  return 1;
}

//init finished, now send the image pair

SendTcpImgDataFromServer(new_socket, memory_data, name, mem_size);

close(socket_desc);
fflush(stdout);
return 0;
}


int InitTcpImageServer(char *filename0)
{
  int socket_desc, new_socket, size_struct;
  struct sockaddr_in server, client;

  //Create a socket
  socket_desc = socket(PF_INET , SOCK_STREAM , 0); //ipv4 and tcp   //changed AF_INET to PF_INET
  if (socket_desc == -1)
  {
     printf("Could not create socket");
  }

  printf("socket returns socket_desc: %d\n",socket_desc);

  int optval;
  // set the SO_REUSEADDR on the socket to true
  optval = 1;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);//reuses the address
//between the sending of image pairs



  //Assign the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 8765 );


  int bind_result = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
  if (bind_result < 0){
	  puts("bind failed");
	  int errnum = errno; //assign the error nr
	  fprintf(stderr, "value of errno: %d\n", errno);
	  perror("the reason of errno");
	  fprintf(stderr, "error binding: %s\n", strerror(errnum));
	  //printf("bind returns: %d\n",bind_result);
	  return 1;
  }

 puts("bind done");


 //Listen to socket
 listen(socket_desc , 3);


  puts("Waiting for incoming connections...");
  size_struct = sizeof(struct sockaddr_in);

  //Accept the connection for client
 if((new_socket = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&size_struct))){
puts("Connection accepted");
     }

fflush(stdout);

if (new_socket<0)
{
  perror("Accept Failed");
  return 1;
}


//init finished, now send the image pair

SendTcpImageFromServer(new_socket, filename0);


close(socket_desc);
fflush(stdout);
return 0;
}



void SetSize (uint32_t w, uint32_t h, uint32_t bpp)
{
	WIDTH = w;
	HEIGHT = h;
	BPP = bpp;
}


int CheckAndClearErrors(uint32_t BaseAdress)
{
   uint32_t inErrors;
   uint32_t outErrors;
   uint32_t Errors;

	int map_len = 0xFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAdress);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
   	}

   inErrors  = REG_READ(VDMA_base, VDMA_S2MM_STATUS ) & 0x0000CFF0;
   outErrors = REG_READ(VDMA_base, VDMA_MM2S_STATUS ) & 0x000046F0;
//   printf( "AXI_VDMA - Checking Error Flags\n\r" );

   Errors = (inErrors << 16) | (outErrors);

   if ( Errors )
   {	//WC = Write Clear, RO = Read Only

	   //S2MM channel status
	   if ( inErrors & 0x00000001 ) //bit 4 WC
          printf( "\tS2MM_VDMASR - Halted\n\r" );
	   if ( inErrors & 0x00000010 ) //bit 4 WC
          printf( "\tS2MM_VDMASR - DMAIntErr\n\r" );
	   if ( inErrors & 0x00000020 ) //bit 5 RO
          printf( "\tS2MM_VDMASR - DMASlvErr\n\r" );
	   if ( inErrors & 0x00000040 ) //bit 6 RO
          printf( "\tS2MM_VDMASR - DMADecErr\n\r" );
	   if ( inErrors & 0x00000080 ) //bit 7 RO or WC
          printf( "\tS2MM_VDMASR - SOFEarlyErr\n\r" );
	   if ( inErrors & 0x00000100 ) //bit 8 WC
          printf( "\tS2MM_VDMASR - EOLEarlyErr\n\r" );
//	   if ( inErrors & 0x00000200 ) //bit 9
//          printf( "\tS2MM_VDMASR - SGSlvErr\n\r" );
//	   if ( inErrors & 0x00000400 ) //bit 10
//          printf( "\tS2MM_VDMASR - SGDecErr\n\r" );
	   if ( inErrors & 0x00000800 ) //bit 11 WC
          printf( "\tS2MM_VDMASR - SOFLateErr\n\r" );
	   if ( inErrors & 0x00004000 ) //bit 14 WC
          printf( "\tS2MM_VDMASR - ErrIrq\n\r" );
	   if ( inErrors & 0x00008000 ) //bit 15 WC
          printf( "\tS2MM_VDMASR - EOLLateErr\n\r" );

	   //MM2S Channel status
	   if ( outErrors & 0x00000010 ) //bit 4 WC
			 printf( "\tMM2S_VDMASR - DMAIntErr\n\r" );
	   if ( outErrors & 0x00000020 ) //bit 5 RO
			 printf( "\tMM2S_VDMASR - DMASlvErr\n\r" );
	   if ( outErrors & 0x00000040 ) //bit 6 RO
			 printf( "\tMM2S_VDMASR - DMADecErr\n\r" );
	   if ( outErrors & 0x00000080 ) //bit 7 RO or WC
			 printf( "\tMM2S_VDMASR - SOFEarlyErr\n\r" );
	   if ( outErrors & 0x00004000 ) //bit 14 WC
			 printf( "\tMM2S_VDMASR - ErrIrq\n\r" );

	   // Clear error flags
	   printf( "AXI_VDMA - Clearing Error Flags\n\r" );//bit 15---11-98 7654 3210
	   REG_WRITE(VDMA_base, VDMA_S2MM_STATUS, 0x0000C990);  //1100 1001 1001 0000
	   REG_WRITE(VDMA_base, VDMA_MM2S_STATUS, 0x00004090);  //0100 0000 1001 0000
//	   REG_WRITE(VDMA_base, XAXIVDMA_RX_OFFSET+XAXIVDMA_SR_OFFSET, 0x0000CFF0);
//	   REG_WRITE(VDMA_base, XAXIVDMA_TX_OFFSET+XAXIVDMA_SR_OFFSET, 0x000046F0);
   }
//   else
//	   printf("No errors found!\n");

   munmap((void *)VDMA_base, map_len);
   close(fd);
   return Errors;
}

int DumpRegisters(uint32_t uBaseAddr)
{

	int map_len = 0xFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)uBaseAddr);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

   // Partial Register Dump
   printf( "AXI_VDMA - Partial Register Dump (uBaseAddr = 0x%08X):\n\r", uBaseAddr );
   printf( "\t PARKPTR          = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_PARK_PTR_REG));
   printf( "\t --------S2MM Regs--------\n\r" );
   printf( "\t S2MM_DMACR       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_CTRL));
   printf( "\t S2MM_DMASR       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_STATUS) );
   printf( "\t S2MM_STRD_FRMDLY = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_FRMDLY_STRIDE) );
   printf( "\t S2MM_START_ADDR0 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_START_ADDRESS+0) );
   printf( "\t S2MM_START_ADDR1 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_START_ADDRESS+4) );
   printf( "\t S2MM_START_ADDR2 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_START_ADDRESS+8) );
   printf( "\t S2MM_HSIZE       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_HSIZE) );
   printf( "\t S2MM_VSIZE       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_S2MM_VSIZE) );
   printf("\n");
   printf( "\t --------MM2S Regs--------\n\r" );
   printf( "\t MM2S_DMACR       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_CTRL));
   printf( "\t MM2S_DMASR       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_STATUS) );
   printf( "\t MM2S_STRD_FRMDLY = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_FRMDLY_STRIDE) );
   printf( "\t MM2S_START_ADDR0 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_START_ADDRESS+0) );
   printf( "\t MM2S_START_ADDR1 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_START_ADDRESS+4) );
   printf( "\t MM2S_START_ADDR2 = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_START_ADDRESS+8) );
   printf( "\t MM2S_HSIZE       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_HSIZE) );
   printf( "\t MM2S_VSIZE       = 0x%08X\n\r", REG_READ(VDMA_base, VDMA_MM2S_VSIZE) );

   munmap((void *)VDMA_base, map_len);
   close(fd);
   return 0;
}

//Fills the memory at [FB_addr] with [WIDTH*HEIGHT*bpp] bytes of 0xDEADBEEF
void InitFB(off_t FB_addr, uint16_t width, uint16_t height, uint16_t bpp)
{
	int i = 0;
	int map_len = (width*height*bpp);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));

	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return;
	}
	printf("\nMap memory for Frame Buffer");
	unsigned char* FB_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)FB_addr);
	printf("\nMapped memory for Frame Buffer");
	if(FB_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	for(i = 0; i < (width*height); i++)
	{
//		REG_WRITE_T(FB_base,i, 0x0F, uint8_t);
		REG_WRITE(FB_base,(i*bpp), DEADBEEF);
	}

	munmap((void *)FB_base, map_len);

	close(fd);
	return;
}

/* Prints out [resolution] equally spaced out locations
between [BaseAddress] and [BaseAddress]+[memLen] in physical memory */
int CheckResults(uint32_t BaseAddress, int memLen, int resolution)
{
	int map_len = (WIDTH*HEIGHT*BPP);
	int i = 0;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	int cnt = 0;
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	for(i = 0; i < WIDTH*HEIGHT; i++)
	{
//			if(col_equal(REG_READ_T(mem_base,(i*BPP),color_t), DEADBEEF))
		if (REG_READ(mem_base,(i*BPP)) == DEADBEEF)
			{
				cnt++;
			}
	}
	printf("\nTotal number of DEADBEEF on FB: %d / %d\n", cnt, WIDTH*HEIGHT);

	int memStep = BPP; //memLen/resolution - (memLen/resolution)%8; //4

	for(i = 0; i < resolution; i++)
	{
//		printf("At %x is: %X\n", BaseAddress+(i*memStep), REG_READ_T(mem_base,(i*memStep),color_t).c);
		printf("At %x is: %X\n", BaseAddress+(i*memStep), REG_READ(mem_base,(i*memStep)));
	}

	munmap((void *)mem_base, map_len);
	close(fd);
	return 0;
}




#define MIN(a,b) (((a)<(b))?(a):(b))

#include "message.h"
#include "vasa_udp_client.h"

//Send image over UDP -- requires UDP to be initialized
int SendImage(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp)
{
	int map_len = (width*height*bpp);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	char* mem_base = (char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	uint32_t seq = 0;
	message_t msg;


	msg.cam_id = 0; //DEFINITION OF FB HAS CHANGED
//	if (BaseAddress == FB0_ADDR)
//		msg.cam_id = 0;
//	else if (BaseAddress == FB1_ADDR)
//		msg.cam_id = 1;
//	else
//		msg.cam_id = 255;




	while (width*height*bpp > seq*PIXELS_PER_FRAME)
	{
		memcpy(&msg.pixels, (mem_base+seq*PIXELS_PER_FRAME), PIXELS_PER_FRAME);
		msg.seq_no = seq++;
//		printf ("%d %d %d %d\n",seq,width,height,bpp);
//		if (seq == 2)
//			for (i=0;i<PIXELS_PER_FRAME;i++)
//				printf ("%d; %08x\n",i,msg.pixels[i]);
		if (sendto_vasa (&msg) == -1) {
			return -1;
			printf("Failed sending frame_seq: %d to vasa",seq);
		}
		usleep(5);
	}
//	printf("%d\n",seq);
	munmap((void *)mem_base, map_len);
	close(fd);
	return 0;

}

//Send image over UDP -- requires UDP to be initialized
int SendImage_mem(image_t* img, char id)
{
	unsigned int seq = 0;
	message_t msg;
	msg.cam_id = id;

	while (img->w*img->h*img->bpp > seq*PIXELS_PER_FRAME)
	{
		memcpy(&msg.pixels, (img->data+seq*PIXELS_PER_FRAME), PIXELS_PER_FRAME);
		msg.seq_no = seq++;
//		printf ("%d ",seq);
//		if (seq == 2)
//			for (i=0;i<PIXELS_PER_FRAME;i++)
//				printf ("%d; %08x\n",i,msg.pixels[i]);
		if (sendto_vasa (&msg) == -1) {
			return -1;
			printf("Failed sending frame_seq: %d to vasa",seq);
		}
	}

	return 0;
}


int SaveJpgImage(uint32_t BaseAddress,uint32_t BaseAddress1, char* file_name,char* file_name1, uint16_t width, uint16_t height, uint16_t bpp, uint8_t scale)
{


char filename[20];
char filename1[20];
memset(filename, '\0', sizeof(filename));
memset(filename1, '\0', sizeof(filename));

	char mem_arr[width*height*bpp];
	char mem_arr1[width*height*bpp];

	int map_len = (width*height*bpp);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
	unsigned char* mem_base1 = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress1);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}
	if(mem_base1 == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}


	strncpy(filename,file_name,strlen(file_name)-3); //change extension to jpg
	strncpy(filename1,file_name1,strlen(file_name1)-3);

	strcat(filename,"jpg");
	strcat(filename1,"jpg");

	memcpy(&mem_arr, mem_base, map_len); //copy the image data from the shared memory, from start address mem_base, copy the length of map_len to the char array mem_arr
	memcpy(&mem_arr1, mem_base1, map_len);


	ConvertImage(filename, mem_arr, width, height, bpp); //send img data to this function, the img file is created and saved to sd card
	ConvertImage(filename1, mem_arr1, width, height, bpp);

	InitTcpImageServer(filename);
	InitTcpImageServer(filename1);


	printf("Image saved...%s \n",filename);
	printf("Image saved...%s \n",filename1);

	munmap((void *)mem_base, map_len); // deallocate the space of the image elements in shared mem when sent
	munmap((void *)mem_base1, map_len);

	close(fd);


	return 0;

}



int SaveJpgImageData(uint32_t BaseAddress,uint32_t BaseAddress1, char* file_name,char* file_name1, uint16_t width, uint16_t height, uint16_t bpp, uint8_t scale)
{
	char filename[strlen(file_name)];
	char filename1[strlen(file_name1)];

	char mem_arr[width*height*bpp];
	char mem_arr1[width*height*bpp];

	int map_len = (width*height*bpp);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
	unsigned char* mem_base1 = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress1);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}
	if(mem_base1 == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	strncpy(filename,file_name,strlen(file_name)-3); //change extension to jpg
	strncpy(filename1,file_name1,strlen(file_name1)-3);

	strcat(filename,"jpg");
	strcat(filename1,"jpg");


	memcpy(&mem_arr, mem_base, map_len); //copy the image data from the shared memory, from start address mem_base, copy the length of map_len to the char array mem_arr
	memcpy(&mem_arr1, mem_base1, map_len);

	unsigned char mem[400000];
	unsigned long mem_size = 0;
	unsigned char mem1[400000];
	unsigned long mem_size1 = 0;

	ConvertImageData(filename, mem_arr, width, height, bpp, mem, &mem_size);
	ConvertImageData(filename1, mem_arr1, width, height, bpp, mem1, &mem_size1);

	InitTcpImgDataServer(mem, filename, mem_size);
	InitTcpImgDataServer(mem1, filename1, mem_size1);


	printf("Image saved...%s \n",filename);
	printf("Image saved...%s \n",filename1);

	munmap((void *)mem_base, map_len); // deallocate the space of the image elements in shared mem when sent
	munmap((void *)mem_base1, map_len);
	close(fd);

	return 0;

}


int SaveBmpImage(uint32_t BaseAddress,uint32_t BaseAddress1, char* filename,char* filename1, uint16_t width, uint16_t height, uint16_t bpp, uint8_t scale)
{


	int map_len = (width*height*bpp);
	int map_len1 = (width*height*bpp);

	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
	unsigned char* mem_base1 = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress1);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	if(mem_base1 == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}



	FILE *ofp;
	FILE *ofp1;

	ofp = fopen(filename, "w");
	ofp1 = fopen(filename1, "w");

	if (ofp == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}
	if (ofp1 == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}

	uint32_t w = width/scale;
	uint32_t h = height/scale;
	uint32_t filesize = w*h*bpp + 54;
//	uint32_t pix;
//	uint8_t col;


	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0,
										0,0,0,0,
										0,0,0,0,
										1,0,
										24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize      );
	bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
	bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
	bmpfileheader[ 5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[ 4] = (unsigned char)(       w      );
	bmpinfoheader[ 5] = (unsigned char)(       w >>  8);
	bmpinfoheader[ 6] = (unsigned char)(       w >> 16);
	bmpinfoheader[ 7] = (unsigned char)(       w >> 24);
	bmpinfoheader[ 8] = (unsigned char)(       h      );
	bmpinfoheader[ 9] = (unsigned char)(       h >>  8);
	bmpinfoheader[10] = (unsigned char)(       h >> 16);
	bmpinfoheader[11] = (unsigned char)(       h >> 24);

	fwrite(bmpfileheader,1,14,ofp);
	fwrite(bmpinfoheader,1,40,ofp);
	fwrite(mem_base,1,w*h*bpp,ofp); //the elements from shared mem

	fwrite(bmpfileheader,1,14,ofp1);
	fwrite(bmpinfoheader,1,40,ofp1);
	fwrite(mem_base1,1,w*h*bpp,ofp1);


	fclose(ofp);
	fclose(ofp1);

	printf("Image saved...%s \n",filename);
	printf("Image saved...%s \n",filename1);

	munmap((void *)mem_base, map_len); // deallocate the space of the image elements in shared mem when sent
	munmap((void *)mem_base1, map_len);

	close(fd);

	InitTcpImageServer(filename);
	InitTcpImageServer(filename1);

	return 0;

}



int SaveBmpImageData(uint32_t BaseAddress,uint32_t BaseAddress1, char* filename,char* filename1, uint16_t width, uint16_t height, uint16_t bpp, uint8_t scale) //cant be saved to the gimme2 sdcard since the img data is sent away
{

	char mem_arr[width*height*bpp];

	char mem_arr1[width*height*bpp];

	int map_len = (width*height*bpp);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	unsigned char* mem_base1 = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress1);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}
	if(mem_base1 == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}



	//the next four rows are for sending image data and not full images. the image file is created in the client on the odroid.
	memcpy(&mem_arr, mem_base, map_len); //copy the image data from the shared memory, from start address mem_base, copy the length of map_len to the char array mem_arr

	memcpy(&mem_arr1, mem_base1, map_len);

	InitTcpImgDataServer(mem_arr, filename, map_len);

	InitTcpImgDataServer(mem_arr1, filename1, map_len);


	printf("Image saved...%s \n",filename);

	munmap((void *)mem_base, map_len); // deallocate the space of the image elements in shared mem when sent

	munmap((void *)mem_base1, map_len);
	close(fd);


	return 0;

}




//Save image from in RAM at [BaseAddress] to disk as [filename]
int SaveImage32(uint32_t BaseAddress, char* filename)
{
	int map_len = (WIDTH*HEIGHT*4);
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	FILE *ofp;

	ofp = fopen(filename, "w");

	if (ofp == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}

	uint32_t w = WIDTH;
	uint32_t h = HEIGHT;
	uint32_t datasize = w*h*BPP;
	uint32_t filesize = datasize + 122;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 122,0,0,0};
	unsigned char bmpinfoheader[108] = {108,0,0,0,
										0,0,0,0,
										0,0,0,0,
										1,0,
										32,0,
										3,0,0,0,
										0,0,0,0, //size
										19,11,0,0,
										19,11,0,0,
										0,0,0,0,
										0,0,0,0,
										0,0,255,0,
										0,255,0,0,
										255,0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize      );
	bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
	bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
	bmpfileheader[ 5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[ 4] = (unsigned char)(       w      );
	bmpinfoheader[ 5] = (unsigned char)(       w >>  8);
	bmpinfoheader[ 6] = (unsigned char)(       w >> 16);
	bmpinfoheader[ 7] = (unsigned char)(       w >> 24);
	bmpinfoheader[ 8] = (unsigned char)(       h      );
	bmpinfoheader[ 9] = (unsigned char)(       h >>  8);
	bmpinfoheader[10] = (unsigned char)(       h >> 16);
	bmpinfoheader[11] = (unsigned char)(       h >> 24);

	fwrite(bmpfileheader,1,14,ofp);
	fwrite(bmpinfoheader,1,108,ofp);
	fwrite(mem_base,1,datasize,ofp);

	fclose(ofp);
	printf("Image saved...\n");

	munmap((void *)mem_base, map_len);
	close(fd);
	return 0;

}

int SaveImage_mem(image_t* im, char* filename)
{

	uint32_t datasize = im->w*im->h*im->bpp;


	FILE *ofp;

	ofp = fopen(filename, "w");

	if (ofp == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}


	if (im->bpp == 4) { // 32-bit bmp

	uint32_t filesize = datasize + BMPFILEHEADER_SIZE + BMPINFOHEADER_SIZE_32;

		unsigned char bmpfileheader[BMPFILEHEADER_SIZE] = {'B','M', 0,0,0,0, 0,0, 0,0, BMPFILEHEADER_SIZE+BMPINFOHEADER_SIZE_32,0,0,0};
		unsigned char bmpinfoheader[BMPINFOHEADER_SIZE_32] = {BMPINFOHEADER_SIZE_32,0,0,0,
											0,0,0,0,
											0,0,0,0,
											1,0,
											32,0,
											3,0,0,0,
											0,0,0,0, //size
											19,11,0,0,
											19,11,0,0,
											0,0,0,0,
											0,0,0,0,
											0,0,255,0,
											0,255,0,0,
											255,0,0,0};

		bmpfileheader[ 2] = (unsigned char)(filesize      );
		bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
		bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
		bmpfileheader[ 5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[ 4] = (unsigned char)(       im->w      );
		bmpinfoheader[ 5] = (unsigned char)(       im->w >>  8);
		bmpinfoheader[ 6] = (unsigned char)(       im->w >> 16);
		bmpinfoheader[ 7] = (unsigned char)(       im->w >> 24);
		bmpinfoheader[ 8] = (unsigned char)(       im->h      );
		bmpinfoheader[ 9] = (unsigned char)(       im->h >>  8);
		bmpinfoheader[10] = (unsigned char)(       im->h >> 16);
		bmpinfoheader[11] = (unsigned char)(       im->h >> 24);

		uint32_t info_header_size = BMPINFOHEADER_SIZE_32;

		fwrite(bmpfileheader,1,BMPFILEHEADER_SIZE,ofp);
		fwrite(bmpinfoheader,1,info_header_size,ofp);
		fwrite(im->data,1,datasize,ofp);

	} else if (im->bpp == 3 || im->bpp == 1) {
		uint32_t filesize = datasize + BMPFILEHEADER_SIZE + BMPINFOHEADER_SIZE;

		unsigned char bmpfileheader[BMPFILEHEADER_SIZE] = {'B','M', 0,0,0,0, 0,0, 0,0, BMPFILEHEADER_SIZE+BMPINFOHEADER_SIZE,0,0,0};
		unsigned char bmpinfoheader[BMPINFOHEADER_SIZE] = {BMPINFOHEADER_SIZE,0,0,0,
											0,0,0,0,
											0,0,0,0,
											1,0,
											im->bpp*8,0};

		bmpfileheader[ 2] = (unsigned char)(filesize      );
		bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
		bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
		bmpfileheader[ 5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[ 4] = (unsigned char)(       im->w      );
		bmpinfoheader[ 5] = (unsigned char)(       im->w >>  8);
		bmpinfoheader[ 6] = (unsigned char)(       im->w >> 16);
		bmpinfoheader[ 7] = (unsigned char)(       im->w >> 24);
		bmpinfoheader[ 8] = (unsigned char)(       im->h      );
		bmpinfoheader[ 9] = (unsigned char)(       im->h >>  8);
		bmpinfoheader[10] = (unsigned char)(       im->h >> 16);
		bmpinfoheader[11] = (unsigned char)(       im->h >> 24);

		uint32_t info_header_size = BMPINFOHEADER_SIZE;

		fwrite(bmpfileheader,1,BMPFILEHEADER_SIZE,ofp);
		fwrite(bmpinfoheader,1,info_header_size,ofp);
		fwrite(im->data,1,datasize,ofp);
	}



	fclose(ofp);
	return 0;

}

int LoadImage_mem(image_t* im, char* filename)
{
	FILE *ifp;
	ifp = fopen(filename, "r");

	if (ifp == NULL) {
	  printf( "Can't open input file: %s\n", filename);
	  exit(1);
	}

	unsigned char bmpfileheader[BMPFILEHEADER_SIZE];
	fread(bmpfileheader,1,BMPFILEHEADER_SIZE,ifp);

	if (bmpfileheader[10] == BMPFILEHEADER_SIZE+BMPINFOHEADER_SIZE) {
		unsigned char bmpinfoheader[BMPINFOHEADER_SIZE];
		fread(bmpinfoheader,1,BMPINFOHEADER_SIZE,ifp);

		im->h = *(uint32_t*)&bmpinfoheader[8];
		im->w = *(uint32_t*)&bmpinfoheader[4];
		im->bpp = *(uint32_t*)&bmpinfoheader[14]/8;

//		printf("%d %d %d\n",im->h,im->w,im->bpp);

		if (im_alloc(im) == -1) {
			fclose(ifp);
			return -1;
		}

		fread(im->data,1,im->w*im->h*im->bpp,ifp);

	}
	else {
		printf("32-bit BMP not yet implemented\n");
	}
	fclose(ifp);
	return 0;
}


////Save image from in RAM at [BaseAddress] to disk as [filename]
//int SaveImageDisp(uint32_t BaseAddress, char* filename, uint8_t scale)
//{
//	int map_len = (WIDTH*HEIGHT*bpp);
//	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
//	if(fd < 0)
//	{
//		printf("Failed to open /dev/mem!\n");
//		return -1;
//	}
//
//	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
//
//	if(mem_base == MAP_FAILED)
//	{
//		perror("Mapping memory for absolute memory access failed.\n");
//		close(fd);
//		return -1;
//	}
//
//	FILE *ofp;
//
//	ofp = fopen(filename, "w");
//
//	if (ofp == NULL) {
//	  printf( "Can't open output file!\n");
//	  exit(1);
//	}
//
//	uint32_t w = WIDTH/scale;
//	uint32_t h = HEIGHT/scale;
//	uint32_t filesize = w*h*3 + 54;
//	color_t pix;
//	uint8_t col;
//
//	uint8_t jet[128][3] = {
//			     {0, 0, 128},
//			     {0, 0, 136},
//			     {0, 0, 144},
//			     {0, 0, 152},
//			     {0, 0, 160},
//			     {0, 0, 168},
//			     {0, 0, 176},
//			     {0, 0, 184},
//			     {0, 0, 192},
//			     {0, 0, 200},
//			     {0, 0, 208},
//			     {0, 0, 216},
//			     {0, 0, 224},
//			     {0, 0, 232},
//			     {0, 0, 240},
//			     {0, 0, 248},
//			     {0, 1, 255},
//			     {0, 9, 255},
//			     {0, 17, 255},
//			     {0, 25, 255},
//			     {0, 33, 255},
//			     {0, 41, 255},
//			     {0, 49, 255},
//			     {0, 57, 255},
//			     {0, 65, 255},
//			     {0, 73, 255},
//			     {0, 81, 255},
//			     {0, 89, 255},
//			     {0, 97, 255},
//			     {0, 105, 255},
//			     {0, 113, 255},
//			     {0, 121, 255},
//			     {0, 130, 255},
//			     {0, 138, 255},
//			     {0, 146, 255},
//			     {0, 154, 255},
//			     {0, 162, 255},
//			     {0, 170, 255},
//			     {0, 178, 255},
//			     {0, 186, 255},
//			     {0, 194, 255},
//			     {0, 202, 255},
//			     {0, 210, 255},
//			     {0, 218, 255},
//			     {0, 226, 255},
//			     {0, 234, 255},
//			     {0, 242, 255},
//			     {0, 250, 255},
//			     {3, 255, 252},
//			     {11, 255, 244},
//			     {19, 255, 236},
//			     {27, 255, 228},
//			     {35, 255, 220},
//			     {43, 255, 212},
//			     {51, 255, 204},
//			     {59, 255, 196},
//			     {67, 255, 188},
//			     {75, 255, 180},
//			     {83, 255, 172},
//			     {91, 255, 164},
//			     {99, 255, 156},
//			     {107, 255, 148},
//			     {115, 255, 140},
//			     {123, 255, 132},
//			     {132, 255, 123},
//			     {140, 255, 115},
//			     {148, 255, 107},
//			     {156, 255, 99},
//			     {164, 255, 91},
//			     {172, 255, 83},
//			     {180, 255, 75},
//			     {188, 255, 67},
//			     {196, 255, 59},
//			     {204, 255, 51},
//			     {212, 255, 43},
//			     {220, 255, 35},
//			     {228, 255, 27},
//			     {236, 255, 19},
//			     {244, 255, 11},
//			     {252, 255, 3},
//			     {255, 250, 0},
//			     {255, 242, 0},
//			     {255, 234, 0},
//			     {255, 226, 0},
//			     {255, 218, 0},
//			     {255, 210, 0},
//			     {255, 202, 0},
//			     {255, 194, 0},
//			     {255, 186, 0},
//			     {255, 178, 0},
//			     {255, 170, 0},
//			     {255, 162, 0},
//			     {255, 154, 0},
//			     {255, 146, 0},
//			     {255, 138, 0},
//			     {255, 130, 0},
//			     {255, 121, 0},
//			     {255, 113, 0},
//			     {255, 105, 0},
//			     {255, 97, 0},
//			     {255, 89, 0},
//			     {255, 81, 0},
//			     {255, 73, 0},
//			     {255, 65, 0},
//			     {255, 57, 0},
//			     {255, 49, 0},
//			     {255, 41, 0},
//			     {255, 33, 0},
//			     {255, 25, 0},
//			     {255, 17, 0},
//			     {255, 9, 0},
//			     {255, 1, 0},
//			     {248, 0, 0},
//			     {240, 0, 0},
//			     {232, 0, 0},
//			     {224, 0, 0},
//			     {216, 0, 0},
//			     {208, 0, 0},
//			     {200, 0, 0},
//			     {192, 0, 0},
//			     {184, 0, 0},
//			     {176, 0, 0},
//			     {168, 0, 0},
//			     {160, 0, 0},
//			     {152, 0, 0},
//			     {144, 0, 0},
//			     {136, 0, 0},
//			     {128, 0, 0}};
//
//	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
//	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
//
//	bmpfileheader[ 2] = (unsigned char)(filesize      );
//	bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
//	bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
//	bmpfileheader[ 5] = (unsigned char)(filesize >> 24);
//
//	bmpinfoheader[ 4] = (unsigned char)(       w      );
//	bmpinfoheader[ 5] = (unsigned char)(       w >>  8);
//	bmpinfoheader[ 6] = (unsigned char)(       w >> 16);
//	bmpinfoheader[ 7] = (unsigned char)(       w >> 24);
//	bmpinfoheader[ 8] = (unsigned char)(       h      );
//	bmpinfoheader[ 9] = (unsigned char)(       h >>  8);
//	bmpinfoheader[10] = (unsigned char)(       h >> 16);
//	bmpinfoheader[11] = (unsigned char)(       h >> 24);
//
//	fwrite(bmpfileheader,1,14,ofp);
//	fwrite(bmpinfoheader,1,40,ofp);
//
//	int x,y;
//	for (y = 0; y<HEIGHT; y++)
//	{
//		if (y % scale == 0) {
//			for(x = 0; x<WIDTH; x++)
//			{
//				if (x % scale == 0) {
////					pix = REG_READ(mem_base,(y*WIDTH+(WIDTH-x-1))*bpp);
//					pix = REG_READ_T(mem_base,(y*WIDTH+x)*bpp,color_t);
//
//					if (pix == DEADBEEF) //no data?
//					{
//						col = 255;
//						fwrite (&col, sizeof(col), 1, ofp);
//						fwrite (&col, sizeof(col), 1, ofp);
//						fwrite (&col, sizeof(col), 1, ofp);
//					}
//					else
//					{
//						pix = (pix & 0xFF);
////						if (x % 100 == 0)
////							printf("pix: %d\n",pix);
//						if (pix > 255 || pix < 0)
//						{ //erronous data?
//							printf("WARNING: Out of range %d > %d\n",pix,255);
//							col = 0;
//							fwrite (&col, sizeof(col), 1, ofp);
//							fwrite (&col, sizeof(col), 1, ofp);
//							fwrite (&col, sizeof(col), 1, ofp);
//						}
//						else
//						{
//							pix =  127-(pix >> 1); //256 --> 128 for jet
//							col = jet[pix][0];
//							fwrite (&col, sizeof(col), 1, ofp);
//							col = jet[pix][1];
//							fwrite (&col, sizeof(col), 1, ofp);
//							col = jet[pix][2];
//							fwrite (&col, sizeof(col), 1, ofp);
////						if (jet[pix][0] == 0 && jet[pix][1] == 0 && jet[pix][2] == 0)
////							printf("BLACK: x: %d, y: %d, pix: %d\n",x,y,pix);
//						}
//					}
//				}
//			}
//		}
//	}
//
//	fclose(ofp);
//	printf("Image saved...\n");
//
//	munmap((void *)mem_base, map_len);
//	close(fd);
//	return 0;
//
//}

//void int2bin(uint32_t val, char *s)
//{
//	char *p;
//	uint32_t t;
//	p = s;
//	t = 0x80000000; // scan 32 bits
//	for ( ; t > 0; t = t >> 1) {
//		if (val & t)
//			*p++ = '1';
//		else
//			*p++ = '0';
//	}
//	*p = 0;
//}
//
////Save image from in RAM at [BaseAddress] to disk as [filename]
//
//
//int SaveImageHarris(uint32_t BaseAddress, char* filename, uint32_t corner_threshold, uint32_t edge_threshold, uint8_t ov, uint32_t BaseAddress2)
//{
//	int map_len = (WIDTH*HEIGHT*bpp);
//	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
//	if(fd < 0)
//	{
//		printf("Failed to open /dev/mem!\n");
//		return -1;
//	}
//
//	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
//
//	if(mem_base == MAP_FAILED)
//	{
//		perror("Mapping memory for absolute memory access failed.\n");
//		close(fd);
//		return -1;
//	}
//
//
//	unsigned char* mem_base2;
//	if (ov == 1)
//	{
//		mem_base2 = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress2);
//
//		if(mem_base == MAP_FAILED)
//		{
//			perror("Mapping memory for absolute memory access failed.\n");
//			close(fd);
//			return -1;
//		}
//	}
//
//	FILE *ofp;
//
//	ofp = fopen(filename, "w");
//
//	if (ofp == NULL) {
//	  printf( "Can't open output file!\n");
//	  exit(1);
//	}
//
//	uint32_t w = WIDTH;
//	uint32_t h = HEIGHT;
//	uint32_t filesize = w * h * 3 + 54;
//	uint32_t pix;
//	uint32_t pix2;
//	uint8_t col2;
//	uint8_t col = 255;
//	uint8_t no_col = 0;
//	uint8_t ft;
//	uint32_t val;
//	uint32_t c_max = 0;
//	uint32_t e_max = 0;
////	char bin_nr[32] = "00000000000000000000000000000000";
//	uint32_t bw;
//
//
//	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
//	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
//
//	bmpfileheader[ 2] = (unsigned char)(filesize      );
//	bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
//	bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
//	bmpfileheader[ 5] = (unsigned char)(filesize >> 24);
//
//	bmpinfoheader[ 4] = (unsigned char)(       w      );
//	bmpinfoheader[ 5] = (unsigned char)(       w >>  8);
//	bmpinfoheader[ 6] = (unsigned char)(       w >> 16);
//	bmpinfoheader[ 7] = (unsigned char)(       w >> 24);
//	bmpinfoheader[ 8] = (unsigned char)(       h      );
//	bmpinfoheader[ 9] = (unsigned char)(       h >>  8);
//	bmpinfoheader[10] = (unsigned char)(       h >> 16);
//	bmpinfoheader[11] = (unsigned char)(       h >> 24);
//
//	fwrite(bmpfileheader,1,14,ofp);
//	fwrite(bmpinfoheader,1,40,ofp);
//
//	int x,y;
//	for (y = 0; y<HEIGHT; y++)
//	{
//		for(x = 0; x<WIDTH; x++)
//		{
//			pix = REG_READ(mem_base,(y*WIDTH+x)*bpp);
//			if (ov == 1)
//			{
//				pix2 = REG_READ(mem_base2,(y*WIDTH+x)*bpp);
//				bw = (pix2 >> 16) & 0xFF;
//				bw += (pix2 >> 8) & 0xFF;
//				bw += (pix2) & 0xFF;
//				bw /= 3;
//				col2 = bw;
//			}
//			if (pix == DEADBEEF) {
//				fwrite(&col,sizeof(col),1,ofp);
//				fwrite(&col,sizeof(col),1,ofp);
//				fwrite(&col,sizeof(col),1,ofp);
//			}
//			else {
//				ft = (pix & 0x800000);
//				val = (pix & 0x3FFFF);
//
//				if ((ft > 0) && val > edge_threshold) { //edge
//					fwrite(&no_col,sizeof(no_col),1,ofp);
//					fwrite(&col,sizeof(col),1,ofp);
//					fwrite(&no_col,sizeof(no_col),1,ofp);
//				}
//				else if ((ft == 0) && val > corner_threshold) { //corner
////					int2bin(pix,bin_nr);
////					printf("%s : %d\n",bin_nr,pix);
//					fwrite(&no_col,sizeof(no_col),1,ofp);
//					fwrite(&no_col,sizeof(no_col),1,ofp);
//					fwrite(&col,sizeof(col),1,ofp);
//				}
//				else {
//					if (ov == 1)
//					{
//						fwrite(&col2,sizeof(no_col),1,ofp);
//						fwrite(&col2,sizeof(no_col),1,ofp);
//						fwrite(&col2,sizeof(no_col),1,ofp);
//					}
//					else
//					{
//						fwrite(&no_col,sizeof(no_col),1,ofp);
//						fwrite(&no_col,sizeof(no_col),1,ofp);
//						fwrite(&no_col,sizeof(no_col),1,ofp);
//					}
//				}
//
//				if ((ft == 0) && val > c_max)
//					c_max = val;
//				if ((ft > 0) && val > e_max)
//					e_max = val;
//			}
//		}
//	}
//
//	fclose(ofp);
//	printf("Image saved (c_max, e_max) = (%d, %d)\n",c_max,e_max);
//
//	munmap((void *)mem_base, map_len);
//	munmap((void *)mem_base2, map_len);
//	close(fd);
//	return 0;
//
//}
//
////Load Image from file [filename] on disk to RAM at [BaseAddress]
//int LoadImageStereo(uint32_t BaseAddress, char* filenameL, char* filenameR)
//{
//	int map_len = (WIDTH*HEIGHT*bpp);
//	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
//	if(fd < 0)
//	{
//		printf("Failed to open /dev/mem!\n");
//		return -1;
//	}
//	printf("\nMap memory for Frame Buffer at physical memory: 0x%X\n", BaseAddress);
//	unsigned char* mem_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
//	printf("\nMapped memory for Frame Buffer at virtual memory: 0x%X\n", (uint32_t)mem_base);
//	if(mem_base == MAP_FAILED)
//	{
//		perror("Mapping memory for absolute memory access failed.\n");
//		close(fd);
//		return -1;
//	}
//
//	FILE *ofpL, *ofpR;
//
//	ofpL = fopen(filenameL, "r");
//
//	if (ofpL == NULL) {
//	  printf( "Can't open file %s!\n",filenameL);
//	  exit(1);
//	}
//
//	ofpR = fopen(filenameR, "r");
//
//	if (ofpR == NULL) {
//	  printf( "Can't open file %s!\n",filenameR);
//	  exit(1);
//	}
//
//	uint32_t pixL,pixR,pix;
//	uint8_t colL,colR;
//
//	unsigned char bmpfileheader[14] = {0};
//	unsigned char bmpinfoheader[40] = {0};
//
//	fread(bmpfileheader, 1, 14, ofpL);
//	fread(bmpinfoheader, 1, 40, ofpL);
//	fread(bmpfileheader, 1, 14, ofpR);
//	fread(bmpinfoheader, 1, 40, ofpR);
//	printf("\nWriting image data to memory:\n");
//	int x,y;
//	for (y = 0; y<HEIGHT; y++)
//	{
//		for(x = 0; x<WIDTH; x++)
//		{
//			pix = 0;
//
//			pixL = 0;
//			//COL[BB], COL[GG], COL[RR] -> PIX[0x00RRBBGG]
//			fread(&colL, sizeof(colL), 1, ofpL);
//			pixL += colL; //B
//			fread(&colL, sizeof(colL), 1, ofpL);
//			pixL += colL; //G
//			fread(&colL, sizeof(colL), 1, ofpL);
//			pixL += colL; //R
//			pixL /= 3;
//
//			pixR = 0;
//			//COL[BB], COL[GG], COL[RR] -> PIX[0x00RRBBGG]
//			fread(&colR, sizeof(colR), 1, ofpR);
//			pixR += colR; //B
//			fread(&colR, sizeof(colR), 1, ofpR);
//			pixR += colR; //G
//			fread(&colR, sizeof(colR), 1, ofpR);
//			pixR += colR; //R
//			pixR /= 3;
//
//			pix = (pixR << 12) + (pixL<<2);
//
////			REG_WRITE(mem_base, ((WIDTH-1-x)+(y*WIDTH))*bpp, pix);
//			REG_WRITE(mem_base, ((HEIGHT-1-y)*WIDTH+x)*bpp, pix);
//
//		}
//	}
//
//	fclose(ofpL);
//	fclose(ofpR);
//	printf("Image loaded...\n");
//
//	munmap((void *)mem_base, map_len);
//	close(fd);
//	return 0;
//}


uint32_t GetImageHeight(char* filename)
{
	FILE *ofp;

	ofp = fopen(filename, "r");

	if (ofp == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}


	uint8_t bmpfileheader[14] = {0};
	uint8_t bmpinfoheader[40] = {0};

	fread(bmpfileheader, 1, 14, ofp);
	fread(bmpinfoheader, 1, 40, ofp);


	uint32_t *height = (uint32_t*)&bmpinfoheader[8];
	printf("%u\n",*height);

	fclose(ofp);
	return *height;
}

uint32_t GetImageWidth(char* filename)
{
	FILE *ofp;

	ofp = fopen(filename, "r");

	if (ofp == NULL) {
	  printf( "Can't open output file!\n");
	  exit(1);
	}

	uint8_t bmpfileheader[14] = {0};
	uint8_t bmpinfoheader[40] = {0};

	fread(bmpfileheader, 1, 14, ofp);
	fread(bmpinfoheader, 1, 40, ofp);


	uint32_t *width = (uint32_t*)&bmpinfoheader[4];

	fclose(ofp);
	return *width;
}



//Feed VDMA with image data from memory
void SetupVDMAmm2s(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp, uint32_t frameAddress1, uint32_t frameAddress2, uint32_t frameAddress3)
{
	int map_len = 0xFFF;

	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return ;
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}
	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, 			REG_READ(VDMA_base, VDMA_MM2S_CTRL) | 0x0004);
//	REG_WRITE(VDMA_base, 0x00, 0x4); //Reset VDMA (MM2S)

	if(msync(VDMA_base, map_len, MS_INVALIDATE) < 0)
		perror("Bla bla bla: ");

	usleep(10000);

	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, 			REG_READ(VDMA_base, VDMA_MM2S_CTRL) | 0x0001);			// Enable VDMA VDMACR.RS = 1
	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS, 	frameAddress1); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+4, 	frameAddress2); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+2*4, 	frameAddress3); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_MM2S_FRMDLY_STRIDE, 	bpp*width); 			// Stride
	REG_WRITE(VDMA_base, VDMA_MM2S_HSIZE, 			bpp*width);			//Bytes per Row
	REG_WRITE(VDMA_base, VDMA_MM2S_VSIZE, 			height);			//Rows per image

//	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, 			0x8B);//0x8B); 	// Enable DMA
//	REG_WRITE(VDMA_base, VDMA_MM2S_FRMDLY_STRIDE, 	4*WIDTH); 		// Stride
//	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS, 	frameAddress); 	// Start Address for frame
////	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+VDMA_START_ADDR_LEN, 	frameAddress+0x800000); 	// Start Address for frame
////	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+VDMA_START_ADDR_LEN*2, frameAddress+0x800000*2); 	// Start Address for frame
//	REG_WRITE(VDMA_base, VDMA_MM2S_HSIZE, 			4*WIDTH);		// Horizontal Resolution
//	REG_WRITE(VDMA_base, VDMA_MM2S_VSIZE, 			HEIGHT);		// Vertical Resolution

	munmap((void *)VDMA_base, map_len);
	close(fd);
	return;
}

//Feed image data from data stream to memory
void SetupVDMAs2mm(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp, uint32_t frameAddress1, uint32_t frameAddress2, uint32_t frameAddress3)
{
	int map_len = 0xFFF;

	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return ;
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

//	REG_WRITE(VDMA_base, 0x30, 0x4); //Reset VDMA
	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, 			REG_READ(VDMA_base, VDMA_S2MM_CTRL) | 0x0004);

	if(msync(VDMA_base, map_len, MS_INVALIDATE) < 0)
		perror("Bla bla bla: ");

	usleep(10000);


	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, 			REG_READ(VDMA_base, VDMA_S2MM_CTRL) | 0x0008);			// Enable Genlock
	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, 			REG_READ(VDMA_base, VDMA_S2MM_CTRL) | 0x0001);			// Enable VDMA VDMACR.RS = 1
	REG_WRITE(VDMA_base, VDMA_S2MM_START_ADDRESS, 	frameAddress1); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_S2MM_START_ADDRESS+4, 	frameAddress2); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_S2MM_START_ADDRESS+2*4, 	frameAddress3); 		// Start Address for frame
	REG_WRITE(VDMA_base, VDMA_S2MM_FRMDLY_STRIDE, 	bpp*width); 			// Stride
	REG_WRITE(VDMA_base, VDMA_S2MM_HSIZE, 			bpp*width);			//Bytes per Row
	REG_WRITE(VDMA_base, VDMA_S2MM_VSIZE, 			height);			//Rows per image
//	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, 			0x108B);			// Enable VDMA
////	REG_WRITE(VDMA_base, 0x48, 						0x1); 				// 1 frame store (dunno what this is. 0x48 is listed as "reserved" in VDMA doc)
//	REG_WRITE(VDMA_base, VDMA_S2MM_FRMDLY_STRIDE, 	4*WIDTH); 			// Stride
//	REG_WRITE(VDMA_base, VDMA_S2MM_START_ADDRESS, 	frameAddress); 		// Start Address for frame
////	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+VDMA_START_ADDR_LEN, 	frameAddress+0x800000); 	// Start Address for frame
////	REG_WRITE(VDMA_base, VDMA_MM2S_START_ADDRESS+VDMA_START_ADDR_LEN*2, frameAddress+0x800000*2); 	// Start Address for frame
//	REG_WRITE(VDMA_base, VDMA_S2MM_HSIZE, 			4*WIDTH);			//Bytes per Row
//	REG_WRITE(VDMA_base, VDMA_S2MM_VSIZE, 			HEIGHT);			//Rows per image

	munmap((void *)VDMA_base, map_len);
	close(fd);
	return;
}

//Stops both channels
void StopVDMA(uint32_t BaseAddress)
{
	int map_len = 0xFFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, REG_READ(VDMA_base, VDMA_MM2S_CTRL) & 0xFFFFFFFE); //Only set "Run/Stop" bit to 0
	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, REG_READ(VDMA_base, VDMA_S2MM_CTRL) & 0xFFFFFFFE); //Only set "Run/Stop" bit to 0
	munmap((void *)VDMA_base, map_len);
	close(fd);
	return;
}

//Reset both channels
void ResetVDMA(uint32_t BaseAddress)
{
	int map_len = 0xFFFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return;
	}

	unsigned char* VDMA_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(VDMA_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, REG_READ(VDMA_base, VDMA_MM2S_CTRL) | 0x4); //Activate reset-bit on VDMA-MM2S control register
	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, REG_READ(VDMA_base, VDMA_S2MM_CTRL) | 0x4); //Activate reset-bit on VDMA-S2MM control register
//	REG_WRITE(VDMA_base, VDMA_MM2S_CTRL, 0x4); //Activate reset-bit on VDMA-MM2S control register
//	REG_WRITE(VDMA_base, VDMA_S2MM_CTRL, 0x4); //Activate reset-bit on VDMA-S2MM control register

	munmap((void *)VDMA_base, map_len);
	close(fd);
	return;
}



