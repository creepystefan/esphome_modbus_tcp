# Universal Modbus-TCP esphome

port is an Option / standard 502

# EXTERNAL COMPONENT required
```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s
```

# Modbus_TCP (nearly same as original modbus (rtu)  
# Please Test
```yaml

modbustcp:
  - id: modbustesttcp
    host: 192.168.178.46
    port: 502
         
modbustcp_controller:
  - id: modbus_device
    modbustcp_id: modbustesttcp
    address: 1                   # Unit-ID


sensor:
  - platform: modbustcp_controller
    modbustcp_controller_id: modbus_device
    name: A-Voltage
    address: 1021
    value_type: FP32  
    register_type: read
    accuracy_decimals: 2


```


# Modbus TCP uint16 16bit  ( 2 Byte )
```yaml
sensor:
  - platform: modbus_tcp_16
    name: NAME
    host: 192.168.178.46
    port: 502             # Optional, default 502
    functioncode: 4       # Optional, default 4
    register_address: 1
```

# Modbus TCP uint32 32bit  ( 4 Byte )
```yaml
sensor:
  - platform: modbus_tcp_32
    name: NAME
    host: 192.168.178.46
    unit_id: 1            # Optional 1-255, default 1
    port: 502             # Optional, default 502
    functioncode: 4       # Optional, 1-4, default 4
    register_address: 1
```

# Modbus TCP  FLOAT  ( 4 Byte )
```yaml
sensor:
  - platform: modbus_tcp_float
    name: NAME
    host: 192.168.178.46
    unit_id: 1            # Optional 1-255, default 1
    port: 502             # Optional, default 502
    functioncode: 4       # Optional, 1-4, default 4
    register_address: 1
```
   
 
# binary Sensor uint8  ( 1 Byte ) / 1 bit = 1 or 0
```yaml

binary_sensor:
  - platform: modbus_tcp_binary  
    name: NAME
    host: 192.168.178.46
    port: 502             # Optional, default 502
    functioncode: 1       # Optional, default 1
    register_address: 1
```





# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
