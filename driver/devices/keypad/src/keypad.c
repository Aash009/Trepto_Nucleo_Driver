/**
 * @file    keypad.c
 * @brief   Membrane keypad driver implementation
 * @details Direct GPIO scan, integrator debounce, per-key FSM, event FIFO.
 *          MISRA C:2012 compliant. No MCU-specific headers.
 */
#include "keypad.h"

/*---------------------------------------------------------------------------
 * Private Constants
 *---------------------------------------------------------------------------*/
#define STUCK_KEY_NONE  0xFFU

/*---------------------------------------------------------------------------
 * Private Helper: FIFO Operations
 *---------------------------------------------------------------------------*/
static bool fifo_push(keypad_fifo_t *fifo, const key_event_t *evt)
{
    bool success = false;

    if (fifo->count < KEYPAD_EVENT_FIFO_SIZE) {
        fifo->events[fifo->head] = *evt;
        fifo->head = (fifo->head + 1U) % KEYPAD_EVENT_FIFO_SIZE;
        fifo->count++;
        success = true;
    } else {
        fifo->overflow = true;
    }

    return success;
}

static bool fifo_pop(keypad_fifo_t *fifo, key_event_t *evt)
{
    bool success = false;

    if (fifo->count > 0U) {
        *evt = fifo->events[fifo->tail];
        fifo->tail = (fifo->tail + 1U) % KEYPAD_EVENT_FIFO_SIZE;
        fifo->count--;
        success = true;
    }

    return success;
}

/*---------------------------------------------------------------------------
 * Private Helper: Read Pin with Polarity
 *---------------------------------------------------------------------------*/
static bool read_key_active(const keypad_t *dev, uint8_t key_id)
{
    uint8_t pin_id = dev->cfg->pin_map[key_id];
    bool pin_state = dev->ops->read_pin(pin_id, dev->ops->ctx);

    return dev->cfg->active_low ? !pin_state : pin_state;
}

/*---------------------------------------------------------------------------
 * Private Helper: Push Event to FIFO
 *---------------------------------------------------------------------------*/
static void emit_event(keypad_t *dev, key_id_t key_id, key_event_type_t type)
{
    key_event_t evt;
    evt.key_id = key_id;
    evt.type = type;
    evt.timestamp_ms = dev->current_time_ms;
    (void)fifo_push(&dev->fifo, &evt);
}

/*---------------------------------------------------------------------------
 * Private Helper: Per-Key State Machine
 *---------------------------------------------------------------------------*/
static void process_key(keypad_t *dev, uint8_t key_id)
{
    key_internal_t *key = &dev->keys[key_id];
    bool active = read_key_active(dev, key_id);
    uint32_t elapsed = dev->current_time_ms - key->state_enter_ms;
    uint32_t hold_time;

    switch (key->state) {

    case KEY_STATE_IDLE:
        if (active) {
            key->state = KEY_STATE_DEBOUNCE_PRESS;
            key->state_enter_ms = dev->current_time_ms;
        }
        break;

    case KEY_STATE_DEBOUNCE_PRESS:
        if (!active) {
            /* Glitch - return to idle */
            key->state = KEY_STATE_IDLE;
        } else if (elapsed >= dev->cfg->debounce_ms) {
            /* Stable press confirmed */
            key->state = KEY_STATE_PRESSED;
            key->state_enter_ms = dev->current_time_ms;
            key->press_start_ms = dev->current_time_ms;
            key->long_press_fired = false;
            emit_event(dev, key_id, KEY_EVT_DOWN);
        }
        break;

    case KEY_STATE_PRESSED:
        if (!active) {
            key->state = KEY_STATE_DEBOUNCE_RELEASE;
            key->state_enter_ms = dev->current_time_ms;
        } else {
            hold_time = dev->current_time_ms - key->press_start_ms;

            if (hold_time >= dev->cfg->long_press_ms) {
                key->state = KEY_STATE_HELD;
                key->state_enter_ms = dev->current_time_ms;

                if (!key->long_press_fired) {
                    emit_event(dev, key_id, KEY_EVT_LONG_PRESS);
                    key->long_press_fired = true;
                }
            }

            /* Check stuck-key fault */
            if (hold_time >= dev->cfg->stuck_key_ms) {
                dev->stuck_key_id = key_id;
            }
        }
        break;

    case KEY_STATE_HELD:
        if (!active) {
            key->state = KEY_STATE_DEBOUNCE_RELEASE;
            key->state_enter_ms = dev->current_time_ms;
        } else {
            /* Check stuck-key fault */
            hold_time = dev->current_time_ms - key->press_start_ms;
            if (hold_time >= dev->cfg->stuck_key_ms) {
                dev->stuck_key_id = key_id;
            }
        }
        break;

    case KEY_STATE_DEBOUNCE_RELEASE:
        if (active) {
            /* Bounce - return to previous pressed state */
            if (key->long_press_fired) {
                key->state = KEY_STATE_HELD;
            } else {
                key->state = KEY_STATE_PRESSED;
            }
            key->state_enter_ms = dev->current_time_ms;
        } else if (elapsed >= dev->cfg->debounce_ms) {
            /* Stable release confirmed */
            emit_event(dev, key_id, KEY_EVT_UP);

            /* Emit short press only if long press was not fired */
            if (!key->long_press_fired) {
                emit_event(dev, key_id, KEY_EVT_SHORT_PRESS);
            }

            key->state = KEY_STATE_IDLE;
            key->state_enter_ms = dev->current_time_ms;
        }
        break;

    default:
        /* Invalid state - reset to idle */
        key->state = KEY_STATE_IDLE;
        break;
    }
}

