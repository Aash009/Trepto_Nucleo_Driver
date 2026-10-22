# connectivity

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-07 Connectivity service (Class B per software_items.yaml).
Build variants: INSUFLO. Excluded from the Treptoflo build; CI map check confirms zero symbols.
Responsibility: BLE command client and ACE schema: commands are proposals only, submitted through application/sdc/api.
Allocation source: FAD §15, FAD Table 8 row 7
Legacy (L496 project): BLE_Comm / BLE_security_application, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
