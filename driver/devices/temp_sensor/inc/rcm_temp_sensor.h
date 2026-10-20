#ifndef RCM_TEMP_SENSOR_H
#define RCM_TEMP_SENSOR_H
#include "rcm_i2c.h"
/* PROTOTYPE FORMAT ONLY: not an approved physical sensor driver.
 * Bus must outlive the device. Owner serializes init/read access. */
typedef struct {
    const rcm_i2c_t *bus;
    uint8_t addr7;
} rcm_temp_sensor_t;
float rcm_temp_sensor_decode(uint8_t raw);
rcm_status_t rcm_temp_sensor_init(rcm_temp_sensor_t *sensor, const rcm_i2c_t *bus, uint8_t addr7);
/* Output is unchanged on failure. Caller must check status before using it. */
rcm_status_t rcm_temp_sensor_read_celsius(const rcm_temp_sensor_t *sensor, float *value);
#endif
