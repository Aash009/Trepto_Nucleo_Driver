/**
 * @file    keypad.h
 * @brief   Membrane keypad driver with debounce and event classification
 * @details Direct GPIO scanning, per-key state machine, event FIFO.
 *          Software item SI-P-06 GUI, Class B.
 *          Hardware: Custom FFC membrane keypad (HAD Table 8)
 */
#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdbool.h>
#include <stdint.h>

/*---------------------------------------------------------------------------
 * Configuration Constants
 *---------------------------------------------------------------------------*/
#define KEYPAD_MAX_KEYS         5U
#define KEYPAD_EVENT_FIFO_SIZE  8U

/*---------------------------------------------------------------------------
 * Status Codes
 *---------------------------------------------------------------------------*/
typedef enum {
    KEYPAD_OK = 0,
    KEYPAD_INVALID_ARG,
    KEYPAD_NOT_READY,
    KEYPAD_FIFO_EMPTY,
    KEYPAD_FIFO_OVERFLOW,
    KEYPAD_STUCK_KEY_FAULT
} keypad_status_t;

/*---------------------------------------------------------------------------
 * Key Identification
 *---------------------------------------------------------------------------*/
typedef uint8_t key_id_t;

/*---------------------------------------------------------------------------
 * Event Types
 *---------------------------------------------------------------------------*/
typedef enum {
    KEY_EVT_NONE = 0,
    KEY_EVT_DOWN,
    KEY_EVT_UP,
    KEY_EVT_SHORT_PRESS,
    KEY_EVT_LONG_PRESS
} key_event_type_t;

/*---------------------------------------------------------------------------
 * Key Event Structure
 *---------------------------------------------------------------------------*/
typedef struct {
    key_id_t         key_id;
    key_event_type_t type;
    uint32_t         timestamp_ms;
} key_event_t;

/*---------------------------------------------------------------------------
 * Per-Key State Machine States
 *---------------------------------------------------------------------------*/
typedef enum {
    KEY_STATE_IDLE = 0,
    KEY_STATE_DEBOUNCE_PRESS,
    KEY_STATE_PRESSED,
    KEY_STATE_HELD,
    KEY_STATE_DEBOUNCE_RELEASE
} key_state_t;

/*---------------------------------------------------------------------------
 * Hardware Abstraction Interface
 *---------------------------------------------------------------------------*/
typedef struct {
    bool (*read_pin)(uint8_t pin_id, void *ctx);
    void *ctx;
} keypad_gpio_ops_t;

/*---------------------------------------------------------------------------
 * Configuration Structure
 *---------------------------------------------------------------------------*/
typedef struct {
    uint8_t              key_count;
    const uint8_t       *pin_map;          /**< Array mapping key_id -> pin_id */
    bool                 active_low;       /**< true = pressed when pin is LOW */
    uint16_t             debounce_ms;      /**< Debounce time (default 20ms) */
    uint16_t             long_press_ms;    /**< Long-press threshold (default 1000ms) */
    uint32_t             stuck_key_ms;     /**< Stuck-key fault threshold (default 30000ms) */
} keypad_config_t;

/*---------------------------------------------------------------------------
 * Per-Key Internal State
 *---------------------------------------------------------------------------*/
typedef struct {
    key_state_t state;
    uint32_t    state_enter_ms;
    uint32_t    press_start_ms;
    bool        long_press_fired;
} key_internal_t;

/*---------------------------------------------------------------------------
 * Event FIFO
 *---------------------------------------------------------------------------*/
typedef struct {
    key_event_t events[KEYPAD_EVENT_FIFO_SIZE];
    uint8_t     head;
    uint8_t     tail;
    uint8_t     count;
    bool        overflow;
} keypad_fifo_t;

/*---------------------------------------------------------------------------
 * Keypad Instance Handle
 *---------------------------------------------------------------------------*/
typedef struct {
    const keypad_config_t   *cfg;
    const keypad_gpio_ops_t *ops;
    key_internal_t           keys[KEYPAD_MAX_KEYS];
    keypad_fifo_t            fifo;
    uint32_t                 current_time_ms;
    bool                     initialized;
    volatile uint8_t         stuck_key_id;   /**< 0xFF = none, else key_id with fault */
} keypad_t;

/*---------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

/**
 * @brief  Initialize keypad driver instance
 * @param  dev  Pointer to keypad instance (caller-allocated)
 * @param  cfg  Pointer to configuration (must outlive dev)
 * @param  ops  Pointer to GPIO operations (must outlive dev)
 * @return KEYPAD_OK on success, KEYPAD_INVALID_ARG on NULL or invalid config
 */
keypad_status_t keypad_init(keypad_t *dev,
                            const keypad_config_t *cfg,
                            const keypad_gpio_ops_t *ops);

/**
 * @brief  Periodic scan - call from timer ISR or RTOS task (5-10ms interval)
 * @param  dev       Pointer to initialized keypad instance
 * @param  delta_ms  Time elapsed since last scan call
 * @return KEYPAD_OK, KEYPAD_STUCK_KEY_FAULT if fault detected, KEYPAD_NOT_READY if not init
 */
keypad_status_t keypad_scan(keypad_t *dev, uint32_t delta_ms);

/**
 * @brief  Pop next event from FIFO
 * @param  dev    Pointer to keypad instance
 * @param  event  Output: event data (unchanged on failure)
 * @return KEYPAD_OK on success, KEYPAD_FIFO_EMPTY if no events, KEYPAD_INVALID_ARG on NULL
 */
keypad_status_t keypad_get_event(keypad_t *dev, key_event_t *event);

/**
 * @brief  Check if a specific key is currently pressed
 * @param  dev     Pointer to keypad instance
 * @param  key_id  Key identifier
 * @return true if pressed (in PRESSED or HELD state), false otherwise
 */
bool keypad_is_pressed(const keypad_t *dev, key_id_t key_id);

/**
 * @brief  Get current stuck-key fault status
 * @param  dev  Pointer to keypad instance
 * @return Key ID with stuck fault, or 0xFF if no fault
 */
uint8_t keypad_get_stuck_key(const keypad_t *dev);

/**
 * @brief  Clear stuck-key fault flag (after physical issue resolved)
 * @param  dev  Pointer to keypad instance
 */
void keypad_clear_stuck_fault(keypad_t *dev);

/**
 * @brief  Check if FIFO overflow occurred (events lost)
 * @param  dev  Pointer to keypad instance
 * @return true if overflow occurred since last clear
 */
bool keypad_fifo_overflowed(const keypad_t *dev);

/**
 * @brief  Clear FIFO overflow flag
 * @param  dev  Pointer to keypad instance
 */
void keypad_clear_overflow(keypad_t *dev);

#endif /* KEYPAD_H */
