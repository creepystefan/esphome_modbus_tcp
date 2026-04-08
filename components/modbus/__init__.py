from __future__ import annotations

from typing import Literal

from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, socket
from esphome.const import CONF_ADDRESS, CONF_DISABLE_CRC, CONF_FLOW_CONTROL_PIN, CONF_ID, CONF_TYPE
from esphome.cpp_helpers import gpio_pin_expression
import esphome.final_validate as fv



CONF_IP_ADDRESS = 'host'
CONF_PORT = 'port'
CONF_MODBUS_ID = "modbus_id"
CONF_SEND_WAIT_TIME = "send_wait_time"
CONF_ROLE = "role"
#CONF_VARIANT = "variant"
CONF_TURNAROUND_TIME = "turnaround_time"

modbus_ns = cg.esphome_ns.namespace("modbus")
#Modbus = modbus_ns.class_("Modbus", cg.Component, tcp.TCPDevice)
#Modbus = modbus_ns.class_("Modbus", cg.Component, uart.UartDevice)
Modbus = modbus_ns.class_("Modbus", cg.Component)
ModbusDevice = modbus_ns.class_("ModbusDevice")

ModbusRole = modbus_ns.enum("ModbusRole")
MODBUS_ROLES = {
    "client": ModbusRole.CLIENT,
    "server": ModbusRole.SERVER,
}


MULTI_CONF = True
AUTO_LOAD = ["uart", "async_tcp"]
DEPENDENCIES = ['network']



ModbusVariant = modbus_ns.enum("ModbusVariant")
MODBUS_VARIANT = {
    "TCP": ModbusVariant.MODBUS_VARIANT_TCP,
    "RTU": ModbusVariant.MODBUS_VARIANT_RTU,
}

BASE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Modbus),
        cv.Optional(
                CONF_SEND_WAIT_TIME, default="250ms"
            ): cv.positive_time_period_milliseconds,
        cv.Optional(
                CONF_TURNAROUND_TIME, default="100ms"
            ): cv.positive_time_period_milliseconds,
    }
).extend(cv.COMPONENT_SCHEMA)


TCP_VARIANT_SCHEMA = BASE_SCHEMA.extend(
    cv.Schema(
        {
            
            cv.Required(CONF_IP_ADDRESS): cv.ipv4address,
            #cv.Required('host'): cv.string,
            cv.Optional(CONF_PORT, default=502): cv.int_range(0, 65535),
            
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    #.extend(tcp.tcp_device_schema)

)

RTU_VARIANT_SCHEMA = BASE_SCHEMA.extend(
    cv.Schema(
        {
            
            cv.Optional(CONF_ROLE, default="client"): cv.enum(MODBUS_ROLES),
            cv.Optional(CONF_FLOW_CONTROL_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_DISABLE_CRC, default=False): cv.boolean,         
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
    
)

#def _validate(config):
#    if CONF_IP_ADDRESS in config:
#        DEPENDENCIES = ["uart"]
        #raise cv.Invalid(
        #    f"Host-IP Adresse missing"
        #)     

CONFIG_SCHEMA = cv.All(
    cv.typed_schema(
        {
            "TCP": TCP_VARIANT_SCHEMA,
            "RTU": RTU_VARIANT_SCHEMA,
                    },
        upper=True,
    ),
    #_validate,
)


async def to_code(config):
    
    cg.add_global(modbus_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_send_wait_time(config[CONF_SEND_WAIT_TIME]))
    cg.add(var.set_turnaround_time(config[CONF_TURNAROUND_TIME]))
    

    if config[CONF_TYPE] == "TCP":
          cg.add(var.set_host(str(config[CONF_IP_ADDRESS])))
          cg.add(var.set_port(config["port"]))
          cg.add(var.set_disable_crc(True))
          cg.add(var.set_tcp_or_rtu(True))

    if config[CONF_TYPE] == "RTU":
        await uart.register_uart_device(var, config)
        cg.add_global(modbus_ns.using)
        cg.add(var.set_role(config[CONF_ROLE]))
        cg.add(var.set_tcp_or_rtu(False))
        if CONF_FLOW_CONTROL_PIN in config:
            pin = await gpio_pin_expression(config[CONF_FLOW_CONTROL_PIN])
            cg.add(var.set_flow_control_pin(pin))
        cg.add(var.set_disable_crc(config[CONF_DISABLE_CRC]))
       


def modbus_device_schema(default_address):
    schema = {
        cv.GenerateID(CONF_MODBUS_ID): cv.use_id(Modbus),
    }
    if default_address is None:
        schema[cv.Required(CONF_ADDRESS)] = cv.hex_uint8_t
    else:
        schema[cv.Optional(CONF_ADDRESS, default=default_address)] = cv.hex_uint8_t
    return cv.Schema(schema)


async def register_modbus_device(var, config):
    parent = await cg.get_variable(config[CONF_MODBUS_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(parent.register_device(var))