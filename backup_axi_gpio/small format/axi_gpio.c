/*
 * axi_gpio.h
 *
 *  Created on: 2013-06-12
 *      Author: Carl
 */

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
#include <stdbool.h>
#include "hw_base.h"
#include "axi_gpio.h"



static const int _5000 = 5000;
//int mysleep(int time)
//{
//	int i,j,a = 0;
//	for (j=0; j<time; j++)
//		for (i=0; i<25; i++)
//			a++;
//
//	return a;
//}

void mysleep(int time)
{
	usleep(time);
}

void write_cam_register(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc, uint16_t waddr, uint16_t wdata)
{
  uint8_t wr_ID_byte = (0x20 << 0);
  uint8_t i;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}
  // Start:
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send ID:
  for(i=0; i<8; i++) {
	  if ((wr_ID_byte >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send MSB addr:
  for(i=0; i<8; i++) {
	  if ((waddr >> (15-i)) & 0x1) {
		  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send LSB addr:
  for(i=0; i<8; i++) {
	  if ((waddr >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send data:
  for(i=0; i<8; i++) {
	  if ((wdata >> (15-i)) & 0x1) {
		  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send data:
  for(i=0; i<8; i++) {
	  if ((wdata >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  //XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
  usleep(SLEEP_T);

  // Stop:
  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  munmap((void *)gpio_base, map_len);
  close(fd);
}

void dual_write_cam_register_sep(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1, uint16_t waddr, uint16_t wdata0, uint16_t wdata1)
{
  uint8_t wr_ID_byte = (0x20 << 0);
  uint8_t i;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}
  // Start:
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send ID:
  for(i=0; i<8; i++) {
	  if ((wr_ID_byte >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send MSB addr:
  for(i=0; i<8; i++) {
	  if ((waddr >> (15-i)) & 0x1) {
		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send LSB addr:
  for(i=0; i<8; i++) {
	  if ((waddr >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  else {
		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  }
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send data:
  for(i=0; i<8; i++) {
	  if ((wdata0 >> (15-i)) & 0x1) {
		  SET_BIT(*pin_data,siod0);
	  }
	  else {
		  CLR_BIT(*pin_data,siod0);
	  }
	  if ((wdata1 >> (15-i)) & 0x1) {
		  SET_BIT(*pin_data,siod1);
	  }
	  else {
		  CLR_BIT(*pin_data,siod1);
	  }
	  REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  // Send data:
  for(i=0; i<8; i++) {
	  if ((wdata0 >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod0);
	  }
	  else {
		  CLR_BIT(*pin_data,siod0);
	  }
	  if ((wdata1 >> (7-i)) & 0x1) {
		  SET_BIT(*pin_data,siod1);
	  }
	  else {
		  CLR_BIT(*pin_data,siod1);
	  }
	  REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
  }
  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  //XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
  usleep(SLEEP_T);

  // Stop:
  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);
  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
  usleep(SLEEP_T);

  munmap((void *)gpio_base, map_len);
  close(fd);
}

void dual_write_cam_register(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1, uint16_t waddr, uint16_t wdata) {
	dual_write_cam_register_sep(pin_data,pin_dir,siod0,sioc0,siod1,sioc1,waddr,wdata,wdata);
}
//void dual_write_cam_register(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1, uint16_t waddr, uint16_t wdata)
//{
//  uint8_t wr_ID_byte = (0x20 << 0);
//  uint8_t i;
//
//	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
//	if(fd < 0)
//	{
//		perror("Failed to open /dev/mem!\n");
//		return;
//	}
//
//	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;
//
//	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);
//
//	if(gpio_base == MAP_FAILED)
//	{
//		perror("Mapping memory for absolute memory access failed.\n");
//		close(fd);
//		return;
//	}
//  // Start:
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  // Send ID:
//  for(i=0; i<8; i++) {
//	  if ((wr_ID_byte >> (7-i)) & 0x1) {
//		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  else {
//		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  usleep(SLEEP_T);
//	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//  }
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  // Send MSB addr:
//  for(i=0; i<8; i++) {
//	  if ((waddr >> (15-i)) & 0x1) {
//		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  else {
//		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  usleep(SLEEP_T);
//	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//  }
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  // Send LSB addr:
//  for(i=0; i<8; i++) {
//	  if ((waddr >> (7-i)) & 0x1) {
//		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  else {
//		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  usleep(SLEEP_T);
//	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//  }
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  // Send data:
//  for(i=0; i<8; i++) {
//	  if ((wdata >> (15-i)) & 0x1) {
//		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  else {
//		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  usleep(SLEEP_T);
//	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//  }
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  // Send data:
//  for(i=0; i<8; i++) {
//	  if ((wdata >> (7-i)) & 0x1) {
//		  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  else {
//		  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  }
//	  usleep(SLEEP_T);
//	  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//	  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//	  usleep(SLEEP_T);
//  }
//  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,sioc0); CLR_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  CLR_BIT(*pin_data,siod0); CLR_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  //XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
//  usleep(SLEEP_T);
//
//  // Stop:
//  SET_BIT(*pin_data,sioc0); SET_BIT(*pin_data,sioc1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//  SET_BIT(*pin_data,siod0); SET_BIT(*pin_data,siod1); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
//  usleep(SLEEP_T);
//
//  munmap((void *)gpio_base, map_len);
//  close(fd);
//}

uint16_t read_cam_register(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc, uint16_t raddr)
{
  uint8_t rd_ID_byte = (0x20 << 0) | 0x1;
  uint8_t wr_ID_byte = (0x20 << 0);
  uint8_t read_byte1 = 0x0;
  uint8_t read_byte2 = 0x0;
  uint8_t i;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return 0;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return 0;
	}



	   // Start:
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);

	  // Send ID:
	  for(i=0; i<8; i++) {
		  if ((wr_ID_byte >> (7-i)) & 0x1) {
			  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
		  else {
			  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
	//	  XGpioPs_WritePin(Gpio, SIOD, ((wr_ID_byte >> (7-i)) & 0x1)  );
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
	  }
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_DIS);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
	  usleep(SLEEP_T);

	  // Send MSB addr:
	  for(i=0; i<8; i++) {
		  if ((raddr >> (15-i)) & 0x1) {
			  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
		  else {
			  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
	//	  XGpioPs_WritePin(Gpio, SIOD, ((raddr >> (15-i)) & 0x1)  );
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
	  }
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_DIS);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
	  usleep(SLEEP_T);

	  // Send LSB addr:
	  for(i=0; i<8; i++) {
		  if ((raddr >> (7-i)) & 0x1) {
			  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
		  else {
			  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
	//	  XGpioPs_WritePin(Gpio, SIOD, ((raddr >> (7-i)) & 0x1)  );
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
	  }
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_DIS);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
	  usleep(SLEEP_T);

	  // Stop:
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);


	  usleep(5*SLEEP_T);

	  // Start:
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);

	  // Send ID:
	  for(i=0; i<8; i++) {
		  if ((rd_ID_byte >> (7-i)) & 0x1) {
			  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
		  else {
			  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  }
	//	  XGpioPs_WritePin(Gpio, SIOD, ((rd_ID_byte >> (7-i)) & 0x1)  );
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
	  }
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_DIS);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
//	//  XGpioPs_SetDirectionPin(Gpio, SIOD, DIR_IN);
//	  dir = XGpio_GetDataDirection(Gpio, CHANNEL);
//	  XGpio_SetDataDirection(Gpio, CHANNEL, dir | SIOD);
	    // read enable
	    SET_BIT(*pin_dir,siod);
	    REG_WRITE(gpio_base,GPIO_TRI_REG,*pin_dir);

	  // Read byte 1:
	  read_byte1 = 0x0;
	  for(i=0; i<8; i++) {
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  read_byte1 |= (	 ((REG_READ(gpio_base,GPIO_DATA_REG) >> siod) & 0x01) 	<< (7-i));
	//      read_byte1 |= (XGpioPs_ReadPin(Gpio, SIOD) << (7-i));
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);

	  }
	  // Ack:
	//  XGpioPs_SetDirectionPin(Gpio, SIOD, DIR_OUT);
//	  dir = XGpio_GetDataDirection(Gpio, CHANNEL);
//	  XGpio_SetDataDirection(Gpio, CHANNEL, dir & ~SIOD);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
	    // write enable
	    CLR_BIT(*pin_dir,siod);
	    REG_WRITE(gpio_base,GPIO_TRI_REG,*pin_dir);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	//  XGpioPs_SetDirectionPin(Gpio, SIOD, DIR_IN);
//	  dir = XGpio_GetDataDirection(Gpio, CHANNEL);
//	  XGpio_SetDataDirection(Gpio, CHANNEL, dir | SIOD);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_DIS);
	    // read enable
	    SET_BIT(*pin_dir,siod);
	    REG_WRITE(gpio_base,GPIO_TRI_REG,*pin_dir);

	  // Read byte 2:
	  read_byte2 = 0x0;
	  for(i=0; i<8; i++) {
		  usleep(SLEEP_T);
		  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
		  read_byte2 |= (	 ((REG_READ(gpio_base,GPIO_DATA_REG) >> siod) & 0x01) 	<< (7-i));
	//      read_byte2 |= (XGpioPs_ReadPin(Gpio, SIOD) << (7-i));
		  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
		  usleep(SLEEP_T);
	  }
	  // No-Ack:
	//  XGpioPs_SetDirectionPin(Gpio, SIOD, DIR_OUT);
//	  dir = XGpio_GetDataDirection(Gpio, CHANNEL);
//	  XGpio_SetDataDirection(Gpio, CHANNEL, dir & ~SIOD);
	    // write enable
	    CLR_BIT(*pin_dir,siod);
	    REG_WRITE(gpio_base,GPIO_TRI_REG,*pin_dir);
	  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	//  XGpioPs_SetOutputEnablePin(Gpio, SIOD, OUT_EN);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  CLR_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);


	  // Stop:
	  SET_BIT(*pin_data,sioc); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);
	  SET_BIT(*pin_data,siod); REG_WRITE(gpio_base,GPIO_DATA_REG,*pin_data);
	  usleep(SLEEP_T);

	  munmap((void *)gpio_base, map_len);
	  close(fd);
	  return (((uint16_t)read_byte1 << 8) | (uint16_t)read_byte2 );
	}

void sensor_full(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	uint16_t read_data;
	uint16_t write_data;

    //ARRAY READOUT SETTINGS
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3004, 32);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3008, 3871);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3002, 8);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3006, 2755);
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3040, 0x0041);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3178);
    write_data = (read_data & 0xFF0F);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3178, write_data);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x400, 2);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x404, 16);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34C, 3840);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34E, 2748);

    //Frame and Integration Time Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x342, 3952);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x340, 2892);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3010, 156);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3014, 1010);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3018, 0);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x30D4, 0xB080);

}

void sensor_full_dual(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1)
{
	uint16_t read_data;
	uint16_t write_data;

    //ARRAY READOUT SETTINGS
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3004, 32);
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3008, 3871);
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3002, 8);
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3006, 2755);
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3040, 0x0041);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3178);
    write_data = (read_data & 0xFF0F);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3178, write_data);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x400, 2);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x404, 16);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34C, 3840);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34E, 2748);

    //Frame and Integration Time Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x342, 3952);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x340, 2892);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3010, 156);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3014, 1010);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3018, 0);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x30D4, 0xB080);

}

