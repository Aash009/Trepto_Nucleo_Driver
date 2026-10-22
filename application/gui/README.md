# gui

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-06 GUI (Class B per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: TouchGFX application adapter: screens, presenters, input handling. Every therapy action round-trips through application/sdc/api.
Allocation source: FAD Table 8 row 6, FAD Table 19 (display, keypad)
Legacy (L496 project): GUI_system, rewrite for the new panel. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
Open: TouchGFX user code under TouchGFX/gui/ belongs to this item; add it to software_items.yaml when generated (OPEN-018).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
