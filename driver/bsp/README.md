# Board bindings

rev1_0/ maps logical devices to pins and peripherals for Treptoflo Main PCB Rev 1.0
(schematic dated 15-09-2026). A new PCB revision gets a new sibling folder; the
active revision is selected at build time.

CubeMX (.ioc, Core/) owns pin, clock, MSP and peripheral initialisation. Bind those
handles here; never repeat initialisation. reference/ holds the pin table extract
cited by revision.