void sensor_640_240(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	uint16_t read_data;
	uint16_t write_data;

    //ARRAY READOUT SETTINGS
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3004, 640);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3008, 3193);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3002, 872);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3006, 2777);
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3040, 0x09CF);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3178);
    write_data = (read_data & 0xFF0F) | 0x0070;
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3178, write_data);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x400, 2);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x404, 16);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34C, 640);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34E, 240);

    //Frame and Integration Time Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x342, 1732);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x340, 320);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3010, 156);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3014, 1010);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3018, 0);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x30D4, 0x9080);

}

void sensor_640_240_dual(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, int32_t siod1, uint32_t sioc1)
{
	uint16_t read_data;
	uint16_t write_data;

    //ARRAY READOUT SETTINGS
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3004, 640);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3008, 3193);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3002, 872);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3006, 2777);
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3040, 0x09CF);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3178);
    write_data = (read_data & 0xFF0F) | 0x0070;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3178, write_data);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x400, 2);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x404, 16);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34C, 640);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34E, 240);

    //Frame and Integration Time Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x342, 1732);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x340, 320);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3010, 156);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3014, 1010);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3018, 0);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x30D4, 0x9080);

}


void sensor_1080p60(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	uint16_t read_data;
	uint16_t write_data;

    //ARRAY READOUT SETTINGS
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3004, 32);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3008, 3873);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3002, 296);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3006, 2457);
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3040, 0x28C3);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3178);
    write_data = (read_data & 0xFF0F) | 0x0070;
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3178, write_data);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x400, 2);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x404, 16);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34C, 1920);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34E, 1080);

    //Frame and Integration Time Settings
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x342, 2299);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x340, 1161);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3010, 156);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3014, 1010);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3018, 0);

    write_cam_register(pin_data, pin_dir, siod, sioc, 0x30D4, 0x9080);

}


