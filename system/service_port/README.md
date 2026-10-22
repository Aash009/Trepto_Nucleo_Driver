# service_port

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: USB service-port command interface (framed protocol) used in SERVICE mode; host-side CLI goes in tools/.
Allocation source: FAD Table 19 (USB-C), FAD §10.4
Legacy (L496 project): Serial_command_driver, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
