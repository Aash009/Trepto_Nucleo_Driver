#include "sensor_manager.h"
#include <stdbool.h>

#define SENSOR_THREAD_STACK_SIZE 1024U
#define SENSOR_THREAD_PRIORITY   10U
#define SENSOR_PERIOD_TICKS      10U
#define SENSOR_WINDOW_SAMPLES    100U
static TX_THREAD sensor_thread;
static TX_MUTEX sensor_mutex;
static ULONG sensor_thread_stack[SENSOR_THREAD_STACK_SIZE / sizeof(ULONG)];
static sensor_manager_data_t sensor_data;
static uint32_t raw_sum;
static uint32_t millivolts_sum;
static uint16_t window_count;
static bool manager_ready;

static void sensor_manager_thread(ULONG input);
static void sensor_manager_update(const force_sensor_data_t *sample,
                                  HAL_StatusTypeDef status);

UINT sensor_manager_init(ADC_HandleTypeDef *adc)
{
    UINT status;

    if (adc == NULL) {
        return TX_PTR_ERROR;
    }

    if (force_sensor_init(adc) != HAL_OK) {
        return TX_NOT_DONE;
    }

    status = tx_mutex_create(&sensor_mutex, "Force sensor data", TX_NO_INHERIT);
    if (status != TX_SUCCESS) {
        return status;
    }

    status = tx_thread_create(&sensor_thread, "Force sensor", sensor_manager_thread,
                              0UL, sensor_thread_stack, sizeof(sensor_thread_stack),
                              SENSOR_THREAD_PRIORITY, SENSOR_THREAD_PRIORITY,
                              TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS) {
        (void)tx_mutex_delete(&sensor_mutex);
    } else {
        manager_ready = true;
    }

    return status;
}

UINT sensor_manager_get_latest(sensor_manager_data_t *data)
{
    UINT status;

    if (data == NULL) {
        return TX_PTR_ERROR;
    }
    if (!manager_ready) {
        return TX_NOT_DONE;
    }

    status = tx_mutex_get(&sensor_mutex, TX_WAIT_FOREVER);
    if (status == TX_SUCCESS) {
        *data = sensor_data;
        status = tx_mutex_put(&sensor_mutex);
    }

    return status;
}

static void sensor_manager_thread(ULONG input)
{
    force_sensor_data_t sample = {0U, 0U};

    (void)input;
    for (;;) {
        const HAL_StatusTypeDef read_status = force_sensor_read(&sample);
        sensor_manager_update(&sample, read_status);
        (void)tx_thread_sleep(SENSOR_PERIOD_TICKS);
    }
}

static void sensor_manager_update(const force_sensor_data_t *sample,
                                  HAL_StatusTypeDef status)
{
    if (tx_mutex_get(&sensor_mutex, TX_WAIT_FOREVER) != TX_SUCCESS) {
        return;
    }

    sensor_data.last_read_status = (UINT)status;
    if (status == HAL_OK) {
        sensor_data.latest = *sample;
        sensor_data.sample_valid = true;
        if (window_count == 0U) {
            sensor_data.raw_min = sample->raw_adc;
            sensor_data.raw_max = sample->raw_adc;
        } else {
            if (sample->raw_adc < sensor_data.raw_min) {
                sensor_data.raw_min = sample->raw_adc;
            }
            if (sample->raw_adc > sensor_data.raw_max) {
                sensor_data.raw_max = sample->raw_adc;
            }
        }

        raw_sum += sample->raw_adc;
        millivolts_sum += sample->millivolts;
        window_count++;
        sensor_data.sample_count = window_count;
        if (window_count == SENSOR_WINDOW_SAMPLES) {
            sensor_data.raw_mean = raw_sum / SENSOR_WINDOW_SAMPLES;
            sensor_data.millivolts_mean = millivolts_sum / SENSOR_WINDOW_SAMPLES;
            raw_sum = 0U;
            millivolts_sum = 0U;
            window_count = 0U;
            sensor_data.sample_count = 0U;
        }
    }

    (void)tx_mutex_put(&sensor_mutex);
}
