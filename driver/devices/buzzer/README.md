# buzzer

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-04 Alarm manager (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Speaker/buzzer tone generation on the alarm rail.
Hardware: SMT-0540-T-10-R / SMT-0540-S-R via Class-D amplifier on VSYS_ALM (HAD Table 7, 8)
Allocation source: FAD Table 19 (speaker -> alarm manager)
Legacy (L496 project): Alarm_Tone_Generator (Alarm_driver), port. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
