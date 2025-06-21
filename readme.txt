https://ipc2u.de/artikel/wissenswertes/detaillierte-beschreibung-des-modbus-tcp-protokolls-mit-befehlsbeispielen/


Universal Modbus-TCP esphome

sensor:

  - platform: modbus_tcp_16     register data typ uint_16
  - platform: modbus_tcp_32     register data typ uint_32
  - platform: modbus_tcp_float  register data typ float  ( 4 byte )


* name:      Name of Sensor
* host: IP Adresse IPv4
* port: OPTIONAL, default: 502
* functioncode: function code  (allowed 1 - 4 ) 
              * 1 = Coil               (0x01)
              * 2 = Discrete input     (0x02)
              * 3 = Input register     (0x03)
              * 4 = Holding register   (0x04)
* register_address: Register Address from Host




binary_sensor:
  - platform: modbus_tcp_binary     register data typ uint_8
