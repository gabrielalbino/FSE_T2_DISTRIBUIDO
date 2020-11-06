#ifndef __TEMPERATURECONTROLLER__
#define __TEMPERATURECONTROLLER__

#include "bme280_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include "types.h"

void BME280_shutdown();
int8_t BME280_setup();
double BME280_updateTemperature(volatile dadosServidor *temperature);
void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t stream_sensor_data_normal_mode(volatile dadosServidor *dadosServidor);
void handle_sensor_data(struct bme280_data *comp_data, volatile dadosServidor *temperature);

#endif