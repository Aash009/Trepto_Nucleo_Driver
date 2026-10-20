#include "rcm_i2c.h"
rcm_status_t rcm_i2c_read_register(const rcm_i2c_t *bus, uint8_t address,
                                 uint8_t reg, uint8_t *data, size_t length)
{
    if ((bus == NULL) || (bus->read_register == NULL) ||
        (bus->timeout_ms == 0U) || (address < 0x08U) || (address > 0x77U) ||
        (data == NULL) || (length == 0U) || (length > UINT16_MAX)) {
        return RCM_INVALID_ARGUMENT;
    }
    return bus->read_register(bus->context, address, reg, data, length,
                              bus->timeout_ms);
}
