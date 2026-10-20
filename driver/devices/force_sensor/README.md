# force_sensor

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Force channel acquisition through the AFE on the MAIN ADC.
Hardware: HSFPAR304A (HAD Table 8) / HSFPAR004A (FAD Table 19) + INA333 AFE; ADC
Allocation source: FAD Table 19
Legacy (L496 project): Force_driver, port or rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Force sensor variant mismatch HSFPAR304A vs HSFPAR004A and INA333 gain annotation (EER findings).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
