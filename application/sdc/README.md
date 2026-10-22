# Safe Delivery Core (SI-P-01, Class C)

Units: state_machine/, dose_pipeline/, motor_control/, metrology/, occlusion/,
validation_gate/, fault_handler/. Class B items reach the SDC only through api/.
The SDC also owns the motor, encoder, force-sensor and external-watchdog drivers
under driver/devices/ (FAD Table 19), mapped to this item in software_items.yaml.
Source: FAD Table 8 row 1, FAD §9–11.
