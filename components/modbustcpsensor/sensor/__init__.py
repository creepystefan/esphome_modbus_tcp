import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbustcp

from esphome.const import (
    CONF_ID,
    CONF_ADDRESS
)

CODEOWNERS = ["@creepystefan"]
DEPENDENCIES = ['modbustcp']

modbustcpsensor_ns = cg.esphome_ns.namespace("modbustcpsensor")
ModbusTCPSensor = modbustcpsensor_ns.class_("ModbusTCPSensor", cg.PollingComponent, modbustcp.ModbusTCP, sensor.Sensor) 

CONF_FUNCTIONCODE = "functioncode"
#CONF_MODBUSTCPSENSOR_ID = "modbustcpsensor_id"

CONFIG_SCHEMA = cv.Schema(
    sensor.sensor_schema(ModbusTCPSensor)
    .extend(
        {
          cv.GenerateID(): cv.declare_id(ModbusTCPSensor),
          cv.Required(CONF_FUNCTIONCODE): cv.int_range(min=0, max=4),
          cv.Required(CONF_ADDRESS): cv.int_,        
        }
    )
    .extend(cv.polling_component_schema("1s")
    .extend(cv.COMPONENT_SCHEMA)
   
))
 

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    #var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    #await modbustcp.register_modbustcp_device(var, config)
    cg.add(var.set_functioncode(config[CONF_FUNCTIONCODE]))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    
   	
