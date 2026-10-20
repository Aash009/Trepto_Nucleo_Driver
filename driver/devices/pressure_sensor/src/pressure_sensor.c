#include "rcm_pressure_sensor.h"
float rcm_pressure_sensor_decode(uint8_t raw)
{
    return (float)raw * 0.5f;
}
rcm_status_t rcm_pressure_sensor_init(rcm_pressure_sensor_t *sensor, const rcm_i2c_t *bus, uint8_t addr7)
{
    if (sensor == NULL) {
        return RCM_INVALID_ARGUMENT;
    }
    sensor->bus = NULL;
    sensor->addr7 = 0U;
    if ((bus == NULL) || (bus->read_register == NULL) || (bus->timeout_ms == 0U) ||
        (addr7 < 0x08U) || (addr7 > 0x77U)) {
        return RCM_INVALID_ARGUMENT;
    }
    sensor->bus = bus;
    sensor->addr7 = addr7;
    return RCM_OK;
}
rcm_status_t rcm_pressure_sensor_read_kpa(const rcm_pressure_sensor_t *sensor, float *value)
{
    uint8_t raw = 0U;
    if ((sensor == NULL) || (value == NULL)) {
        return RCM_INVALID_ARGUMENT;
    }
    if (sensor->bus == NULL) {
        return RCM_NOT_READY;
    }
    const rcm_status_t status =
        rcm_i2c_read_register(sensor->bus, sensor->addr7, 0x00U, &raw, 1U);
    if (status == RCM_OK) {
        *value = rcm_pressure_sensor_decode(raw);
    }
    return status;
}
