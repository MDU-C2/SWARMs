/*
 * axi_gpio.h
 *
 *  Created on: 2013-06-12
 *      Author: Carl
 */

#ifndef AXI_GPIO_H_
#define AXI_GPIO_H_

#include <stdbool.h>

#define SLEEP_T 5

#define SIOC_0 0
#define SIOD_0 1
#define SIOC_1 2
#define SIOD_1 3
#define LED_0 4
#define LED_1 5
#define CAM_RESET 6
#define FPGA_SOFT_RESET 7
#define FPGA_CLK_RESET 8
#define CAM_STREAM 9

//#define SIOC_0_PIN 0
//#define SIOD_0_PIN 1
//#define SIOC_1_PIN 2
//#define SIOD_1_PIN 3
//#define LED_0_PIN 4
//#define LED_1_PIN 5
//#define CAM_RESET_PIN 6
//#define FPGA_SOFT_RESET_PIN 7

//#define SIOC_0 1 << SIOC_0_PIN
//#define SIOD_0 1 << SIOD_0_PIN
//#define SIOC_1 1 << SIOC_1_PIN
//#define SIOD_1 1 << SIOD_1_PIN
//#define LED_0 1 << LED_0_PIN
//#define LED_1 1 << LED_1_PIN
//#define CAM_RESET 1 << CAM_RESET_PIN
//#define FPGA_SOFT_RESET 1 << FPGA_SOFT_RESET_PIN

//typedef struct {
//	unsigned char* gpio_base;
//	uint32_t pin_data;
//	uint32_t pin_dir;
//	uint32_t data_offset;
//	uint32_t dir_offset;
//} x_gpio_struct;
//
//typedef struct {
//	x_gpio_struct* xgs;
//	uint8_t sioc;
//	uint8_t siod;
//} cam_sccb_struct;
//
//void write_cam_register(cam_sccb_struct *css, uint16_t waddr, uint8_t wdata);
//uint8_t read_cam_register(cam_sccb_struct *css, uint16_t waddr);
//void write_cam_base_registers(cam_sccb_struct *css);

//void fpga_soft_reset(x_gpio_struct *xgs);
//void cam_reset(x_gpio_struct *xgs);

void axi_gpio_init_dir();
void axi_gpio_init_val(uint32_t *pin_data);
void axi_gpio_no_init_val(uint32_t *pin_data);
void cam_reset(uint32_t *pin_data);
void fpga_clk_reset(uint32_t *pin_data);
void fpga_pipe_reset(uint32_t *pin_data);
void init_cams(uint32_t *pin_data, uint32_t *pin_dir);
void init_cam0(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc);
void init_cam1(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc);
void set_pin_low(uint32_t *pin_data, uint32_t pin);
void set_pin_high(uint32_t *pin_data, uint32_t pin);
void set_pin_flip(uint32_t *pin_data, uint32_t pin);

//void cam_sync(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1);
void init_cams_sync(uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1);

void set_harris_th(bool corner, uint32_t th);

//----------------

typedef struct
{
uint16_t fine_min;
uint16_t fine_max;
uint16_t coarse_min;
uint16_t coarse_max;
} integration_time_limits_t;
void get_integration_time_limits(integration_time_limits_t *itl, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0);
void print_integration_time_limits(integration_time_limits_t *itl);

typedef struct
{
uint16_t fine;
uint16_t coarse;
} integration_times_t;
void get_integration_time(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc);
void set_integration_time_dual(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1);
void set_integration_time(integration_times_t *it, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod, uint32_t sioc);

void print_integration_time(integration_times_t *it);

#define INTEGRATION_INTERVAL 0.2
void calc_integration(uint32_t exp_pos, uint32_t exp_neg, integration_times_t *it, integration_time_limits_t *itl);

void toggle_fp(uint32_t *fp,bool dma0, bool dma1, bool dma2);
void write_fp(uint32_t *fp);
void read_fp(uint32_t *fp);

bool compare_fp(uint32_t fp1, uint32_t fp2);

void toggle_cam_stream(uint32_t *pin_data);

void set_gain_register(uint16_t value, uint32_t *pin_data, uint32_t *pin_dir, uint32_t siod0, uint32_t sioc0, uint32_t siod1, uint32_t sioc1);

#endif /* AXI_GPIO_H_ */