void sensor_1080p60_dual(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, int32_t siod1, uint32_t sioc1)
{
	uint16_t read_data;
	uint16_t write_data;

//	//sn: 624
//	int16_t x_adjust = 32;//-15;
//	int16_t y_adjust = -60;

	//naiad
	int16_t x_adjust = 0;
	int16_t y_adjust = 40;

    //ARRAY READOUT SETTINGS -- cam1 reference
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3004, 32+x_adjust, 32-x_adjust); //X-start
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3008, 3873+x_adjust, 3873-x_adjust); //X-end
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3002, 296+y_adjust, 296-y_adjust); //Y-start
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3006, 2457+y_adjust, 2457-y_adjust); //Y-end

    //ARRAY READOUT SETTINGS
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3004, 32);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3008, 3873);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3002, 296);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3006, 2457);
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3040, 0x28C3);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDE, 0xE41A);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3178);
    write_data = (read_data & 0xFF0F) | 0x0070;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3178, write_data);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3170, 0x00E5);

    //OUTPUT DATA Path Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x400, 2);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x404, 16);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34C, 1920);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34E, 1080);

    //Frame and Integration Time Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x342, 2299);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x340, 1161);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3010, 156);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3014, 1010);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3018, 0);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x30D4, 0x9080);

}

//void sensor_640_480_rgb(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
//{
//	uint16_t read_data;
//	uint16_t write_data;
//
//    //ARRAY READOUT SETTINGS
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3004, 672);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3008, 3233);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3002, 416);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3006, 2337);
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3040);
//    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3040, 0x28C3);
//
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDE, 0xE41A);
//
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC);
//    write_data = (read_data & 0xFF7F) | 0x0080;
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDC, write_data);
//
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3178);
//    write_data = (read_data & 0xFF0F) | 0x0070;
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3178, write_data);
//
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3170, 0x00E5);
//
//    //OUTPUT DATA Path Settings
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x400, 2);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x404, 16);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34C, 640);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x34E, 480);
//
//    //Frame and Integration Time Settings
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x342, 2299);//<----------------------????
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x340, 1161);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3010, 156);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3014, 1010);
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3018, 0);
//
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x30D4, 0x9080);
//
//}



