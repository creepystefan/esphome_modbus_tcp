#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <WiFiClient.h>


namespace esphome {
namespace modbus_tcp_32 {


class ModbusTCP32 :  public sensor::Sensor, public PollingComponent {
 public:
   ModbusTCP32() : PollingComponent(5000) {}

float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

void set_host(const std::string &host) { this->host_ = host; }
void set_port(uint16_t port) { this->port_ = port; }
void set_functioncode(uint8_t functioncode) { this->functioncode_ = functioncode; }
void set_register_address(uint16_t register_address) { this->register_address_ = register_address; }
void set_unit_id(uint8_t unit_id) { this->unit_id_ = unit_id; }
void set_timeout(uint32_t timeout) { this->timeout_ = timeout; }


void setup() override{
}

void update() override;



void dump_config() override;

protected:
  uint16_t register_address_;
  uint8_t functioncode_;
  uint8_t unit_id_;
  uint16_t port_;
  std::string host_;

};

}  // namespace modbus_tcp_32
}  // namespace esphome

