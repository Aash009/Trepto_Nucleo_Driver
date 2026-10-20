/**
 * @file    test_keypad.c
 * @brief   Unit tests for keypad driver
 * @details Tests run headless with mock GPIO array. Uses Unity framework.
 */
#include "unity.h"
#include "keypad.h"

/*---------------------------------------------------------------------------
 * Mock GPIO Backend
 *---------------------------------------------------------------------------*/
#define TEST_KEY_COUNT  4U

static bool mock_pins[TEST_KEY_COUNT];
static uint8_t mock_pin_map[TEST_KEY_COUNT] = {0U, 1U, 2U, 3U};

static bool mock_read_pin(uint8_t pin_id, void *ctx)
{
    (void)ctx;
    if (pin_id < TEST_KEY_COUNT) {
        return mock_pins[pin_id];
    }
    return false;
}

/*---------------------------------------------------------------------------
 * Test Fixtures
 *---------------------------------------------------------------------------*/
static keypad_t keypad;
static keypad_config_t cfg;
static keypad_gpio_ops_t ops;

void setUp(void)
{
    uint8_t i;

    for (i = 0U; i < TEST_KEY_COUNT; i++) {
        mock_pins[i] = true;  /* Active-low: HIGH = not pressed */
    }

    cfg.key_count = TEST_KEY_COUNT;
    cfg.pin_map = mock_pin_map;
    cfg.active_low = true;
    cfg.debounce_ms = 20U;
    cfg.long_press_ms = 1000U;
    cfg.stuck_key_ms = 30000U;

    ops.read_pin = mock_read_pin;
    ops.ctx = NULL;

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_init(&keypad, &cfg, &ops));
}

void tearDown(void)
{
}

/*---------------------------------------------------------------------------
 * Helper: Simulate time stepping
 *---------------------------------------------------------------------------*/
static void step_time_ms(uint32_t ms)
{
    keypad_scan(&keypad, ms);
}

static void press_key(uint8_t key_id)
{
    mock_pins[key_id] = false;  /* Active-low */
}

static void release_key(uint8_t key_id)
{
    mock_pins[key_id] = true;
}

/*---------------------------------------------------------------------------
 * Test: Initialization Validation
 *---------------------------------------------------------------------------*/
static void test_init_null_pointers(void)
{
    keypad_t dev;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(NULL, &cfg, &ops));
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, NULL, &ops));
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &cfg, NULL));
}

static void test_init_invalid_config(void)
{
    keypad_t dev;
    keypad_config_t bad_cfg = cfg;
    keypad_gpio_ops_t bad_ops = ops;

    bad_cfg.key_count = 0U;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &bad_cfg, &ops));

    bad_cfg = cfg;
    bad_cfg.key_count = KEYPAD_MAX_KEYS + 1U;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &bad_cfg, &ops));

    bad_cfg = cfg;
    bad_cfg.debounce_ms = 0U;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &bad_cfg, &ops));

    bad_cfg = cfg;
    bad_cfg.pin_map = NULL;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &bad_cfg, &ops));

    bad_ops.read_pin = NULL;
    TEST_ASSERT_EQUAL(KEYPAD_INVALID_ARG, keypad_init(&dev, &cfg, &bad_ops));
}

/*---------------------------------------------------------------------------
 * Test: Glitch / Bounce Rejection
 *---------------------------------------------------------------------------*/
static void test_glitch_rejection(void)
{
    key_event_t evt;

    press_key(0U);
    step_time_ms(10U);  /* Less than debounce */
    release_key(0U);
    step_time_ms(10U);

    TEST_ASSERT_EQUAL(KEYPAD_FIFO_EMPTY, keypad_get_event(&keypad, &evt));
}

/*---------------------------------------------------------------------------
 * Test: Clean Press and Release
 *---------------------------------------------------------------------------*/
static void test_clean_press_release(void)
{
    key_event_t evt;

    press_key(0U);
    step_time_ms(25U);  /* Past debounce */

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(0U, evt.key_id);
    TEST_ASSERT_EQUAL(KEY_EVT_DOWN, evt.type);

    release_key(0U);
    step_time_ms(25U);

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_UP, evt.type);

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_SHORT_PRESS, evt.type);

    TEST_ASSERT_EQUAL(KEYPAD_FIFO_EMPTY, keypad_get_event(&keypad, &evt));
}

/*---------------------------------------------------------------------------
 * Test: Long Press Detection
 *---------------------------------------------------------------------------*/
static void test_long_press_detection(void)
{
    key_event_t evt;

    press_key(0U);
    step_time_ms(25U);  /* DOWN event */

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_DOWN, evt.type);

    /* Hold until long press threshold */
    step_time_ms(500U);
    TEST_ASSERT_EQUAL(KEYPAD_FIFO_EMPTY, keypad_get_event(&keypad, &evt));

    step_time_ms(500U);  /* Total 1025ms */
    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_LONG_PRESS, evt.type);

    /* Release after long press */
    release_key(0U);
    step_time_ms(25U);

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_UP, evt.type);

    /* No short press after long press */
    TEST_ASSERT_EQUAL(KEYPAD_FIFO_EMPTY, keypad_get_event(&keypad, &evt));
}

