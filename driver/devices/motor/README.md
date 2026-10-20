# motor

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: DRV8214 H-bridge: PWM drive, nSLEEP control and I2C diagnostics (stall, ripple).
Hardware: DRV8214 dual H-bridge motor driver; PWM + I2C (HAD Table 8)
Allocation source: FAD Table 19
Legacy (L496 project): Motor_SMA_driver, rewrite (SMA actuator replaced by DC motor). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
