from __future__ import annotations

from typing import Literal

from esphome import pins
import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ADDRESS, CONF_DISABLE_CRC, CONF_ID
 #from esphome.const import CONF_ADDRESS, CONF_DISABLE_CRC, CONF_FLOW_CONTROL_PIN, CONF_ID
 #from esphome.cpp_helpers import gpio_pin_expression
import esphome.final_validate as fv

CONF_IP_ADDRESS = 'host'
CONF_PORT = 'port'

#DEPENDENCIES = ["uart"]

modbustcp_ns = cg.esphome_ns.namespace("modbustcp")
 #ModbusTCP = modbustcp_ns.class_("ModbusTCP", cg.Component, uart.UARTDevice)
ModbusTCP = modbustcp_ns.class_("ModbusTCP", cg.Component)
ModbusDevice = modbustcp_ns.class_("ModbusDevice")
MULTI_CONF = True

CONF_ROLE = "role"
CONF_MODBUSTCP_ID = "modbustcp_id"
CONF_SEND_WAIT_TIME = "send_wait_time"

ModbusRole = modbustcp_ns.enum("ModbusRole")
MODBUS_ROLES = {
    "client": ModbusRole.CLIENT,
    "server": ModbusRole.SERVER,
}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ModbusTCP),
            cv.Required(CONF_IP_ADDRESS): cv.ipv4address,
            cv.Optional(CONF_PORT, default=502): cv.int_range(0, 65535),
            cv.Optional(CONF_ROLE, default="client"): cv.enum(MODBUS_ROLES),
            #cv.Optional(CONF_FLOW_CONTROL_PIN): pins.gpio_output_pin_schema,
            cv.Optional(
                CONF_SEND_WAIT_TIME, default="250ms"
            ): cv.positive_time_period_milliseconds,
            cv.Optional(CONF_DISABLE_CRC, default=True): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    #.extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    cg.add_global(modbustcp_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_host(str(config[CONF_IP_ADDRESS])))
    cg.add(var.set_port(config["port"]))
 
   # await uart.register_uart_device(var, config)

    cg.add(var.set_role(config[CONF_ROLE]))
   # if CONF_FLOW_CONTROL_PIN in config:
   #     pin = await gpio_pin_expression(config[CONF_FLOW_CONTROL_PIN])
   #     cg.add(var.set_flow_control_pin(pin))

    cg.add(var.set_send_wait_time(config[CONF_SEND_WAIT_TIME]))
    cg.add(var.set_disable_crc(config[CONF_DISABLE_CRC]))


def modbus_device_schema(default_address):
    schema = {
        cv.GenerateID(CONF_MODBUSTCP_ID): cv.use_id(ModbusTCP),
    }
    if default_address is None:
        schema[cv.Required(CONF_ADDRESS)] = cv.hex_uint8_t
    else:
        schema[cv.Optional(CONF_ADDRESS, default=default_address)] = cv.hex_uint8_t
    return cv.Schema(schema)


def final_validate_modbus_device(
    name: str, *, role: Literal["server", "client"] | None = None
):
    def validate_role(value):
        assert role in MODBUS_ROLES
        if value != role:
            raise cv.Invalid(f"Component {name} requires role to be {role}")
        return value

    def validate_hub(hub_config):
        hub_schema = {}
        if role is not None:
            hub_schema[cv.Required(CONF_ROLE)] = validate_role

        return cv.Schema(hub_schema, extra=cv.ALLOW_EXTRA)(hub_config)

    return cv.Schema(
        {cv.Required(CONF_MODBUSTCP_ID): fv.id_declaration_match_schema(validate_hub)},
        extra=cv.ALLOW_EXTRA,
    )


async def register_modbus_device(var, config):
    parent = await cg.get_variable(config[CONF_MODBUSTCP_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(parent.register_device(var))
