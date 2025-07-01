from __future__ import annotations

from typing import Literal

from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ADDRESS, CONF_ID, CONF_PORT, CONF_IP_ADDRESS

modbustcp_ns = cg.esphome_ns.namespace("modbustcp")
ModbusTCP = modbustcp_ns.class_("ModbusTCP", cg.Component)
ModbusDevice = modbustcp_ns.class_("ModbusTCPDevice")
MULTI_CONF = True

CONF_MODBUSTCP_ID = "modbustcp_id"
CONF_UNIT_ID = 'unit_id'

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ModbusTCP),
            cv.Required(CONF_IP_ADDRESS): cv.ipv4address, 
            cv.Optional(CONF_PORT, default=502): cv.port,
            cv.Optional(CONF_UNIT_ID, default=1): cv.int_range(min=1, max=255), 
            
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    )


async def to_code(config):
    cg.add_global(modbustcp_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_host_ip_address(str(config[CONF_IP_ADDRESS])))
    cg.add(var.set_host_port(config[CONF_PORT]))
    cg.add(var.set_unit_id(config[CONF_UNIT_ID]))


async def register_modbustcp_device(var, config):
    parent = await cg.get_variable(config[CONF_MODBUSTCP_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(var.set_functioncode(config[CONF_FUNCTIONCODE]))
    cg.add(parent.register_device(var))
