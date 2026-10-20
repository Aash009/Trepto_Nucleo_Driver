# display

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-06 GUI (Class B per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: TFT panel driver: SPI command/data, reset, backlight PWM; TouchGFX target interface.
Hardware: TFT 240x320; SPI + PWM backlight
Allocation source: FAD Table 19 (display -> GUI)
Legacy (L496 project): OLED_driver, rewrite (different panel). Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: Panel identity differs across HAD (E18RA1-FW450-N), schematic (Densitron 1.69" custom), FAD (ILI9342C controller) and DMT018QHNXNT0-1A (OPEN-022).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
