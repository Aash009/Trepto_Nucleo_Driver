# temp_sensor

Status: implemented prototype with host tests; not verified for product use.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Board temperature readout (implemented prototype; generic one-byte format).
Hardware: HAD Table 8: STS40-CD1B-R3. Schematic sheet 5: TMP1075DSGR at I2C 0x48
Allocation source: HAD Table 8
Legacy (L496 project): Temp_driver. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Part mismatch between HAD and schematic (OPEN-024). Prototype format is not the selected part's format.
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
