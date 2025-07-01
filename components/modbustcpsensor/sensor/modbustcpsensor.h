#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/modbustcp/modbustcp.h"
#include <vector>

namespace esphome {
namespace modbustcpsensor {


class ModbusTCPSensor :  public sensor::Sensor, public PollingComponent, modbustcp::ModbusTCPDevice {
 public:
   ModbusTCPSensor() : PollingComponent(5000) {}

//float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

void set_functioncode(uint8_t functioncode) { this->functioncode_ = functioncode; }
void set_address(uint16_t address) { this->address_ = address; }

void setup() override{
}

void update() override;

void dump_config() override;

protected:
  uint16_t register_address_;
  uint8_t functioncode_;

};

}  // namespace modbus_tcp_16
}  // namespace esphome