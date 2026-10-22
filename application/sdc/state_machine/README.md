# state_machine

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Delivery state machine: state set, fault classes that force SAFE, VBUS and SERVICE interaction, clearing SAFE. Includes priming states pending FAD allocation.
Allocation source: FAD §10.1–10.5
Legacy (L496 project): Insulon_delivery_system and Priming_fixed/manual/rewind_system, rewrite. Clause 4.4 legacy assessment or re-development is required before any L496 code is copied (docs/LEGACY_PORTING_MAP.md).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
