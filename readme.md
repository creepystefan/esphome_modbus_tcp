# Universal Modbus-TCP esphome
# Modbus_TCP (nearly same as original modbus (rtu)  

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
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s
esphome:
  min_version: 2025.11.0

modbus:
  - id: modbustesttcp
    host: 192.168.178.46    # Required
    port: 502               # Optional 502 is default
    send_wait_time: 250ms   # Optional 250ms is default
```
all Components orignal from ESPHOME

in modbus_controller:  address = UNIT ID
platform: modbus_controller
sensor
number
switch
textsensor
....


# useful link
https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/
# https://github.com/Gucioo/esphome_modbus_tcp_master
