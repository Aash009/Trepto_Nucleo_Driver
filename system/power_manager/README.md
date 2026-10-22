# power_manager

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Battery state of charge, depletion alarms, rail sequencing and power-mode transitions.
Allocation source: FAD Table 19 (battery gauge, rail enables)
Legacy (L496 project): SMA_switching: drop (no SMA actuator). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
