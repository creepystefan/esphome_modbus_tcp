# ESPHome Modbus TCP
# Framework ESP32(IDF) or esp8266(Arduino)

```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s

modbustcp:
  - id: modbus_tcp_id     # you can change to your id
    host: 192.168.178.46    # Required
    port: 502               # Optional 502 is default
    send_wait_time: 250ms   # Optional 250ms is default

modbustcp_controller:
  - id: modbus_tcp_controller_id
    modbustcp_id: modbus_tcp_id     # ID to modbustcp_id 
    address: 10                     # Unit-ID
    update_interval: 5s             #default 60sec
  
sensor:
  - platform: modbustcp_controller
    modbustcp_controller_id: modbus_tcp_controller_id   # ID to modbustcp_controller_id 
    name: YEAR
    address: 31
    value_type: U_WORD
    register_type: read
  
   
```

# useful link
- [Public Server for Modbus TCP](https://modbus.pult.online/en.html)
- [Modbus TCP Protocol Description](https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/)
# https://github.com/Gucioo/esphome_modbus_tcp_master
