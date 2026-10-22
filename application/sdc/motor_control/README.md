# motor_control

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Motor control law and position loop driving the DRV8214 through driver/devices/motor.
Allocation source: FAD §11.3
Legacy (L496 project): Motor_single_rotation_algorithm_system (SMA actuator), rewrite for DC motor. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Control law selection (FAD open item 4); replaces the former application/pid placeholder.
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
