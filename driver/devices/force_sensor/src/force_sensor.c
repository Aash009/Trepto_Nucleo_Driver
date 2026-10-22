#include "force_sensor.h"
#include "adc.h"

#define FORCE_SENSOR_ADC_MAX 4095U

static ADC_HandleTypeDef *force_adc;

HAL_StatusTypeDef force_sensor_init(ADC_HandleTypeDef *adc)
{
    HAL_StatusTypeDef status;

    if (adc == NULL) {
        return HAL_ERROR;
    }

    status = adc_init(adc);
    if (status == HAL_OK) {
        force_adc = adc;
    }

    return status;
}

HAL_StatusTypeDef force_sensor_read_raw(uint16_t *raw)
{
    if ((force_adc == NULL) || (raw == NULL)) {
        return HAL_ERROR;
    }

    return adc_read(force_adc, raw);
}

HAL_StatusTypeDef force_sensor_read_mv(uint16_t *millivolts)
{
    uint16_t raw;
    HAL_StatusTypeDef status;

    if (millivolts == NULL) {
        return HAL_ERROR;
    }

    status = force_sensor_read_raw(&raw);
    if (status == HAL_OK) {
        *millivolts = (uint16_t)(((uint32_t)raw * FORCE_SENSOR_VDD_MV +
                                  (FORCE_SENSOR_ADC_MAX / 2U)) /
                                 FORCE_SENSOR_ADC_MAX);
    }

    return status;
}

HAL_StatusTypeDef force_sensor_read(force_sensor_data_t *data)
{
    force_sensor_data_t sample;
    HAL_StatusTypeDef status;

    if (data == NULL) {
        return HAL_ERROR;
    }

    status = force_sensor_read_raw(&sample.raw_adc);
    if (status == HAL_OK) {
        sample.millivolts = (uint16_t)(((uint32_t)sample.raw_adc *
                                        FORCE_SENSOR_VDD_MV +
                                        (FORCE_SENSOR_ADC_MAX / 2U)) /
                                       FORCE_SENSOR_ADC_MAX);
        *data = sample;
    }

    return status;
}
