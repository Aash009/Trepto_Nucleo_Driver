# Driver layer

devices/      one folder per external part on the Treptoflo Main PCB Rev 1.0
peripherals/  project contracts for MCU peripherals (gpio, spi, i2c, uart, adc, timer_pwm, octospi, usb, rtc)
platform/     STM32U5 HAL/LL bindings to CubeMX-initialised handles
bsp/rev1_0/   logical-device to pin/peripheral map for PCB Rev 1.0

Drivers never include system/ or application/ headers. Most drivers belong to
SI-P-05 Platform services; drivers whose interface FAD Table 19 allocates to
another item are mapped to that item in software_items.yaml (for example
devices/motor -> SI-P-01, devices/display -> SI-P-06).
The vendor HAL lives in root Drivers/ (CubeMX output, SOUP), not here.
