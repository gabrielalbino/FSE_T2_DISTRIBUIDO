#include "bme280_controller.h"

struct bme280_dev dispositivo;
struct identificador user_id;

void BME280_shutdown(){
    close(user_id.descritorDoArquivo);
}

int8_t BME280_setup(){
/* Variable to define the result */
  int8_t rslt = BME280_OK;

  if ((user_id.descritorDoArquivo = open("/dev/i2c-1", O_RDWR)) < 0)
  {
      return BME280_E_DEV_NOT_FOUND;
  }

  if (ioctl(user_id.descritorDoArquivo, I2C_SLAVE, 0x76) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      exit(1);
  }

  user_id.enderecoDoDispositivo = BME280_I2C_ADDR_PRIM;

  dispositivo.intf = BME280_I2C_INTF;
  dispositivo.read = user_i2c_read;
  dispositivo.write = user_i2c_write;
  dispositivo.delay_us = user_delay_us;

  /* Update interface pointer with the structure that contains both device address and file descriptor */
  dispositivo.intf_ptr = &user_id;

  /* Initialize the bme280 */
  rslt = bme280_init(&dispositivo);
  if (rslt != BME280_OK)
  {
      fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
      exit(1);
  }
  /* Recommended mode of operation: Indoor navigation */
  dispositivo.settings.osr_h = BME280_OVERSAMPLING_1X;
  dispositivo.settings.osr_p = BME280_OVERSAMPLING_16X;
  dispositivo.settings.osr_t = BME280_OVERSAMPLING_2X;
  dispositivo.settings.filter = BME280_FILTER_COEFF_16;
  dispositivo.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

  uint8_t settings_sel;
  settings_sel = BME280_OSR_PRESS_SEL;
  settings_sel |= BME280_OSR_TEMP_SEL;
  settings_sel |= BME280_OSR_HUM_SEL;
  settings_sel |= BME280_STANDBY_SEL;
  settings_sel |= BME280_FILTER_SEL;
  rslt = bme280_set_sensor_settings(settings_sel, &dispositivo);
  rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dispositivo);
  return rslt;
}

double BME280_updateTemperature(volatile dadosServidor* dadosServidor){
  int8_t rslt;
  
  rslt = stream_sensor_data_normal_mode(dadosServidor);
  if (rslt != BME280_OK)
  {
      fprintf(stderr, "Failed to stream sensor data (code %+d).\n", rslt);
      exit(1);
  }
  return 0;
}

int8_t stream_sensor_data_normal_mode(volatile dadosServidor* dadosServidor)
{
  int8_t rslt;
  struct bme280_data comp_data;

  dispositivo.delay_us(1000000, dispositivo.intf_ptr);
  rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dispositivo);
  if(rslt == BME280_OK){
    dadosServidor->temperatura = comp_data.temperature;
    dadosServidor->umidade = comp_data.humidity;
  }
  return rslt;
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identificador id;

    id = *((struct identificador *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.descritorDoArquivo, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}



/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identificador id;

    id = *((struct identificador *)intf_ptr);
    write(id.descritorDoArquivo, &reg_addr, 1);
    read(id.descritorDoArquivo, data, len);
    return 0;
}


void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}