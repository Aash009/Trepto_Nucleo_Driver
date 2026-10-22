#include "adc.h"

#define ADC_CHANNEL              ADC_CHANNEL_9
#define ADC_TIMEOUT_MS           10U
#define ADC_MAX_RAW              4095U

HAL_StatusTypeDef adc_init(ADC_HandleTypeDef *adc)
{
    ADC_ChannelConfTypeDef channel = {0};

    if ((adc == NULL) || (adc->Instance != ADC1)) {
        return HAL_ERROR;
    }

    channel.Channel = ADC_CHANNEL;
    channel.Rank = ADC_REGULAR_RANK_1;
    channel.SamplingTime = ADC_SAMPLETIME_814CYCLES;
    channel.SingleDiff = ADC_SINGLE_ENDED;
    channel.OffsetNumber = ADC_OFFSET_NONE;
    channel.Offset = 0U;

    if (HAL_ADC_ConfigChannel(adc, &channel) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_ADCEx_Calibration_Start(adc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
}

HAL_StatusTypeDef adc_read(ADC_HandleTypeDef *adc, uint16_t *raw)
{
    uint32_t value;
    HAL_StatusTypeDef status;

    if ((adc == NULL) || (raw == NULL) || (adc->Instance != ADC1)) {
        return HAL_ERROR;
    }

    status = HAL_ADC_Start(adc);
    if (status != HAL_OK) {
        return status;
    }

    status = HAL_ADC_PollForConversion(adc, ADC_TIMEOUT_MS);
    if (status == HAL_OK) {
        value = HAL_ADC_GetValue(adc);
        if (value > ADC_MAX_RAW) {
            status = HAL_ERROR;
        } else {
            *raw = (uint16_t)value;
        }
    }

    if (HAL_ADC_Stop(adc) != HAL_OK) {
        status = HAL_ERROR;
    }

    return status;
}
