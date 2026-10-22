# journal

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Append-only delivery journal and audit log with two-phase commit to FRAM; the only writer of journal records. Replaces the former system/data_logger placeholder.
Allocation source: FAD §13.1–13.2; FAD Table 8 note: journal writes sit in Platform, not Logging
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
