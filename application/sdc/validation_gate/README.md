# validation_gate

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-01 Safe Delivery Core (SDC) (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Validate every dose request from Therapy Policy, GUI and Connectivity before actuation; the only path to motor authorisation. MC/DC coverage required.
Allocation source: FAD §6.1.4, FAD Table 23
Note: Replaces the former application/delivery_safety placeholder.
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
