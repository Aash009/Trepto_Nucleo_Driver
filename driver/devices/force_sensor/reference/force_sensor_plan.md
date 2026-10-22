# Force Sensor Driver & ThreadX Sensor Manager Implementation Plan

## Overview
Implement a clean, simple, and layered force sensor subsystem for the **STM32U585** target based on the **HSFPAR304A** force sensor and **INA333** instrumentation amplifier front-end, porting relevant logic from `Alpine20N`.

---

## Technical Specifications & Calculations
- **MCU Pin**: `PA4` &rarr; `ADC1_IN9` (Single-ended conversion).
- **Supply Voltage ($V_{DD}$)**: `1.8 V` (`1800 mV`).
- **AFE Reference Voltage ($V_{REF}$)**: `0.3 V` (`300 mV`).
- **AFE Gain ($G$)**: `12`.
- **ADC Resolution**: 12-bit (`4095` counts max).
- **Voltage Calculation**:
  $$\text{mV} = \frac{\text{raw\_adc} \times 1800}{4095}$$
- **Force Function Behavior**:
  Retains the force calculation contract; reports `raw_adc`, `millivolts`, `diff_mv`, and sets `force_n` without stripping the force interface.

---

## Architectural Breakdown

```
firmware/
├── driver/
│   ├── peripherals/adc/             <-- Layer 1: Hardware ADC peripheral access
│   └── devices/force_sensor/        <-- Layer 2: HSFPAR304A + INA333 device driver
├── system/sensor_manager/           <-- Layer 3: ThreadX Thread & stats management
└── Core/Src/app_threadx.c           <-- Integration: Thread hook in App_ThreadX_Init()
```

---

## Proposed Changes

### 1. Peripheral Layer: [`driver/peripherals/adc`](file:///D:/Treptoflo_Firmware/firmware/driver/peripherals/adc)
- **Files**:
  - `driver/peripherals/adc/inc/adc.h`
  - `driver/peripherals/adc/src/adc.c`
- **Responsibilities**:
  - Enable ADC clock & configure GPIO `PA4` as analog mode (`ADC1_IN9`).
  - Self-calibrate ADC1 (`HAL_ADCEx_Calibration_Start`).
  - Read raw 12-bit ADC value cleanly using `HAL_ADC_Start` & `HAL_ADC_PollForConversion`.

---

### 2. Device Driver Layer: [`driver/devices/force_sensor`](file:///D:/Treptoflo_Firmware/firmware/driver/devices/force_sensor)
- **Files**:
  - `driver/devices/force_sensor/inc/force_sensor.h`
  - `driver/devices/force_sensor/src/force_sensor.c`
- **Responsibilities**:
  - Zero RTOS dependency.
  - Defines hardware constants: `FORCE_SENSOR_VDD_MV = 1800`, `FORCE_SENSOR_VREF_MV = 300`, `FORCE_SENSOR_GAIN = 12`.
  - Exposes:
    - `force_sensor_init()`
    - `force_sensor_read_raw(uint16_t *raw)`
    - `force_sensor_read_mv(uint16_t *millivolts)`
    - `force_sensor_read(force_sensor_data_t *data)`: populates `raw_adc` and `millivolts`; force conversion is pending calibration.

---

### 3. System Service Layer: [`system/sensor_manager`](file:///D:/Treptoflo_Firmware/firmware/system/sensor_manager)
- **Files**:
  - `system/sensor_manager/inc/sensor_manager.h`
  - `system/sensor_manager/src/sensor_manager.c`
- **Responsibilities**:
  - **ThreadX Thread Owner**: Creates `TX_THREAD` worker (`sensor_manager_thread`).
  - Periodic execution: sleeps for configured ticks (e.g., 10 ms).
  - Collects samples, tracks window statistics (`sample_count`, `raw_min`, `raw_max`, `raw_mean`, `mv_mean`).
  - Thread-safe data access via mutex or simple atomic copy (`sensor_manager_get_latest()`).

---

### 4. Integration: [`Core/Src/app_threadx.c`](file:///D:/Treptoflo_Firmware/firmware/Core/Src/app_threadx.c)
- Call `sensor_manager_init(&hadc1)` inside `App_ThreadX_Init()`.
- Ensure `HAL_ADC_MODULE_ENABLED` is active in `Core/Inc/stm32u5xx_hal_conf.h`.

---

## Verification Plan
1. Validate header guards, include paths, and absence of compile warnings/conflicts.
2. Verify calculations:
   - Raw `0` &rarr; `0 mV`
   - Raw `2048` &rarr; `900 mV`
   - Raw `4095` &rarr; `1800 mV`
3. Verify thread initialization arguments and stack size sanity in ThreadX.
