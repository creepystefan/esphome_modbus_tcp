import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID


DEPENDENCIES = ["uart"]

#MULTI_CONF = True

realdash_ns = cg.esphome_ns.namespace("realdash")
Realdash = realdash_ns.class_("Realdash", cg.Component, uart.UARTDevice)
RealdashDevice = realdash_ns.class_("RealdashDevice")



CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Realdash),
        
    }
).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
   var = cg.new_Pvariable(config[CONF_ID])
   await cg.register_component(var, config)
   await uart.register_uart_device(var, config)

async def register_realdash_device(var, config):
    arent = await cg.get_variable(config[CONF_REALDASH_ID])
    cg.add(var.set_parent(parent))