/*---------------------------------------------------------------------------
 * Public API Implementation
 *---------------------------------------------------------------------------*/

keypad_status_t keypad_init(keypad_t *dev,
                            const keypad_config_t *cfg,
                            const keypad_gpio_ops_t *ops)
{
    uint8_t i;

    /* Validate arguments */
    if ((dev == NULL) || (cfg == NULL) || (ops == NULL)) {
        return KEYPAD_INVALID_ARG;
    }

    if ((ops->read_pin == NULL) || (cfg->pin_map == NULL)) {
        return KEYPAD_INVALID_ARG;
    }

    if ((cfg->key_count == 0U) || (cfg->key_count > KEYPAD_MAX_KEYS)) {
        return KEYPAD_INVALID_ARG;
    }

    if ((cfg->debounce_ms == 0U) || (cfg->long_press_ms == 0U)) {
        return KEYPAD_INVALID_ARG;
    }

    /* Initialize instance */
    dev->cfg = cfg;
    dev->ops = ops;
    dev->current_time_ms = 0U;
    dev->stuck_key_id = STUCK_KEY_NONE;

    /* Initialize per-key state */
    for (i = 0U; i < cfg->key_count; i++) {
        dev->keys[i].state = KEY_STATE_IDLE;
        dev->keys[i].state_enter_ms = 0U;
        dev->keys[i].press_start_ms = 0U;
        dev->keys[i].long_press_fired = false;
    }

    /* Initialize FIFO */
    dev->fifo.head = 0U;
    dev->fifo.tail = 0U;
    dev->fifo.count = 0U;
    dev->fifo.overflow = false;

    dev->initialized = true;

    return KEYPAD_OK;
}

keypad_status_t keypad_scan(keypad_t *dev, uint32_t delta_ms)
{
    uint8_t i;

    if (dev == NULL) {
        return KEYPAD_INVALID_ARG;
    }

    if (!dev->initialized) {
        return KEYPAD_NOT_READY;
    }

    /* Advance time */
    dev->current_time_ms += delta_ms;

    /* Process each key */
    for (i = 0U; i < dev->cfg->key_count; i++) {
        process_key(dev, i);
    }

    /* Return fault status if stuck key detected */
    if (dev->stuck_key_id != STUCK_KEY_NONE) {
        return KEYPAD_STUCK_KEY_FAULT;
    }

    return KEYPAD_OK;
}

keypad_status_t keypad_get_event(keypad_t *dev, key_event_t *event)
{
    if ((dev == NULL) || (event == NULL)) {
        return KEYPAD_INVALID_ARG;
    }

    if (!dev->initialized) {
        return KEYPAD_NOT_READY;
    }

    if (!fifo_pop(&dev->fifo, event)) {
        return KEYPAD_FIFO_EMPTY;
    }

    return KEYPAD_OK;
}

bool keypad_is_pressed(const keypad_t *dev, key_id_t key_id)
{
    if ((dev == NULL) || (!dev->initialized) || (key_id >= dev->cfg->key_count)) {
        return false;
    }

    key_state_t state = dev->keys[key_id].state;
    return (state == KEY_STATE_PRESSED) || (state == KEY_STATE_HELD);
}

uint8_t keypad_get_stuck_key(const keypad_t *dev)
{
    if ((dev == NULL) || (!dev->initialized)) {
        return STUCK_KEY_NONE;
    }

    return dev->stuck_key_id;
}

void keypad_clear_stuck_fault(keypad_t *dev)
{
    if ((dev != NULL) && dev->initialized) {
        dev->stuck_key_id = STUCK_KEY_NONE;
    }
}

bool keypad_fifo_overflowed(const keypad_t *dev)
{
    if ((dev == NULL) || (!dev->initialized)) {
        return false;
    }

    return dev->fifo.overflow;
}

void keypad_clear_overflow(keypad_t *dev)
{
    if ((dev != NULL) && dev->initialized) {
        dev->fifo.overflow = false;
    }
}
