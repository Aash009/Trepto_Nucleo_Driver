# STM32U575ZIT6Q project target

The CubeMX project targets `STM32U575ZITxQ` (`STM32U575ZIT6Q`, LQFP144). The build selects the `STM32U575xx` CMSIS device definition, U575 startup file, and U575 linker scripts. The linker memory layout is 2048 KiB Flash, 768 KiB main RAM, and 16 KiB SRAM4.

## ADC channel 9

ADC1 remains enabled in the CubeMX configuration. PA4 is configured as `ADC1_IN9`, and the ADC driver selects `ADC_CHANNEL_9`. Keep both the `.ioc` pin configuration and the ADC driver channel selection aligned when regenerating or changing the board wiring.

## Port and bring-up process

1. In STM32CubeMX, select the exact device `STM32U575ZIT6Q` and confirm package and supply configuration against the board.
2. Check each pin and alternate function against the board schematic, including PA4/ADC1_IN9, LPUART1, keys, oscillator pins, USB, and debug connections.
3. Match clock and power settings to the board hardware, including oscillators, SMPS/LDO selection, USB clock, and voltage scaling.
4. Regenerate with a matching STM32CubeU5 package. Review changes to MSP setup, interrupts, RTOS memory, USBX, and low-power autonomous mode.
5. Build and inspect the linker map for memory use and startup/vector selection.
6. Bring up on hardware, then verify ADC acquisition, UART, key interrupts, USB, RTOS scheduling, sensor paths, reset/fault behavior, and any low-power wake sources in use.
7. Review the current U575 errata and board electrical limits before product use.

The project configuration is changed for U575. Board wiring and hardware operation still require confirmation on the destination board.
