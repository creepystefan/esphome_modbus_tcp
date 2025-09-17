import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv

CODEOWNERS = ["@creepystefan"]

CONF_FUNCTIONCODE =  'functioncode'
CONF_REGISTER_ADDRESS =  'register_address'
#CONF_UPDATE_INTERVAL =  'update_interval'
CONF_IP_ADDRESS = 'host'
CONF_PORT = 'port'
CONF_UNIT_ID = 'unit_id'


modbus_tcp_float_ns = cg.esphome_ns.namespace("modbus_tcp_float")
ModbusTCPfloat = modbus_tcp_float_ns.class_("ModbusTCPfloat", sensor.Sensor, cg.PollingComponent)


CONFIG_SCHEMA = cv.Schema(
    sensor.sensor_schema(ModbusTCPfloat)
    .extend(
        {
        cv.GenerateID(): cv.declare_id(ModbusTCPfloat),
        cv.Required(CONF_IP_ADDRESS): cv.ipv4address,
        cv.Optional(CONF_PORT, default=502): cv.int_range(0, 65535),
        cv.Optional(CONF_FUNCTIONCODE, default=4): cv.int_range(min=1, max=4),
        cv.Optional(CONF_UNIT_ID, default=1): cv.int_range(min=0, max=255),
        cv.Required(CONF_REGISTER_ADDRESS): cv.int_,
     
        }
       ).extend(cv.COMPONENT_SCHEMA)
       .extend(cv.polling_component_schema("1s")))
       


async def to_code(config):
    #var = cg.new_Pvariable(config[CONF_ID])
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    cg.add(var.set_host(str(config[CONF_IP_ADDRESS])))
    cg.add(var.set_functioncode(config[CONF_FUNCTIONCODE]))
    cg.add(var.set_unit_id(config[CONF_UNIT_ID]))
    cg.add(var.set_register_address(config[CONF_REGISTER_ADDRESS]))
    cg.add(var.set_port(config["port"]))
   

