# common

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-02 Therapy Policy (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Drug-agnostic therapy sequencing and the per-drug configuration block interface. Dose requests leave this item only through the SDC validation gate.
Allocation source: FAD §5.4, §6.1.4
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
