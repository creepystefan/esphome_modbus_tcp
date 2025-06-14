# Functioncode   COIL   0x01

external_components:
  - source: github://creepystefan/esphome_tcp


sensor:
  - platform: dimplex
    name: a-voltage
    host: 192.168.178.46
    register_address: 1021
    unit_of_measurement: "V"
    device_class: "voltage"
    state_class: "measurement"
    accuracy_decimals: 2
  - platform: dimplex
    name: b-voltage
    host: 192.168.178.46
    register_address: 1041
    unit_of_measurement: "V"
    device_class: "voltage"
    state_class: "measurement"
    accuracy_decimals: 2
