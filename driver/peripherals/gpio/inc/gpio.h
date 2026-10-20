/**
 * @file    gpio.h
 * @brief   GPIO hardware abstraction interface
 * @details Platform-agnostic GPIO operations for device drivers.
 *          Backend implements actual HAL calls; drivers use only this interface.
 */
#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    GPIO_OK = 0,
    GPIO_INVALID_ARG,
    GPIO_ERROR
} gpio_status_t;

/**
 * @brief GPIO operations interface (function pointers for HAL decoupling)
 * @note  Context may be NULL for stateless backends.
 */
typedef struct {
    bool (*read_pin)(uint8_t pin_id, void *ctx);
    gpio_status_t (*write_pin)(uint8_t pin_id, bool level, void *ctx);
    void *ctx;
} gpio_ops_t;

#endif /* GPIO_H */
