# Porting to STM32U585VIT6Q

## What changed in this project

- CubeMX target is now `STM32U585VITxQ` (`STM32U585VIT6Q`, LQFP100).
- The build selects the `STM32U585xx` CMSIS device definition and the U585 startup file.
- The FLASH and RAM linker scripts were renamed for U585. Their regions remain 2048 KiB Flash, 768 KiB main RAM, and 16 KiB SRAM4, matching this target.
- The configured pins remain PE2–PE6 for keys and PC0/PC1 for LPUART1. ST's U585 LQFP100 pinout includes all seven pins, with PE2–PE6 on package pins 1–5 and PC0/PC1 on package pins 15–16.

## Porting process

1. **Select the exact ordering code in STM32CubeMX.** Choose `STM32U585VIT6Q`, not a family-level U5 part. Confirm LQFP100 and the board's power option (this project is configured for SMPS).
2. **Rebuild the pinout from the schematic.** Check every signal's package pin and alternate function, including key polarity, LPUART routing, oscillator pins, USB VDDUSB, and any debug connections. Package availability alone does not confirm board wiring.
3. **Review clock and power setup.** Match HSE/LSE crystals, supply topology, SMPS/LDO selection, USB 48 MHz clock, voltage scaling, and maximum clock settings to the actual board. Do not assume the current board's oscillator population is identical.
4. **Regenerate with a matching STM32CubeU5 package.** Preserve application-owned code and inspect regenerated files for changes to MSP setup, interrupt routing, RTOS memory pools, USBX, and low-power autonomous mode.
5. **Build and inspect the map.** Confirm no linker overflow, verify startup/vector symbols and the selected U585 CMSIS header, then record Flash/RAM use.
6. **Bring up on the target board.** Start with reset/debug and a GPIO heartbeat, then validate clock output/timing, UART, keys/EXTI, USB, RTOS scheduling, and sensor/peripheral paths. Test reset, watchdog/fault handling, and low-power wake sources used by the product.
7. **Close hardware-specific checks.** Review the current U585 errata, board schematic, electrical limits, and any safety-related fault response before using the port in a device.

## Scope and assumptions

This is a firmware project-target migration, not a board schematic review. The existing active pins are present on the target package, but their electrical connections, alternate-function routing, oscillator population, and external USB circuitry still need confirmation against the destination board. The U575 and U585 share the STM32U5 HAL/CMSIS family in this repository; the U585-specific compile definition selects its device header. No U585-only feature is enabled here.
