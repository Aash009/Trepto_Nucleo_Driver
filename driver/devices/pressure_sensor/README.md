# pressure_sensor

Status: implemented prototype with host tests; not verified for product use.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Ambient pressure readout (implemented prototype; generic 0.5 kPa/count format).
Hardware: DPS368XTSA1 x2 (U6, U7); I2C addresses 0x76 and 0x77 (schematic sheet 5)
Allocation source: HAD Table 8
Legacy (L496 project): Pressure_temp_driver. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Prototype format is not the DPS368 register format; two instances need two device handles.
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
