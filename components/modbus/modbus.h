#pragma once

#include "esphome/core/component.h"
#include <WiFiClient.h>
#include <vector>
#include "esphome/components/modbus/modbus_definitions.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#else
#error Platform not supported
#endif

namespace esphome {
namespace modbus {

  enum ModbusRole {
  CLIENT,
  SERVER,
};

class ModbusDevice;

class Modbus :  public Component {

 public:
 
   Modbus() = default;

  void setup() override;

  void loop() override;

  void dump_config() override;

  void register_device(ModbusDevice *device) { this->devices_.push_back(device); }

  float get_setup_priority() const override;

  void send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities,
            uint8_t payload_len = 0, const uint8_t *payload = nullptr);
  void send_raw(const std::vector<uint8_t> &payload);
  void set_role(ModbusRole role) { this->role = role; }
  uint8_t waiting_for_response{0};
  void set_send_wait_time(uint16_t time_in_ms) { send_wait_time_ = time_in_ms; }
  void set_host(const std::string &host) { this->host_ = host; }
  void set_port(uint16_t port) { this->port_ = port; }
  
  //bool server_ready_ = false;
  //bool client_ready_ = false;

  //void ensure_tcp_server();
  //void ensure_tcp_client();

ModbusRole role;

 protected:
 
  bool parse_modbus_byte_(uint8_t byte1[256]);
  uint16_t send_wait_time_{250};
  uint32_t last_modbus_byte_{0};
  uint32_t last_send_{0};
  std::vector<ModbusDevice *> devices_;
  uint16_t Transaction_Identifier = 0;
  WiFiClient client;
  uint16_t port_;
  std::string host_;
  AsyncClient tcp_client;
   
};

class ModbusDevice {
 public:
  void set_parent(Modbus *parent) { parent_ = parent; }
  void set_address(uint8_t address) { address_ = address; }
  virtual void on_modbus_data(const std::vector<uint8_t> &data) = 0;
  virtual void on_modbus_error(uint8_t function_code, uint8_t exception_code) {}
  virtual void on_modbus_read_registers(uint8_t function_code, uint16_t start_address, uint16_t number_of_registers){};
  virtual void on_modbus_write_registers(uint8_t function_code, const std::vector<uint8_t> &data){};
  void send(uint8_t function, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len = 0,
            const uint8_t *payload = nullptr) {
    this->parent_->send(this->address_, function, start_address, number_of_entities, payload_len, payload);
  }
  void send_raw(const std::vector<uint8_t> &payload) { this->parent_->send_raw(payload); }
  void send_error(uint8_t function_code, ModbusExceptionCode exception_code) {
    std::vector<uint8_t> error_response;
    error_response.reserve(3);
    error_response.push_back(this->address_);
    error_response.push_back(function_code | FUNCTION_CODE_EXCEPTION_MASK);
    error_response.push_back(static_cast<uint8_t>(exception_code));
    this->send_raw(error_response);
  }
  // If more than one device is connected block sending a new command before a response is received
  bool waiting_for_response() { return parent_->waiting_for_response != 0; }

    

 protected:
  friend Modbus;
  
  Modbus *parent_;
  uint8_t address_;
};

}  // namespace modbus
}  // namespace esphome
