#pragma once

#include "esphome/core/component.h"
#include <WiFiClient.h>

#include <vector>

namespace esphome {
namespace modbustcp {


class ModbusTCPDevice;

class ModbusTCP :  public Component {
 public:
  ModbusTCP() = default;
  //float get_setup_priority() const override;
  void setup() override;
  void loop() override;
  void dump_config() override;
  //void register_device(ModbusTCPDevice *device) { this->devices_.push_back(device); }
  void set_host_ip_address(const std::string &ip_address){this->ip_address_ = ip_address; }
  void set_host_port(uint16_t port){this->port_ = port; }
  void set_unit_id(uint8_t id){this->read_unit_id_ = id; }

 // void send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities,
 //           uint8_t payload_len = 0, const uint8_t *payload = nullptr);
  void send(uint8_t function_code, uint16_t address);
  
 protected:
  
  bool parse_modbus_byte_(uint8_t byte);
  
  std::string ip_address_;
  uint16_t port_;
  uint8_t read_unit_id_;
  uint32_t last_modbus_byte_{0};
  uint32_t last_send_{0};
  std::vector<ModbusTCPDevice *> devices_;
};

class ModbusTCPDevice {
 public:
  //void set_parent(ModbusTCP *parent) { parent_ = parent; }
  //void set_address(uint8_t address) { address_ = address; }
  //virtual void on_modbus_data(const std::vector<uint8_t> &data) = 0;
  //virtual void on_modbus_error(uint8_t function_code, uint8_t exception_code) {}
  //virtual void on_modbus_read_registers(uint8_t function_code, uint16_t start_address, uint16_t number_of_registers){};
  //virtual void on_modbus_write_registers(uint8_t function_code, const std::vector<uint8_t> &data){};
  void send(uint8_t function_code, uint16_t address) {
    this->parent_->send(function_code, address);
  }
  //void send_raw(const std::vector<uint8_t> &payload) { this->parent_->send_raw(payload); }
  //void send_error(uint8_t function_code, uint8_t exception_code) {
  //  std::vector<uint8_t> error_response;
  //  error_response.reserve(3);
  //  error_response.push_back(this->address_);
  //  error_response.push_back(function_code | 0x80);
  //  error_response.push_back(exception_code);
  //  this->send_raw(error_response);
  //}
  // If more than one device is connected block sending a new command before a response is received
  //bool waiting_for_response() { return parent_->waiting_for_response != 0; }

 protected:
  friend ModbusTCP;

  ModbusTCP *parent_;
  uint8_t address_;
};

}  // namespace modbustcp
}  // namespace esphome