void sensor_640_480_rgb_dual(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, int32_t siod1, uint32_t sioc1)
{
	uint16_t read_data;
	uint16_t write_data;

	int16_t x_adjust = 0;
	int16_t y_adjust = 0;

	//sn: 00607, 6mm
//	int16_t x_adjust = -30; //15;
//	int16_t y_adjust = -30; //25;
	//sn: 00607, 8mm
//	int16_t x_adjust = 35;
//	int16_t y_adjust = -8;

	//sn: 621
//	int16_t x_adjust = 43; //13;//43;
//	int16_t y_adjust = 11;

	//sn: 615
//	int16_t x_adjust = -25;//-15;
//	int16_t y_adjust = -9;

	//sn: 622
//	int16_t x_adjust = 35;//-15;
//	int16_t y_adjust = -9;

    //ARRAY READOUT SETTINGS -- cam1 reference
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3004, 672+(x_adjust*2*2), 672); //X-start
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3008, 3233+(x_adjust*2*2), 3233); //X-end
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3002, 416+(y_adjust*2*2), 416); //Y-start
	dual_write_cam_register_sep(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3006, 2337+(y_adjust*2*2), 2337); //Y-end
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3040);
    printf("Read init value reg 0x3040: 0x%.4x \r\n", read_data);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3040, 0x2BC7);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDE, 0xE412);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3EDC);
    write_data = (read_data & 0xFF7F) | 0x0080;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDC, write_data);

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3178);
    write_data = (read_data & 0xFF0F) | 0x0070;
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3178, write_data);



    //OUTPUT DATA Path Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x400, 2);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x404, 16);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34C, 640);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x34E, 480);

    //Frame and Integration Time Settings
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x342, 1206);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x340, 1106);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3010, 72);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3014, 522);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3018, 0);

    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3170, 0x0071);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x30D4, 0xB080);

}

void init_cam0(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	uint16_t read_data;
	uint16_t write_data;

    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x31fe);
    printf("Read Sensor revision (0x32=MT9J003) reg 31fe: 0x%.4x \r\n", read_data);
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x31ae);
    printf("Read Sensor interface (0x0301=parallel 0x0304=HiSPi) reg 31ae: 0x%.4x \r\n", read_data);

    //Setup PLL using 24MHz clock
    printf("Setup PLL using 24MHz clock \r\n");
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0300, 3); // vt_pix_clk_div
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0302, 1); // vt_sys_clk_div
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0304, 2); // pre_pll_clk_div
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0306, 30); // pll_multiplier --360 --> 180MHz
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0306, 40); // pll_multiplier --480 --> 240MHz
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0308, 12); // op_pix_clk_div
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x030A, 1); // op_sys_clk_div
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3016);
    write_data = (read_data & 0xF8F8) | 0x0101; //pc_speed="001", op_speed="001"
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0316, write_data);

    //Default Recommended Register Changes
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x316C,0x0429);//Reserved
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3174,0x8000);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3E40,0xDC05);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3E42,0x6E22);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3E44,0xDC22);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3E46,0xFF00);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3ED4,0xF998);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3ED6,0x9789);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EDE,0xE41A);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EE0,0xA43F);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EE2,0xA4BF);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EEC,0x1221);//Reserved
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x3EEE, 0x1224); // Reserved

    // Change the data pedestal and SMIA encoded data
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x301A, 0x0010);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3064, 0x0805);
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x301E, 0x00A8);
    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3064);
    write_data = (read_data & 0xFEFF); // Disable SMIA encoding
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3064, write_data);


    //sensor array
//    sensor_full(pin_data, pin_dir, siod, sioc);
//    sensor_640_240(pin_data, pin_dir, siod, sioc);
    sensor_1080p60(pin_data, pin_dir, siod, sioc);
