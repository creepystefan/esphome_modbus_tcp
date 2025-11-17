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
value_type (Optional): datatype of the mod_bus register data. The default data type for ModBUS is a 16 bit integer in big endian format (network byte order, MSB first)

U_WORD : unsigned 16 bit integer, 1 register, uint16_t
S_WORD : signed 16 bit integer, 1 register, int16_t
U_DWORD : unsigned 32 bit integer, 2 registers, uint32_t
S_DWORD : signed 32 bit integer, 2 registers, int32_t
U_DWORD_R : little endian unsigned 32 bit integer, 2 registers, uint32_t
S_DWORD_R : little endian signed 32 bit integer, 2 registers, int32_t
U_QWORD : unsigned 64 bit integer, 4 registers, uint64_t
S_QWORD : signed 64 bit integer, 4 registers int64_t
U_QWORD_R : little endian unsigned 64 bit integer, 4 registers, uint64_t
S_QWORD_R : little endian signed 64 bit integer, 4 registers, int64_t
FP32 : 32 bit IEEE 754 floating point, 2 registers, float
FP32_R : little endian 32 bit IEEE 754 floating point, 2 registers, float
Defaults to U_WORD.






# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
