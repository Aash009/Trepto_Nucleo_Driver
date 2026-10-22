# fault_handler

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Classify detected faults and command the transition to SAFE.
Allocation source: FAD §10.3
Open: Allocation to SDC is proposed; FAD Table 8 has no separate fault-manager item (OPEN-019).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
