# nor_flash

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: W25Q256 NOR flash: GUI assets (XiP) and bulk logs; never the only copy of SDC data.
Hardware: W25Q256JWBIQ; OCTOSPI/QSPI (HAD Table 8)
Allocation source: FAD §13.1, §13.4
Legacy (L496 project): FlashDriver (Flash_driver) and QUADSPI_FLASH, merge into one driver. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
