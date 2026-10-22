#ifndef ADC_H
#define ADC_H

#include "stm32u5xx_hal.h"

HAL_StatusTypeDef adc_init(ADC_HandleTypeDef *adc);
HAL_StatusTypeDef adc_read(ADC_HandleTypeDef *adc, uint16_t *raw);

#endif
