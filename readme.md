# Universal Modbus-TCP esphome

port is an Option / standard 502


# Modbus_TCP (nearly same as original modbus (rtu)  

```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s
```
# for esp8266
```yaml
esp8266:
  board: YOURBOARDVARIANT
```
# for esp32 / ONLY Framework ARDUINO
```yaml
esp32:
  board: esp32dev / YOURBOARDVARIANT
  framework:
    type: arduino
```
# for modbus TCP
```yaml
modbus:
  - id: modbustesttcp
    host: 192.168.178.46
    port: 502

in modbus_controller:  address = UNIT ID
```






# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
