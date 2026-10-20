# battery_gauge

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Coulomb counter readout, alert GPIO handling and chemistry-ID input.
Hardware: LTC2959IDDB (FAD Table 19, schematic sheet 5); I2C + ADC; alert GPIO also routed to SAFETY
Allocation source: FAD Table 19
Legacy (L496 project): BatteryGauge (battery_gauge_driver), rewrite (different gauge IC). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: FAD Table 19 assigns the power manager Class B; FAD Table 8 puts the power manager in Platform (Class C). Held at C until the FAD is reconciled (OPEN-023).
Open: Second-source gas-gauge architecture mismatch (component qualification).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
