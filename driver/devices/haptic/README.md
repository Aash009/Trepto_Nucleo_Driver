# haptic

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-04 Alarm manager (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: DRV2605L haptic driver for the LRA: waveform selection and playback.
Hardware: DRV2605LYZFR + HD-LA0503-LW28; I2C (HAD Table 8)
Allocation source: FAD Table 19 (LRA -> alarm manager)
Legacy (L496 project): Vibrator_driver, rewrite (new driver IC). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
