# ext_watchdog

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: TPS3850 window watchdog: WDI kick output and WDO status input; WDO is AND-gated to motor nSLEEP.
Hardware: TPS3850G18DRCR; GPIO WDI/WDO (HAD Table 8, HAD §6.3.1)
Allocation source: FAD Table 19
Legacy (L496 project): Watchdog_driver (L496 internal IWDG), rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
