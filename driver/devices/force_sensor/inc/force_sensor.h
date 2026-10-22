#ifndef FORCE_SENSOR_H
#define FORCE_SENSOR_H

#include <stdint.h>
#include "stm32u5xx_hal.h"

#define FORCE_SENSOR_VDD_MV       1800U
#define FORCE_SENSOR_VREF_MV       300U
#define FORCE_SENSOR_GAIN           12U

typedef struct {
    uint16_t raw_adc;
    uint16_t millivolts;
} force_sensor_data_t;

HAL_StatusTypeDef force_sensor_init(ADC_HandleTypeDef *adc);
HAL_StatusTypeDef force_sensor_read_raw(uint16_t *raw);
HAL_StatusTypeDef force_sensor_read_mv(uint16_t *millivolts);
HAL_StatusTypeDef force_sensor_read(force_sensor_data_t *data);

#endif