/*---------------------------------------------------------------------------
 * Test: Multiple Keys (Rollover)
 *---------------------------------------------------------------------------*/
static void test_multi_key_rollover(void)
{
    key_event_t evt;

    press_key(0U);
    press_key(1U);
    step_time_ms(25U);

    /* Both keys should generate DOWN events */
    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_DOWN, evt.type);

    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));
    TEST_ASSERT_EQUAL(KEY_EVT_DOWN, evt.type);
}

/*---------------------------------------------------------------------------
 * Test: FIFO Overflow
 *---------------------------------------------------------------------------*/
static void test_fifo_overflow(void)
{
    uint8_t i;
    key_event_t evt;

    /* Generate more events than FIFO can hold */
    for (i = 0U; i < (KEYPAD_EVENT_FIFO_SIZE + 2U); i++) {
        press_key(0U);
        step_time_ms(25U);
        release_key(0U);
        step_time_ms(25U);
    }

    TEST_ASSERT_TRUE(keypad_fifo_overflowed(&keypad));

    /* Should still be able to read buffered events */
    TEST_ASSERT_EQUAL(KEYPAD_OK, keypad_get_event(&keypad, &evt));

    keypad_clear_overflow(&keypad);
    TEST_ASSERT_FALSE(keypad_fifo_overflowed(&keypad));
}

/*---------------------------------------------------------------------------
 * Test: Stuck Key Fault Detection
 *---------------------------------------------------------------------------*/
static void test_stuck_key_fault(void)
{
    keypad_status_t status;

    press_key(0U);
    step_time_ms(25U);  /* Past debounce */

    TEST_ASSERT_EQUAL(0xFFU, keypad_get_stuck_key(&keypad));

    /* Simulate holding for stuck threshold */
    step_time_ms(30000U);
    status = keypad_scan(&keypad, 0U);

    TEST_ASSERT_EQUAL(KEYPAD_STUCK_KEY_FAULT, status);
    TEST_ASSERT_EQUAL(0U, keypad_get_stuck_key(&keypad));

    keypad_clear_stuck_fault(&keypad);
    TEST_ASSERT_EQUAL(0xFFU, keypad_get_stuck_key(&keypad));
}

/*---------------------------------------------------------------------------
 * Test: is_pressed Query
 *---------------------------------------------------------------------------*/
static void test_is_pressed(void)
{
    TEST_ASSERT_FALSE(keypad_is_pressed(&keypad, 0U));

    press_key(0U);
    step_time_ms(25U);

    TEST_ASSERT_TRUE(keypad_is_pressed(&keypad, 0U));
    TEST_ASSERT_FALSE(keypad_is_pressed(&keypad, 1U));

    release_key(0U);
    step_time_ms(25U);

    TEST_ASSERT_FALSE(keypad_is_pressed(&keypad, 0U));
}

/*---------------------------------------------------------------------------
 * Test: Uninitialized Access
 *---------------------------------------------------------------------------*/
static void test_uninitialized_access(void)
{
    keypad_t uninit_dev = {0};
    key_event_t evt;

    TEST_ASSERT_EQUAL(KEYPAD_NOT_READY, keypad_scan(&uninit_dev, 10U));
    TEST_ASSERT_EQUAL(KEYPAD_NOT_READY, keypad_get_event(&uninit_dev, &evt));
    TEST_ASSERT_FALSE(keypad_is_pressed(&uninit_dev, 0U));
}

/*---------------------------------------------------------------------------
 * Test: Boundary - Key ID Out of Range
 *---------------------------------------------------------------------------*/
static void test_key_id_boundary(void)
{
    TEST_ASSERT_FALSE(keypad_is_pressed(&keypad, TEST_KEY_COUNT));
    TEST_ASSERT_FALSE(keypad_is_pressed(&keypad, 255U));
}

/*---------------------------------------------------------------------------
 * Main Test Runner
 *---------------------------------------------------------------------------*/
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init_null_pointers);
    RUN_TEST(test_init_invalid_config);
    RUN_TEST(test_glitch_rejection);
    RUN_TEST(test_clean_press_release);
    RUN_TEST(test_long_press_detection);
    RUN_TEST(test_multi_key_rollover);
    RUN_TEST(test_fifo_overflow);
    RUN_TEST(test_stuck_key_fault);
    RUN_TEST(test_is_pressed);
    RUN_TEST(test_uninitialized_access);
    RUN_TEST(test_key_id_boundary);

    return UNITY_END();
}
