# insulin

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-02 Therapy Policy (Class C per software_items.yaml).
Build variants: INSUFLO. Excluded from the Treptoflo build; CI map check confirms zero symbols.
Responsibility: Insulin basal and bolus policy and per-drug configuration.
Allocation source: FAD §5.4
Legacy (L496 project): Basal_application, Bolus_application, Basal_bolus_application, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
