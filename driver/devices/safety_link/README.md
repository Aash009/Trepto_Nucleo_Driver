# safety_link

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: SPI transport to the SAFETY MCU (U073); framing lives in system/inter_mcu_protocol.
Hardware: Inter-MCU SPI to STM32U073 (HAD §6.4)
Allocation source: FAD Table 19 (inter-MCU link)
Legacy (L496 project): Safety_SPI_driver, port. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
