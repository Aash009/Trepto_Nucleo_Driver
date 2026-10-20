# radio_link

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-07 Connectivity service (Class B per software_items.yaml).
Build variants: INSUFLO. Excluded from the Treptoflo build; CI map check confirms zero symbols.
Responsibility: UART transport to the STM32WB5MMG radio module.
Hardware: STM32WB5MMG module; UART with flow control (HAD Table 8)
Allocation source: FAD Table 19 (radio -> connectivity)
Legacy (L496 project): BLE_new (BLE_driver), rewrite; vendor stack runs on the module, not in this image. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
