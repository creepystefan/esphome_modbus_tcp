# Universal Modbus-TCP esphome
# Modbus_TCP (nearly same as original modbus (rtu)  

# Framework IDF or Arduino

# for modbus TCP
```yaml
external_components:
  - source: github://creepystefan/esphome_tcp
    refresh: 0s
esphome:
  min_version: 2026.2.4

uart:   # from original esphome
  - id: uartid
    tx_pin: GPIO for TX
    rx_pin: GPIO for RX
    baud_rate: 9600

modbus:
  - id: modbustest_tcp      # you can change to your id
    type: TCP               # TCP for TCP connection
    host: 192.168.178.46    # Required
    port: 502               # Optional 502 is default
    send_wait_time: 250ms   # Optional 250ms is default

  - id: modbustest_rtu      # you can change to your id
    uart_id:          ### ID to uart ID
    type: RTU               # RTU for Uart RTU connection
    send_wait_time: 250ms   # Optional 250ms is default

modbus_controller:
  - id: modbus_device_tcp
    modbus_id: modbustest_tcp       # id to modbus_id example to TCP "modbustest_tcp"
    address: 10            # Unit-ID
    update_interval: 10s   #default 60sec

  - id: modbus_device_rtu
    modbus_id: modbustest_rtu
    address: 10            # Unit-ID
    update_interval: 10s   #default 60sec

sensor:
  - platform: modbus_controller
    modbus_controller_id: modbus_device_tcp   # for TCP
    name: YEAR
    address: 31
    value_type: U_WORD
    register_type: read
    
  - platform: modbus_controller
    modbus_controller_id: modbus_device_rtu  # to ID for RTU
    name: Month
    address: 32
    value_type: U_WORD
    register_type: read

   
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
