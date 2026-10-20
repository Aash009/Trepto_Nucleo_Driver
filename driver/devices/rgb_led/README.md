# rgb_led

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-04 Alarm manager (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: LP5810 RGB LED driver: colour and blink patterns for status and alarms.
Hardware: LP5810BYBHR + 150044M155260; I2C (HAD Table 8)
Allocation source: FAD Table 19 (RGB -> alarm manager)
Legacy (L496 project): Blink_driver, rewrite (new driver IC). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
