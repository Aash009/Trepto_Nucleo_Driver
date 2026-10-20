# power_rails

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Rail enable outputs and feedback inputs per HAD control authority; holds the radio module powered off and in reset in the Treptoflo build.
Hardware: 3V3 ALARM and SENS/DISP enables (OR with SAFETY), display VCI load switch (MAIN owns), MOTOR rail read-only feedback (SAFETY owns), radio load switch and reset (HAD Table 8 control authority)
Allocation source: FAD Table 19 (rail enables), SAD Table 5 (connectivity power-gating)
Legacy (L496 project): Power_control_driver, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
