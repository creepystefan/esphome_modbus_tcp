# Universal Modbus-TCP esphome

sensor:

  - platform: modbus_tcp_16     register data typ uint_16  ( 2 byte )
  - platform: modbus_tcp_32     register data typ uint_32  ( 4 byte )
  

* name:      Name of Sensor
* host: IP Adresse IPv4
* port: OPTIONAL, default: 502
* functioncode: function code  (allowed 1 - 4 ) 
              * 1 = Coil               (0x01)
              * 2 = Discrete input     (0x02)
              * 3 = Input register     (0x03)
              * 4 = Holding register   (0x04)
* register_address: Register Address from Host

# EXTERNAL COMPONENT required
```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s
```

# Modbus TCP  FLOAT  ( 4 Byte )
```yaml
sensor:
  - platform: modbus_tcp_float
    name: NAME
    host: 192.168.178.46
    port: 502
    functioncode: 4
    register_address: 1
```
   
        # only required
  - platform: modbus_tcp_float
    name: NAME
    host: 192.168.178.46
    functioncode: 4
    register_address: 1



#binary Sensor
```yaml

binary_sensor:
  - platform: modbus_tcp_binary     register data typ uint_8  ( 1 byte ) / 1 bit = 1 or 0
```

# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
