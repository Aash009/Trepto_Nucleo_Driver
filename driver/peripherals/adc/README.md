# adc

ADC1 channel 9 adapter for force sensor acquisition. ADC1 and PA4 analog mode
must be configured by CubeMX; this module selects the regular channel,
calibrates ADC1 and performs finite-time single conversions.
Software item: SI-P-05 Platform services (Class C per software_items.yaml).
Build variants: TREPTOFLO, INSUFLO.
Responsibility: ADC contract: force AFE channel, battery voltage, chemistry-ID sense.
Allocation source: FAD Table 19 (force sensor, battery gauge)
inc/: public API; src/: implementation; test/: unit verification.
Requirements (SR-P-###), unit design (SU-P-###), timing and fault behaviour: TBD.
