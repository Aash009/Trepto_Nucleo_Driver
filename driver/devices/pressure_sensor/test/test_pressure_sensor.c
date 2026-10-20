#include "unity.h"
#include "fake_i2c.h"
#include "rcm_pressure_sensor.h"
static fake_i2c_t fake;
static rcm_i2c_t bus;
static rcm_pressure_sensor_t sensor;
void setUp(void)
{
    const fake_i2c_t empty = {0};
    fake = empty;
    bus = fake_i2c_bind(&fake);
    TEST_ASSERT_EQUAL(RCM_OK, rcm_pressure_sensor_init(&sensor, &bus, 0x48U));
}
void tearDown(void) {}
static void decode_boundaries(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.0f, rcm_pressure_sensor_decode(0U));
    TEST_ASSERT_EQUAL_FLOAT(0.5f, rcm_pressure_sensor_decode(1U));
    TEST_ASSERT_EQUAL_FLOAT(101.0f, rcm_pressure_sensor_decode(202U));
    TEST_ASSERT_EQUAL_FLOAT(127.5f, rcm_pressure_sensor_decode(255U));
}
static void read_uses_expected_transaction(void)
{
    float value = -999.0f;
    fake.samples[0x48] = 25U;
    TEST_ASSERT_EQUAL(RCM_OK, rcm_pressure_sensor_read_kpa(&sensor, &value));
    TEST_ASSERT_EQUAL_FLOAT(12.5f, value);
    TEST_ASSERT_EQUAL_UINT(1U, fake.calls);
    TEST_ASSERT_EQUAL_HEX8(0x48U, fake.address);
    TEST_ASSERT_EQUAL_HEX8(0x00U, fake.reg);
    TEST_ASSERT_EQUAL_UINT(1U, fake.length);
    TEST_ASSERT_EQUAL_UINT32(25U, fake.timeout_ms);
}
static void failures_preserve_output_and_propagate(void)
{
    const rcm_status_t failures[] = {RCM_TIMEOUT, RCM_BUS_ERROR, RCM_NOT_READY};
    for (size_t i = 0U; i < sizeof(failures) / sizeof(failures[0]); ++i) {
        float value = -999.0f;
        fake.status = failures[i];
        TEST_ASSERT_EQUAL(failures[i], rcm_pressure_sensor_read_kpa(&sensor, &value));
        TEST_ASSERT_EQUAL_FLOAT(-999.0f, value);
    }
}
static void invalid_arguments_do_not_touch_bus(void)
{
    float value = 12.0f;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_read_kpa(NULL, &value));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_read_kpa(&sensor, NULL));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(NULL, &bus, 0x48U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(&sensor, NULL, 0x48U));
    TEST_ASSERT_EQUAL(RCM_NOT_READY, rcm_pressure_sensor_read_kpa(&sensor, &value));
    TEST_ASSERT_EQUAL_FLOAT(12.0f, value);
    TEST_ASSERT_EQUAL_UINT(0U, fake.calls);
}
static void invalid_configuration_clears_previous_binding(void)
{
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(&sensor, &bus, 0x07U));
    TEST_ASSERT_NULL(sensor.bus);
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(&sensor, &bus, 0x78U));
    bus.timeout_ms = 0U;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(&sensor, &bus, 0x48U));
    bus = fake_i2c_bind(&fake);
    bus.read_register = NULL;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_pressure_sensor_init(&sensor, &bus, 0x48U));
    TEST_ASSERT_EQUAL_UINT(0U, fake.calls);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(decode_boundaries);
    RUN_TEST(read_uses_expected_transaction);
    RUN_TEST(failures_preserve_output_and_propagate);
    RUN_TEST(invalid_arguments_do_not_touch_bus);
    RUN_TEST(invalid_configuration_clears_previous_binding);
    return UNITY_END();
}