//    sensor_640_480_rgb(pin_data, pin_dir, siod, sioc);


    // Enable HiSpi streaming:
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x301A, 0x001C);

    // Test pattern
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3070, 3); //3: Fade to grey color bar test pattern
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3070, 2); //color bars
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3070, 1); //solid color
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3072, 0); //0x0F55); //red
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3074, 0x0); //0x0FAA); //green GR
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3076, 0); //0x0F66); //blue
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3078, 0x0FFF); //FAA); //green BG
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x3070, 256); //walking ones

//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3072);
//    printf("0x3072 - red: 0x%.4x \r\n", read_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3074);
//    printf("0x3074 - green: 0x%.4x \r\n", read_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3076);
//    printf("0x3076 - blue: 0x%.4x \r\n", read_data);

    //hispi test
    //7 - test enable
    //6-4 - 3:Transmit a square wave at the pixel rate on all enabled data lanes
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C6, 0x80 | ((3)<<4));
    //streaming
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C6, 0x0);

//
//	read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x0200);
//	printf("0x0200 - fine_integraton_time: 0x%.4x \r\n", read_data);
//	write_cam_register(pin_data, pin_dir, siod, sioc, 0x0202, 0x0500);
//	read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x0202);
//	printf("0x0202 - coarse_integrdation_time: 0x%.4x \r\n", read_data);



    //packetized
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C6, 0x400);

    //Delay per LVDS-lane and/or clock
    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C0, 0x3000);

    printf("Init cam0 done\n");

}

void init_cam1(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{

    init_cam0(pin_data,pin_dir,siod,sioc);
}


void init_cams_sync(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1)
{
	uint16_t read_data;
	uint16_t write_data;

    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x31fe);
    printf("Read Sensor revision (0x32=MT9J003) reg 31fe: 0x%.4x \r\n", read_data);
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x31ae);
    printf("Read Sensor interface (0x0301=parallel 0x0304=HiSPi) reg 31ae: 0x%.4x \r\n", read_data);

    //Setup PLL using 24MHz clock
    printf("Setup PLL using 24MHz clock \r\n");
    //************* 12-bit ******************/
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0300, 3); // vt_pix_clk_div
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0302, 1); // vt_sys_clk_div
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0304, 2); // pre_pll_clk_div
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x0306, 30); // pll_multiplier --360 --> 180MHz
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0306, 40); // pll_multiplier --480 --> 240MHz
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0308, 12); // op_pix_clk_div
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x030A, 1);// op_sys_clk_div
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3016);
    write_data = (read_data & 0xF8F8) | 0x0101; //pc_speed="001", op_speed="001"
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0316, write_data);
    //**************** 10-bit *********************/
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0300, 5); // vt_pix_clk_div
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0302, 1); // vt_sys_clk_div
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0304, 4); // pre_pll_clk_div
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0306, 128); // pll_multiplier --480 --> 240MHz
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0308, 10); // op_pix_clk_div
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x030A, 2);// op_sys_clk_div
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3016);
//    write_data = (read_data & 0xF8F8) | 0x0101; //pc_speed="001", op_speed="001"
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0316, write_data);


    //Default Recommended Register Changes
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x316C,0x0429);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3174,0x8000);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3E40,0xDC05);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3E42,0x6E22);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3E44,0xDC22);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3E46,0xFF00);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3ED4,0xF998);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3ED6,0x9789);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EDE,0xE41A);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EE0,0xA43F);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EE2,0xA4BF);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EEC,0x1221);//Reserved
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3EEE, 0x1224); // Reserved

    // Change the data pedestal and SMIA encoded data
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x301A, 0x0010);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3064, 0x0805);
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x301E, 0x00A8);
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3064);
    write_data = (read_data & 0xFEFF); // Disable SMIA encoding
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3064, write_data);


    //sensor array
//    sensor_full_dual(pin_data, pin_dir, siod0, sioc0, siod1, sioc1); //3840 (V) x 2748 (H)
//    sensor_640_240_dual(pin_data, pin_dir, siod0, sioc0, siod1, sioc1); //640x240
//    sensor_1080p60_dual(pin_data, pin_dir, siod0, sioc0, siod1, sioc1); //1920x1080
    sensor_640_480_rgb_dual(pin_data, pin_dir, siod0, sioc0, siod1, sioc1); //640x480


    // Enable HiSpi streaming:
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x301A, 0x001C);

    // Test pattern
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3070, 2); //color bars
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3070, 3); //fade to gray color bars
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3070, 1); //solid color
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3072, 0); //0x0F55); //red
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3074, 0); //0x0FAA); //green GR
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3076, 0); //0x0F66); //blue
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3078, 0); //FAA); //green BG
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3070, 256); //walking ones

