# keypad

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-06 GUI (Class B per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Membrane keypad direct GPIO scan with debounce.
Hardware: Custom FFC membrane keypad; GPIO (HAD Table 8)
Allocation source: FAD Table 19 (keypad -> GUI input handler)
Legacy (L496 project): Encoder_button_limit_driver, button part only. The retract limit / contact datum is read by the SAFETY MCU (FAD Table 19) and moves to the SAFETY project. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
  ### 1. Apart from Pin Configuration, What is in the Keypad Driver?

  Configuring GPIO pins (input, output, pull-up/pull-down) is only the raw hardware interface. A
  robust, testable embedded keypad driver contains:

  1. Hardware Abstraction / I/O Decoupling (Function Pointers):
      • Abstract callbacks for read_pin(pin_id, ctx) and write_pin(pin_id, level, ctx).
      • This keeps hardware access isolated from driver logic, allowing you to run unit tests on a
      PC without STM32 hardware or registers.
  2. Scanning Engine:
      • If Matrix (Rows × Columns): Row drive cycling (active-low/high), settling time between pin
      drive and read, column sampling, and pin state restoration.
      • If Direct GPIOs (1 pin per key): Multi-pin simultaneous sampling and bitmask decoding.
  3. Debounce Filter:
      • Mechanical membrane switches bounce for 5–20 ms.
      • The driver needs an integrator/counter or sliding window filter to filter out glitches and
      only accept stable state transitions.
  4. Key State Machine (Per-Key State Tracking):
      • States: IDLE → DEBOUNCING_PRESS → PRESSED → HELD → DEBOUNCING_RELEASE.
      • Calculates duration of the press to distinguish short clicks from hold actions.
  5. Event Generation & Classification:
      • Generates distinct application-level events:
          • KEY_EVENT_DOWN / KEY_EVENT_UP
          • KEY_EVENT_SHORT_PRESS
          • KEY_EVENT_LONG_PRESS (e.g., held > 1000 ms)
          • KEY_EVENT_REPEAT (periodic firing while held, useful for numeric increment/volume)

  6. Event FIFO / Ring Buffer:
      • Buffers press/release events so fast double-clicks or UI task latency do not lose user
      actions.
  7. Multi-Key & Ghosting Mitigation:
      • N-key rollover tracking.
      • Ghost key detection/lockout (when 3 keys are pressed simultaneously in a diode-less matrix,
      phantom 4th key is prevented).
      • Combination key detection (e.g., Key A + Key B pressed together for diagnostic/service menu).
  8. Diagnostic & Fault Detection:
      • Stuck-key fault: Alerting if a key is depressed continuously beyond a safe threshold (e.g.,
      > 30 s indicating a physical membrane short).

  ──────
  ### 2. Plan for Driver Layer (.h and .c)

  #### Header File: rcm_driver_devices_keypad.h

  Defines public data types, hardware abstraction interfaces, and API prototypes:

  • Enums & Types:
      • rcm_key_id_t: Logical keys (e.g., KEY_POWER, KEY_UP, KEY_DOWN, KEY_ENTER).
      • rcm_key_event_type_t: KEY_EVT_DOWN, KEY_EVT_UP, KEY_EVT_LONG_PRESS, KEY_EVT_REPEAT.
      • rcm_key_event_t: Struct with key_id, event_type, and timestamp_ms.
  • Hardware Operations Interface (HAL Abstraction):
    typedef struct {
        bool (*read_col)(uint8_t col_idx, void *user_data);
        void (*write_row)(uint8_t row_idx, bool level, void *user_data);
    } rcm_keypad_gpio_ops_t;

  • Configuration Struct (rcm_keypad_config_t):
      • Debounce time (e.g., 20 ms), long-press threshold (e.g., 1000 ms), repeat rate (e.g., 200
      ms), stuck-key threshold.
  • Keypad Instance Handle (rcm_keypad_t):
      • Internal states, debounce counters, timer trackers, event ring buffer, and ops pointer.
  • Public Function Prototypes:
      • rcm_status_t rcm_keypad_init(rcm_keypad_t *dev, const rcm_keypad_config_t *cfg, const
      rcm_keypad_gpio_ops_t *ops, void *user_data);
      • rcm_status_t rcm_keypad_scan(rcm_keypad_t *dev, uint32_t delta_ms); (called periodically by
      timer or RTOS task)
      • rcm_status_t rcm_keypad_get_event(rcm_keypad_t *dev, rcm_key_event_t *event); (pops event
      from queue)
      • bool rcm_keypad_is_pressed(const rcm_keypad_t *dev, rcm_key_id_t key_id);


  #### Source File: rcm_driver_devices_keypad.c

  Implements pure logic without any STM32 HAL includes:

  • Input validation (checks for NULL pointers, valid boundaries).
  • Matrix scanning algorithm using the provided ops->write_row and ops->read_col.
  • State machine updates driven by delta_ms.
  • Ring buffer push/pop operations.
  ──────
  ### 3. Unit Testing Plan (Running Headless on Host)

  To test the driver in the background without real hardware, we use a C test file (e.g., using
  Unity, matching your existing sensor tests in
  driver/devices/pressure_sensor/test/test_pressure_sensor.c).

  #### How It Works Without Hardware

  1. Mock GPIO Array in RAM:
  The test defines a simulated 2D state matrix in C memory:
    static bool mock_button_matrix[NUM_ROWS][NUM_COLS];
  The read_col mock function simply returns mock_button_matrix[active_row][col_idx].
  2. Simulated Time Stepping:
  Instead of waiting for real-world milliseconds, the test calls rcm_keypad_scan(&keypad, 5); in a
  loop. A simulated 1-second press executes in less than 1 millisecond on your CPU.

  #### Planned Unit Test Cases (test_rcm_driver_devices_keypad.c)

   Test Case                    | Scenario / Input                | Expected Result
  ------------------------------|---------------------------------|---------------------------------
   1. Init & Validation         | Call init with NULL pointers or | Returns RCM_INVALID_ARGUMENT;
                                | invalid timings.                | device stays uninitialized.
   2. Glitch / Bounce Rejection | Toggle key pin High -> Low ->   | No event is generated; state
                                | High rapidly within 10 ms (less | returns to IDLE.
                                | than 20 ms debounce).           |
   3. Clean Press & Release     | Set key pin active, step time   | Exactly two events generated:
                                | by 25 ms, then release and step | KEY_EVT_DOWN followed by
                                | 25 ms.                          | KEY_EVT_UP.
   4. Long-Press Detection      | Hold key pin active, step time  | Fires KEY_EVT_DOWN at 20 ms,
                                | to 1000 ms.                     | followed by KEY_EVT_LONG_PRESS
                                |                                 | at 1000 ms.
   5. Auto-Repeat Generation    | Hold key pin active for 1600    | Fires long press at 1000 ms,
                                | ms.                             | then repeat events at 1200 ms,
                                |                                 | 1400 ms, 1600 ms.
   6. Multi-Key Rollover        | Set Key 1 and Key 2 active      | Both keys debounced
                                | simultaneously.                 | independently; both events
                                |                                 | recorded in the FIFO.
   7. FIFO Queue Overflow       | Generate 10 rapid events with   | Buffer absorbs first 8 events;
                                | an 8-item buffer without        | gracefully reports overflow
                                | reading.                        | without crashing or memory
                                |                                 | corruption.
   8. Stuck-Key Diagnostic      | Hold key active past fault      | Driver flags stuck-key
                                | limit (> 30 s).                 | error/event.


  A state machine, managing each key's lifecycle, defines distinct states (e.g., RELEASED, PRESSED,...
  Here is an end-to-end architectural plan for a robust, production-grade Button / Keypad Driver.

  This plan is structured around strict embedded firmware standards (decoupled hardware,
  deterministic timing, and high-reliability design suitable for medical/safety-critical devices).
  ──────
    +-----------------------------------------------------------------------+
    |                         APPLICATION / GUI LAYER                       |
    |        (e.g., UI screen navigation, parameter adjustment, power)      |
    +-----------------------------------------------------------------------+
                                       ▲
                                       │ Reads high-level events
                                       │ (SHORT_PRESS, LONG_HOLD, REPEAT, COMBO)
    +-----------------------------------------------------------------------+
    |                         KEYPAD DRIVER LAYER                           |
    |                                                                       |
    |  +--------------------+   +-------------------+   +----------------+  |
    |  | Event FIFO Buffer  |   | Multi-Key / Combo |   | Stuck-Key &    |  |
    |  |   (Ring Buffer)    |   | & Ghost Detection |   | Diagnostics    |  |
    |  +--------------------+   +-------------------+   +----------------+  |
    |            ▲                       ▲                      ▲           |
    |            +-----------------------+----------------------+           |
    |                                    │                                  |
    |                    +-------------------------------+                  |
    |                    |   Per-Key State Machine       |                  |
    |                    | (IDLE -> DEBOUNCE -> PRESSED) |                  |
    |                    +-------------------------------+                  |
    |                                    ▲                                  |
    |                    +-------------------------------+                  |
    |                    |    Debounce Filter Engine     |                  |
    |                    |  (Integrator / Shift Window)  |                  |
    |                    +-------------------------------+                  |
    |                                    ▲                                  |
    |                    +-------------------------------+                  |
    |                    |     Matrix / Pin Scanner      |                  |
    |                    +-------------------------------+                  |
    +-----------------------------------------------------------------------+
                                       ▲
                                       │ Hardware Abstraction (Function Pointers)
    +-----------------------------------------------------------------------+
    |                       HARDWARE / PLATFORM LAYER                       |
    |           (STM32 HAL / Register GPIO / Simulated RAM for Tests)       |
    +-----------------------------------------------------------------------+
  ──────
  ### Module 1: Hardware Abstraction Layer (HAL Decoupling)

  The driver itself must not include MCU-specific headers (like stm32xxxx_hal.h). Instead, hardware
  access is abstracted through a lightweight interface:

  • Pin Read / Write Callbacks:
      • Function pointer to read an input pin / column pin.
      • Function pointer to set an output pin / row drive pin (for matrix keypads).
  • Polarity Abstraction:
      • Ability to configure keys as Active-Low (internal pull-up, switch to GND) or Active-High
      (internal pull-down, switch to VDD).
  • Portability Benefit:
      • In production: maps directly to HAL_GPIO_ReadPin().
      • In unit tests: maps to a simple array in RAM (mock_pins[]) to test without any hardware.

  ──────
  ### Module 2: Hardware Topology & Scanning Engine

  Depending on the physical keypad design:

  1. Direct GPIO Mode (1 Pin per Button):
      • Direct simultaneous read of all button lines into a raw bitmask.
  2. Matrix Multiplexing Mode (Rows × Columns):
      • Sequentially asserts one Row line (e.g., drives Low).
      • Brief settling delay to allow trace capacitance to discharge.
      • Reads all Column lines.
      • Restores the Row line to inactive state (High-Z or High).
  3. Settling Time Handling:
      • Configurable settling time or non-blocking scan steps for high-capacitance membrane cables
      (FFC cables).

  ──────
  ### Module 3: Signal Conditioning & Debounce Engine

  Mechanical switches and membrane buttons produce electrical contact bounce (noise spikes) lasting
  5 to 25 ms.

  • Filtering Strategies:
      • Integrator / Counter Filter: A state is only validated when sampled continuously identical
      for N milliseconds.
      • Shift-Register Bitmask Filter: Pushes samples into a 16-bit register; state changes only
      when bitmask is all 1s (0xFFFF) or all 0s (0x0000).
  • Noise Rejection:
      • Transient spikes shorter than the debounce window are completely discarded without false-
      triggering the application.

  ──────
  ### Module 4: Key State Machine (Per-Key Lifecycle)

  Every key maintains an independent finite state machine (FSM):

    📊 Diagram (unsupported type)
    ──────────────────────────────
    stateDiagram-v2
        [*] --> IDLE : Power Up / Reset
        IDLE --> DEBOUNCE_PRESS : Pin Active Detected
        DEBOUNCE_PRESS --> IDLE : Noise (Glitch < Debounce Time)
        DEBOUNCE_PRESS --> PRESSED : Stable for >= Debounce Time
        PRESSED --> HELD : Kept Pressed >= Long-Press Threshold
        HELD --> REPEATING : Kept Pressed >= Repeat Delay
        REPEATING --> REPEATING : Every Repeat Interval
        PRESSED --> DEBOUNCE_RELEASE : Pin Inactive Detected
        HELD --> DEBOUNCE_RELEASE : Pin Inactive Detected
        REPEATING --> DEBOUNCE_RELEASE : Pin Inactive Detected
        DEBOUNCE_RELEASE --> IDLE : Stable Inactive (Generates SHORT_PRESS if released before HELD)
  ──────
  ### Module 5: Event Classification & Generation

  The driver translates raw contact states into rich, actionable events:

  1. Raw Edge Events:
      • KEY_EVENT_DOWN: Fired immediately when contact is debounced.
      • KEY_EVENT_UP: Fired when contact is released.
  2. Higher-Level Semantic Events:
      • KEY_EVENT_SHORT_PRESS (Click): Validated only after release if the hold time was shorter
      than the long-press threshold.
      • KEY_EVENT_LONG_PRESS_START: Fired the exact moment the button exceeds the long-press
      threshold (e.g., 1000 ms).
      • KEY_EVENT_REPEAT: Fired periodically while held (e.g., every 200 ms) for rapid value
      scrolling or volume adjustment.
      • KEY_EVENT_DOUBLE_CLICK (Optional): Two quick clicks within a configurable interval (e.g., <
      300 ms).
      • KEY_EVENT_COMBO: Specific combination of two keys held at once (e.g., ENTER + CANCEL for
      factory reset).

  ──────
  ### Module 6: Event FIFO & Queue Management

  • Ring Buffer:
      • Circular FIFO storing pending events (key_id, event_type, timestamp).
      • Enables asynchronous processing: the application/GUI task can pull events when ready without
      missing rapid presses.
  • Overflow Handling:
      • Configurable queue size (e.g., 8–16 events).
      • Overflow policy: Drop newest or report an OVERFLOW warning status to prevent memory
      corruption.

  ──────
  ### Module 7: Safety, Diagnostics & Fault Detection

  Particularly critical for reliable and medical-grade firmware:

  1. Stuck-Key Fault Detection:
      • If any key remains continuously active beyond a timeout (e.g., > 30 seconds), flag a fault
      (indicates physical membrane defect, fluid ingress, or short circuit).
  2. Ghost Key Mitigation (For Matrix Keypads):
      • When 3 keys sharing rows and columns are pressed at once in a diode-less matrix, a 4th
      "phantom" key appears.
      • The driver detects multi-contact ambiguities and suppresses false key readings.
  3. Defensive Parameter Validation:
      • Validates configuration parameters at initialization (rejects NULL callbacks, zero debounce
      time, invalid key IDs).

  ──────
  ### Module 8: Timing & Integration Architecture

  • Non-Blocking Periodic Scan:
      • rcm_keypad_scan(handle, delta_ms) called from a periodic timer (e.g., 5 ms or 10 ms) or RTOS
      task.
  • Deterministic Timebase:
      • Time increments are passed explicitly (delta_ms), making the driver completely independent
      of system clocks or hardware timers (and 100% testable in software).

  ──────
  ### Summary Checklist of Driver Responsibilities

   Feature                      |        In Scope        | Description
  ------------------------------|------------------------|------------------------------------------
   GPIO Pin Configuration       |    ❌ Excluded from    | Handled by BSP / board initialization
                                |         Driver         |
   GPIO Read/Write Abstraction  |      ✅ Included       | Function pointer ops table
   Matrix / Direct Scan Logic   |      ✅ Included       | Active driving & line sampling
   Noise & Debounce Filter      |      ✅ Included       | Time-based stable state verification
   Individual Key FSM           |      ✅ Included       | Idle, Pressing, Held, Repeating,
                                |                        | Releasing
   Short / Long / Repeat Events |      ✅ Included       | Time-threshold event decoding
   Combo / Simultaneous Keys    |      ✅ Included       | Multi-key bitmask tracking
   Event Ring Buffer            |      ✅ Included       | Thread-safe FIFO for GUI/App
   Stuck-Key & Diagnostics      |      ✅ Included       | Timeout fault detection
  ──────