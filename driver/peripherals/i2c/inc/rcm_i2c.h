#ifndef RCM_I2C_H
#define RCM_I2C_H
#include <stddef.h>
#include <stdint.h>

typedef enum {
    RCM_OK = 0,
    RCM_INVALID_ARGUMENT,
    RCM_TIMEOUT,
    RCM_BUS_ERROR,
    RCM_NOT_READY
} rcm_status_t;

/* Callback performs ONE register-read transaction, including required repeated
 * START semantics. Backend owns bus locking, finite timeout, and HAL mapping.
 * Context may be NULL for a stateless backend. No buffers may be retained. */
typedef rcm_status_t (*rcm_i2c_read_register_fn)(
    void *context, uint8_t address, uint8_t reg,
    uint8_t *data, size_t length, uint32_t timeout_ms);

typedef struct {
    void *context;
    rcm_i2c_read_register_fn read_register;
    uint32_t timeout_ms;
} rcm_i2c_t;

rcm_status_t rcm_i2c_read_register(const rcm_i2c_t *bus, uint8_t address,
                                 uint8_t reg, uint8_t *data, size_t length);
#endif
