# alarm_manager

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-04 Alarm manager (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Catalogue-driven alarm annunciation, priorities, latching, inactivation and escalation; drives speaker, haptic and RGB LED.
Allocation source: FAD §12, FAD Table 19 (speaker / LRA / RGB)
Legacy (L496 project): Alarm_system, port or rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Alarm catalogue population from the hazard analysis (FAD open item 2).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