//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3072);
//    printf("0x3072 - red: 0x%.4x \r\n", read_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3074);
//    printf("0x3074 - green: 0x%.4x \r\n", read_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod, sioc, 0x3076);
//    printf("0x3076 - blue: 0x%.4x \r\n", read_data);

    //hispi test
    //7 - test enable
    //6-4 - 3:Transmit a square wave at the pixel rate on all enabled data lanes
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C6, 0x80 | ((3)<<4));
    //streaming
//    write_cam_register(pin_data, pin_dir, siod, sioc, 0x31C6, 0x0);


	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0200);
	printf("0x0200 - fine_integraton_time: 0x%.4x \r\n", read_data);
	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0202);
	printf("0x0202 - coarse_integrdation_time: 0x%.4x \r\n", read_data);
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0202, 0x2000); //100
	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0202);
	printf("0x0202 - coarse_integrdation_time: 0x%.4x \r\n", read_data);



    //packetized
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x31C6, 0x400);

    //Delay per LVDS-lane and/or clock
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x31C0, 0x3000);

	//orientation
    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3040);
    printf("0x3040 - read_mode: 0x%.4x \r\n", read_data);
//    write_data = read_data;
//    write_data = (read_data | 0x4000); //mirror
    write_data = (read_data | 0x8000); //flip
//    write_data = (read_data | 0xC000); //mirror and flip
    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3040, write_data);

    //global gain
//	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x305E);
//	printf("0x305E - Global Gain: 0x%.4x \r\n", read_data);
//	write_data = read_data;
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x305E, 0x1c40);
	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x305E);
	printf("0x305E - Global Gain: 0x%.4x \r\n", read_data);


//    //red gain
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x305A);
//    printf("0x305A - Red Gain: 0x%.4x \r\n", read_data);
//    write_data = (read_data | 0x0060);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x305A, write_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x305A);
//    printf("0x305A - Red Gain: 0x%.4x \r\n", read_data);


//    //blue gain
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3058);
//    printf("0x3058 - Blue Gain: 0x%.4x \r\n", read_data);
//    write_data = (read_data & 0xFF80);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x3058, write_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x3058);
//    printf("0x3058 - Blue Gain: 0x%.4x \r\n", read_data);





//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0112); //cpp_data_format
//    printf("DATA READ 0x0112: 0x%.4x \r\n", read_data);
//    write_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x00CA); //data_format_descriptor_4 = 0x0C08
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0112, write_data);
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0112);
//    printf("DATA READ 0x0112: 0x%.4x \r\n", read_data);

    printf("Init cam0 done\n");

}

//void cam_sync(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1)
//{
//	uint16_t read_data;
//	uint16_t write_data;
//
//	read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x301A); //assume same value for both cameras
//	write_data = (read_data | 0x0002);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x301A, write_data);
//    write_cam_register(pin_data, pin_dir, siod0, sioc0, 0x301A, write_data);
//    printf("reset_register_restart = 1\n");
//    read_data = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x301A); //assume same value for both cameras
//    printf("Reset_register: 0x%.4x \r\n", write_data);
//    printf("Reset_register: 0x%.4x \r\n", read_data);
//    write_data = (write_data & 0xFFFD);
//    dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x301A, write_data);
//
//    printf("reset_register_restart = 0\n");
//}


void set_pin_low(uint32_t *pin_data, uint32_t pin)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	CLR_BIT(*pin_data,pin);
	REG_WRITE(gpio_base, GPIO_DATA_REG, *pin_data);

	munmap((void *)gpio_base, map_len);
	close(fd);

//	printf("pin %d set low\n",pin);
	return;
}

void set_pin_high(uint32_t *pin_data, uint32_t pin)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	SET_BIT(*pin_data,pin);
	REG_WRITE(gpio_base, GPIO_DATA_REG, *pin_data);

	munmap((void *)gpio_base, map_len);
	close(fd);

//	printf("pin %d set high\n",pin);
	return;
}

void set_pin_flip(uint32_t *pin_data, uint32_t pin)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	FLIP_BIT(*pin_data,pin);
	REG_WRITE(gpio_base, GPIO_DATA_REG, *pin_data);

	munmap((void *)gpio_base, map_len);
	close(fd);

//	printf("pin %d set high\n",pin);
	return;
}

void reset_n(uint32_t *pin_data, uint32_t pin)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	CLR_BIT(*pin_data,pin);
	REG_WRITE(gpio_base, GPIO_DATA_REG, *pin_data);
	usleep(1000);
	SET_BIT(*pin_data,pin);
	REG_WRITE(gpio_base, GPIO_DATA_REG, *pin_data);
	usleep(1000);

	munmap((void *)gpio_base, map_len);
	close(fd);

//	printf("reset pin %d\n",pin);
	return;
}

