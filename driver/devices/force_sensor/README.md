# Force sensor

Reads the AFE output through ADC1 channel 9 and reports raw counts and
millivolts. Millivolts use the planned 1.8 V ADC full scale. Force conversion
is intentionally not implemented until its calibration equation is defined.

Before use, configure ADC1 for 12-bit single-ended conversion and PA4 as
analog input in CubeMX. The application passes the generated `hadc1` handle to
`sensor_manager_init()`. The sensor driver configures channel 9 and performs
offset calibration. Do not access the ADC from another owner while sampling.

The ThreadX manager samples every 10 kernel ticks and publishes the latest
sample plus 100-sample window statistics. Confirm the ThreadX tick rate when
selecting the final sample interval.

Hardware references and force-sensor variant/gain questions remain documented
in `reference/README.md`.
