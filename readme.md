# Universal Modbus-TCP esphome

port is an Option / standard 502


# Modbus_TCP (nearly same as original modbus (rtu)  

```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s

modbustcp:
  - id: modbustesttcp
    host: 192.168.178.46
    port: 502
         
modbustcp_controller:
  - id: modbus_device
    modbustcp_id: modbustesttcp
    address: 1                   # Unit-ID
    update_interval: 5s   #default 60sec

sensor:
  - platform: modbustcp_controller
    modbustcp_controller_id: modbus_device
    name: A-Voltage
    address: 1021
    value_type: FP32  
    register_type: read
    accuracy_decimals: 2

binary_sensor:
  - platform: modbustcp_controller
    modbustcp_controller_id: modbus_device
    name: "Error status"
    register_type: read
    address: 0x3200

switch:
  - platform: modbustcp_controller
    modbustcp_controller_id: modbus_device 
    id: testswitch
    register_type: coil
    address: 2
    name: "testswitch"
    bitmask: 1



```






# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
