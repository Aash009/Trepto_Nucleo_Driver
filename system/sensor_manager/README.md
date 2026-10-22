# sensor_manager

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Sampling, freshness, calibration and plausibility for temperature, pressure and humidity.
Allocation source: FAD Table 8 row 5
Legacy (L496 project): Humidity_temp/Pressure_temp sampling logic, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
