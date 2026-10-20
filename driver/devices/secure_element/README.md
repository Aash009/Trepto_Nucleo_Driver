# secure_element

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-03 Update client and bootloader (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: STSAFE-A110 secure element access for boot and update key operations.
Hardware: STSAFE-A110; I2C (HAD Table 8)
Allocation source: FAD Table 19 (secure element -> boot/update)
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
