import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_SENSORS,
)

#DEPENDENCIES = ['captive_portal']
DEPENDENCIES = ['realdash']

realdash_controller44_ns = cg.esphome_ns.namespace("realdash_controller44")
RealdashComponent = realdash_ns.class_("RealdashComponent", cg.PollingComponent, uart.UARTDevice)

#CONF_CANID = 'canid'


CONFIG_SENSORS_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.use_id(sensor.Sensor),
        #cv.Optional(CONF_NAME, default="test"): cv.string_strict,
        cv.Required(CONF_CANID): cv.int_range(min=0, max=0x1FFFFFFF),
    }
)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RealdashComponent),
        cv.Optional(CONF_SENSORS): cv.ensure_list(CONFIG_SENSORS_SCHEMA),
    }
)#.extend(uart.UART_DEVICE_SCHEMA)

#).extend(cv.polling_component_schema("10s")
#).extend(cv.COMPONENT_SCHEMA)
#).extend(uart.UART_DEVICE_SCHEMA)
#).extend(cv.polling_component_schema("10s")
#).extend(cv.COMPONENT_SCHEMA).extend(realdash.DEVICE_SCHEMA)



async def to_code(config):
   var = cg.new_Pvariable(config[CONF_ID])
   await cg.register_component(var, config)
   #await uart.register_uart_device(var, config)
   
   for sensor_cfg in config.get(CONF_SENSORS, []):
       s = await cg.get_variable(sensor_cfg[CONF_ID])
       #cg.add(var.register_sensor(sensor_cfg[CONF_NAME], s))
       cg.add(var.register_sensor(s))

       #cg.add(var.register_sensor(sensor_cfg[CONF_CANID], s))
       #var = cg.new_Pvariable(config[CONF_SENSORS])
       cg.add(var.set_canid(sensor_cfg[CONF_CANID]))
