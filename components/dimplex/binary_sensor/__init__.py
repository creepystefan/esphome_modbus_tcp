import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv

CODEOWNERS = ["@creepystefan"]

CONF_FUNCTIONCODE =  'functioncode'
CONF_REGISTER_ADDRESS =  'register_address'
#CONF_UPDATE_INTERVAL =  'update_interval'
CONF_IP_ADDRESS = 'host'
CONF_PORT = 'port'


dimplex_binary_sensor_ns = cg.esphome_ns.namespace("dimplex_binary_sensor")
DimplexTCP_Binary_Sensor = dimplex_binary_sensor_ns.class_("DimplexTCP_Binary_Sensor", binary_sensor.BinarySensor, cg.PollingComponent)


CONFIG_SCHEMA = cv.Schema(
    binary_sensor.BinarySensor_schema(DimplexTCP_Binary_Sensor)
    .extend(
        {
        cv.GenerateID(): cv.declare_id(DimplexTCP_Binary_Sensor),
        cv.Required(CONF_IP_ADDRESS): cv.ipv4address,
        cv.Optional(CONF_PORT, default=502): cv.int_range(0, 65535),
        cv.Optional(CONF_FUNCTIONCODE, default=4): cv.int_range(min=0, max=10),
        cv.Required(CONF_FUNCTIONCODE): cv.int_range(min=0, max=4),
        cv.Required(CONF_REGISTER_ADDRESS): cv.int_,
     #   cv.Optional(CONF_UPDATE_INTERVAL, default=1000): cv.int_range(min=500, max=10000),
        }
       ).extend(cv.COMPONENT_SCHEMA)
       .extend(cv.polling_component_schema("5s")))
       


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    cg.add(var.set_host(str(config[CONF_IP_ADDRESS])))
    cg.add(var.set_functioncode(config[CONF_FUNCTIONCODE]))
    cg.add(var.set_register_address(config[CONF_REGISTER_ADDRESS]))
    cg.add(var.set_port(config["port"]))
   # cg.add(var.update_interval(config[CONF_UPDATE_INTERVAL]))
