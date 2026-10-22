#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "tx_api.h"
#include "force_sensor.h"

typedef struct {
    force_sensor_data_t latest;
    uint16_t raw_min;
    uint16_t raw_max;
    uint16_t sample_count;
    uint32_t raw_mean;
    uint32_t millivolts_mean;
    UINT last_read_status;
    bool sample_valid;
} sensor_manager_data_t;

UINT sensor_manager_init(ADC_HandleTypeDef *adc);
UINT sensor_manager_get_latest(sensor_manager_data_t *data);

#endif
