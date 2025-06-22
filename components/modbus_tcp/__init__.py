import esphome.codegen as cg
import esphome.config_validation as cv
# from esphome.components import sensor, output
# from esphome import automation
# from esphome.automation import maybe_simple_id

CODEOWNERS = ["@sebydocky","@creepystefan"]
MULTI_CONF = True

modbus_tcp_ns = cg.esphome_ns.namespace("modbus_tcp")
ModbusTCP = modbus_tcp_ns.class_("ModbusTCP", cg.PollingComponent)

from esphome.const import (
    CONF_ID,
)

CONF_MODBUSTCP_ID = "modbustcp_id"
CONF_HOST_IP_ADDRESS = 'host_ip_address'
CONF_HOST_PORT = 'host_port'
CONF_UNIT_ID = 'unit_id'
#CONF_IP_ADDRESS = 'host'

ModbusTCP_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MODBUSTCP_ID): cv.use_id(ModbusTCP),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
          cv.GenerateID(): cv.declare_id(ModbusTCP),
          cv.Required(CONF_HOST_IP_ADDRESS): cv.ipv4address, 
        # cv.Required(CONF_HOST_IP_ADDRESS): cv.templatable(cv.string),
          cv.Optional(CONF_HOST_PORT, default=502): cv.int_range(min=1, max=65535),
          cv.Optional(CONF_UNIT_ID, default=1): cv.int_range(min=1, max=255),          
        }
    ).extend(cv.polling_component_schema("10s"))
 )
 
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    #cg.add(var.set_host_ip_address(config[CONF_HOST_IP_ADDRESS]))
    cg.add(var.set_host_ip_address(str(config[CONF_HOST_IP_ADDRESS])))
    if CONF_HOST_PORT in config:
        cg.add(var.set_host_port(config[CONF_HOST_PORT]))
        
    if CONF_UNIT_ID in config:
        cg.add(var.set_unit_id(config[CONF_UNIT_ID]))
	
