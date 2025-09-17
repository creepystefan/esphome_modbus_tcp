#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <WiFiClient.h>


namespace esphome {
namespace modbus_tcp_float {


class ModbusTCPfloat :  public sensor::Sensor, public PollingComponent {
 public:
   ModbusTCPfloat() : PollingComponent(5000) {}

float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

//void update_interval(uint32_t update_interval) { this->update_interval_ = update_interval; }
void set_host(const std::string &host) { this->host_ = host; }
void set_port(uint16_t port) { this->port_ = port; }
void set_functioncode(uint8_t functioncode) { this->functioncode_ = functioncode; }
void set_register_address(uint16_t register_address) { this->register_address_ = register_address; }
void set_unit_id(uint8_t unit_id) { this->unit_id_ = unit_id; }


void setup() override{
}

void update() override;

 float decode_float(uint8_t *data) {
    uint32_t raw_value = 0;
    raw_value = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    float value;
    memcpy(&value, &raw_value, sizeof(float));
    return value;
  }



void dump_config() override;

protected:
  uint16_t register_address_;
  uint8_t functioncode_;
  uint8_t unit_id_;
//  uint32_t update_interval_;
  uint16_t port_;
  std::string host_;

};

}  // namespace modbus_tcp_float
}  // namespace esphome
