#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include <WiFiClient.h>


namespace esphome {
namespace dimplex_binary_sensor {


class DimplexTCP_Binary_Sensor :  public binary_sensor::BinarySensor, public PollingComponent {
 public:
   DimplexTCP_Binary_Sensor() : PollingComponent(5000) {}

float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

//void update_interval(uint32_t update_interval) { this->update_interval_ = update_interval; }
void set_host(const std::string &host) { this->host_ = host; }
void set_port(uint16_t port) { this->port_ = port; }
void set_functioncode(uint8_t functioncode) { this->functioncode_ = functioncode; }
void set_register_address(uint16_t register_address) { this->register_address_ = register_address; }


void setup() override{
}

void update() override;
 
void dump_config() override;

protected:
  uint16_t register_address_;
  uint8_t functioncode_;
//  uint32_t update_interval_;
  uint16_t port_;
  std::string host_;

};

}  // namespace dimplex_binary_sensor
}  // namespace esphome
