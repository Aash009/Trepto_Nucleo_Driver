#include "unity.h"
#include "fake_i2c.h"
static fake_i2c_t fake;
static rcm_i2c_t bus;
void setUp(void)
{
    const fake_i2c_t empty = {0};
    fake = empty;
    bus = fake_i2c_bind(&fake);
}
void tearDown(void) {}
static void forwards_transaction_and_backend_status(void)
{
    uint8_t byte = 0U;
    fake.samples[0x48] = 0xA5U;
    TEST_ASSERT_EQUAL(RCM_OK, rcm_i2c_read_register(&bus, 0x48U, 0x11U, &byte, 1U));
    TEST_ASSERT_EQUAL_HEX8(0xA5U, byte);
    TEST_ASSERT_EQUAL_HEX8(0x11U, fake.reg);
    TEST_ASSERT_EQUAL_UINT32(25U, fake.timeout_ms);
    fake.status = RCM_TIMEOUT;
    TEST_ASSERT_EQUAL(RCM_TIMEOUT, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, 1U));
    fake.status = RCM_BUS_ERROR;
    TEST_ASSERT_EQUAL(RCM_BUS_ERROR, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, 1U));
}
static void rejects_invalid_inputs_without_calling_backend(void)
{
    uint8_t byte = 0U;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(NULL, 0x48U, 0U, &byte, 1U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x48U, 0U, NULL, 1U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, 0U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, (size_t)UINT16_MAX + 1U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x07U, 0U, &byte, 1U));
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x78U, 0U, &byte, 1U));
    bus.timeout_ms = 0U;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, 1U));
    bus = fake_i2c_bind(&fake);
    bus.read_register = NULL;
    TEST_ASSERT_EQUAL(RCM_INVALID_ARGUMENT, rcm_i2c_read_register(&bus, 0x48U, 0U, &byte, 1U));
    TEST_ASSERT_EQUAL_UINT(0U, fake.calls);
}
static void accepts_supported_address_boundaries(void)
{
    uint8_t byte = 0U;
    TEST_ASSERT_EQUAL(RCM_OK, rcm_i2c_read_register(&bus, 0x08U, 0U, &byte, 1U));
    TEST_ASSERT_EQUAL(RCM_OK, rcm_i2c_read_register(&bus, 0x77U, 0U, &byte, 1U));
    TEST_ASSERT_EQUAL_UINT(2U, fake.calls);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(forwards_transaction_and_backend_status);
    RUN_TEST(rejects_invalid_inputs_without_calling_backend);
    RUN_TEST(accepts_supported_address_boundaries);
    return UNITY_END();
}