void axi_gpio_init_dir()
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(gpio_base, GPIO_TRI_REG, 0); //all out

	munmap((void *)gpio_base, map_len);
	close(fd);

//	printf("pin %d set high\n",pin);
	return;

}

void axi_gpio_init_val(uint32_t *pin_data)
{
	set_pin_high(pin_data,SIOD_0);
	set_pin_high(pin_data,SIOC_0);

	set_pin_high(pin_data,SIOD_1);
	set_pin_high(pin_data,SIOC_1);

	set_pin_low(pin_data,FPGA_CLK_RESET);
	set_pin_low(pin_data,CAM_RESET);
	set_pin_low(pin_data,FPGA_SOFT_RESET);
}

void axi_gpio_no_init_val(uint32_t *pin_data)
{
	set_pin_high(pin_data,SIOD_0);
	set_pin_high(pin_data,SIOC_0);

	set_pin_high(pin_data,SIOD_1);
	set_pin_high(pin_data,SIOC_1);

	set_pin_high(pin_data,FPGA_CLK_RESET);
	set_pin_high(pin_data,CAM_RESET);
	set_pin_low(pin_data,FPGA_SOFT_RESET);
}

void cam_reset(uint32_t *pin_data)
{
	reset_n(pin_data, CAM_RESET);
}

void fpga_clk_reset(uint32_t *pin_data)
{
	reset_n(pin_data, FPGA_CLK_RESET);
}

void fpga_pipe_reset(uint32_t *pin_data)
{
	reset_n(pin_data, FPGA_SOFT_RESET);
}

void init_cams(uint32_t *pin_data, uint32_t *pin_dir)
{

	init_cam0(pin_data,pin_dir, SIOD_0, SIOC_0);
	init_cam1(pin_data,pin_dir, SIOD_1, SIOC_1);
}

void set_harris_th(bool corner, uint32_t th)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = GPIO_2_HIGHADDR-GPIO_2_BASEADDR;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_2_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	if (corner)
		REG_WRITE(gpio_base, GPIO_DATA_REG, th);
	else
		REG_WRITE(gpio_base, GPIO2_DATA_REG, th);

	munmap((void *)gpio_base, map_len);
	close(fd);



}

//void init_cams()
//{
//	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
//	if(fd < 0)
//	{
//		perror("Failed to open /dev/mem!\n");
//		return;
//	}
//
//	int map_len = GPIO_HIGHADDR-GPIO_BASEADDR;
//
//	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_BASEADDR);
//
//	if(gpio_base == MAP_FAILED)
//	{
//		perror("Mapping memory for absolute memory access failed.\n");
//		close(fd);
//		return;
//	}
//
//
//
//
//
//	//REG_WRITE(gpio_base,OUTPUT_OFFSET,0x01); // Enable output on GPIO 0 (EMIO)
//	REG_WRITE(gpio_base,GPIO_TRI_REG,0x00); // Set direction to output on GPIO 0 - all out
//
//	x_gpio_struct xgs = {gpio_base,0,0,GPIO_DATA_REG,GPIO_TRI_REG};
//	cam_sccb_struct css = {&xgs,SIOC_0,SIOD_0};
//	cam_sccb_struct css1 = {&xgs,SIOC_1,SIOD_1};
//
//	//set SDA & SCL high
//	SET_BIT(xgs.pin_data,SIOC_0);
//	SET_BIT(xgs.pin_data,SIOD_0);
//	SET_BIT(xgs.pin_data,SIOC_1);
//	SET_BIT(xgs.pin_data,SIOD_1);
//	CLR_BIT(xgs.pin_data,FPGA_SOFT_RESET);
//	CLR_BIT(xgs.pin_data,CAM_RESET_PIN);
//	REG_WRITE(xgs.gpio_base,xgs.data_offset,xgs.pin_data);
//	printf("Pin data: %.2x \r\n",xgs.pin_data);
//
//// NOT CONNECTED
////	printf("Reset Image Sensor\r\n");
////	CAM_RESET_PIN(&xgs);
////	init_cam(&css);
////	init_cam(&css1);
//	fpga_soft_reset(&xgs);
//
//
////int i;
////uint8_t read_data;
////
////    for (i=0; i<12; i++) {
////      read_data = read_cam_register(&css, 0x4b00+i);
////      printf("Read LVDS R%d: 0x%.2x \r\n", i,read_data);
////    }
//
//
////    mysleep(1000);
////    printf("Reset LVDS interface\r\n");
////    fpga_soft_reset(&xgs);
////    printf("Reset released\r\n");
//
//
//
//	munmap((void *)gpio_base, map_len);
//	close(fd);
//}


void get_integration_time_limits(integration_time_limits_t *itl, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0)
{
	itl->coarse_min = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x1004);
	itl->coarse_max = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0340)-read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x1006);
	itl->fine_min = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x1008);
	itl->fine_max = read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x0342)-read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x100A);
}

