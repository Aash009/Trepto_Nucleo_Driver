# humidity_sensor

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: SHT40 humidity and temperature readout.
Hardware: SHT40-AD1B-R3; I2C address 0x44 (HAD Table 8, schematic sheet 5)
Allocation source: HAD Table 8
Legacy (L496 project): Humidity_temp_driver, port. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
