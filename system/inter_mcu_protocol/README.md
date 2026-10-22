# inter_mcu_protocol

Status: design placeholder, excluded from compilation and verification counts.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: Frame layouts, CRC, sequence and heartbeat rules for MAIN<->SAFETY (SPI, 100 ms heartbeat) and MAIN<->RADIO (UART). Single source of truth: the SAFETY project consumes it at a pinned tag.
Allocation source: FAD Table 19 (inter-MCU link, radio)
Open: Export mechanism to the SAFETY project (pinned copy with checksum, or its own repository) is decided in the SCMP (OPEN-021).
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