void print_integration_time_limits(integration_time_limits_t *itl)
{
	printf("Integration time limits: coarse(%d,%d), fine(%d,%d)\n",itl->coarse_min,itl->coarse_max,itl->fine_min,itl->fine_max);
}

void get_integration_time(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	it->coarse = read_cam_register(pin_data, pin_dir, siod, sioc, 0x0202);
	it->fine = read_cam_register(pin_data, pin_dir, siod, sioc, 0x0200);
}
void set_integration_time_dual(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1)
{
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0202, it->coarse);
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x0200, it->fine);
}

void set_integration_time(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc)
{
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x0202, it->coarse);
	write_cam_register(pin_data, pin_dir, siod, sioc, 0x0200, it->fine);
}

void print_integration_time(integration_times_t *it)
{
	printf("Integration times: coarse: %d, fine: %d\n",it->coarse,it->fine);
}

void calc_integration(uint32_t exp_pos, uint32_t exp_neg, integration_times_t *it, integration_time_limits_t *itl)
{
	double ratio; //<1.0

	if (exp_neg > exp_pos) //dark
	{
		ratio = (double)exp_pos/exp_neg;
//		printf("Dark %lf\n",ratio);
		if (ratio+INTEGRATION_INTERVAL < 1.0) //increase exposure time
		{
			if (it->coarse > 100)
				it->coarse = it->coarse+100 > itl->coarse_max ? itl->coarse_max : it->coarse+10;
			else if (it->coarse > 10)
				it->coarse = it->coarse+5;
			else
				it->coarse = it->coarse+1;
		}
	}
	else if (exp_pos > exp_neg) //bright
	{
		ratio = (double)exp_neg/exp_pos;
//		printf("Bright %lf\n",ratio);
		if (ratio-INTEGRATION_INTERVAL < 1.0) //decrease exposure time
		{
			if (it->coarse < 10)
				it->coarse = it->coarse-1 > itl->coarse_min ? it->coarse-1 : itl->coarse_min;
			else if (it->coarse < 100)
				it->coarse = it->coarse-5;
			else
				it->coarse = it->coarse-10;


		}
	}
	else return;
}

uint8_t incr_grey(uint8_t val, bool incr)
{
	uint8_t ret = 0;

	if (!incr) return val;

	switch (val) { //current value - grey code cycle 0-1-3-2-
		case 0:
			ret = 1;
			break;
		case 1:
			ret = 3;
			break;
		case 2:
			ret = 0;
			break;
		case 3:
			ret = 2;
			break;
		default:
			ret = 0;
		}

	return ret;
}

void toggle_fp(uint32_t *fp, bool dma0, bool dma1, bool dma2)
{
	uint32_t tmp = 0;

	tmp += incr_grey(*fp & 0x3F, dma0);
	tmp += incr_grey((*fp >> 6) & 0x3F, dma1) << 6;
	tmp += incr_grey((*fp >> 12) & 0x3F, dma2) << 12;

	*fp = tmp;
}

void write_fp(uint32_t *fp)
{
//	printf("Writing fp: %x\n",*fp);

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = 0xFFFF;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_1_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(gpio_base, GPIO_DATA_REG, *fp);

	munmap((void *)gpio_base, map_len);
	close(fd);
}

void read_fp(uint32_t *fp)
{

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return;
	}

	int map_len = 0xFFFF;

	unsigned char* gpio_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)GPIO_1_BASEADDR);

	if(gpio_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	*fp = REG_READ(gpio_base, GPIO2_DATA_REG);

//	printf("Read fp: %x\n",*fp);

	munmap((void *)gpio_base, map_len);
	close(fd);
}

bool compare_fp_part(uint32_t fp1, uint32_t fp2)
{
	if (fp1 == 0 || fp1 == 3)
		if (fp2 == 0 || fp2 == 3)
			return true;
	if (fp1 == 1 || fp1 == 2)
		if (fp2 == 1 || fp2 == 2)
			return true;
	return false;
}

bool compare_fp(uint32_t fp1, uint32_t fp2)
{
	return compare_fp_part(fp1 & 0x3f, fp2 & 0x3f) & compare_fp_part((fp1>>6) & 0x3f, (fp2>>6) & 0x3f) & compare_fp_part((fp1>>12) & 0x3f, (fp2>>12) & 0x3f);
}


void toggle_cam_stream(uint32_t *pin_data)
{
	set_pin_flip(pin_data, CAM_STREAM);
}

void set_gain_register(uint16_t value, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1)
{
    //global gain
	dual_write_cam_register(pin_data, pin_dir, siod0, sioc0, siod1, sioc1, 0x305E, value);
	printf("0x305E - Global Gain: 0x%.4x \r\n", read_cam_register(pin_data, pin_dir, siod0, sioc0, 0x305E));
}
