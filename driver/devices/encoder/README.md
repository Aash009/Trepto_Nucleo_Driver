# encoder

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: MA782 magnetic angle encoder over SPI; position for the SDC position loop.
Hardware: MA782GGU-Z; SPI (HAD Table 8)
Allocation source: FAD Table 19
Legacy (L496 project): Motor_encoder_driver, rewrite for the new part. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
