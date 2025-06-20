#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <WiFiClient.h>


namespace esphome {
namespace dimplex {


class DimplexTCP :  public sensor::Sensor, public PollingComponent {
 public:
   DimplexTCP() : PollingComponent(5000) {}

float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

//void update_interval(uint32_t update_interval) { this->update_interval_ = update_interval; }
void set_host(const std::string &host) { this->host_ = host; }
void set_port(uint16_t port) { this->port_ = port; }
void set_functioncode(uint8_t functioncode) { this->functioncode_ = functioncode; }
void set_register_address(uint16_t register_address) { this->register_address_ = register_address; }


void setup() override{
}

void update() override{
WiFiClient client;
    if (!client.connect(host_.c_str(), port_)) {
      ESP_LOGE("modbus_tcp", "Failed to connect to Modbus server %s:%d", host_.c_str(), port_);
      return;
    }


  uint8_t request[] = {
        0x00, 0x08,  // Transaction ID
        0x00, 0x00,  // Protocol ID
        0x00, 0x06,  // Length
        0x01,        // Unit ID
        functioncode_,
        (uint8_t)((register_address_ >> 8) & 0xFF),  // Start Address (High Byte)
        (uint8_t)(register_address_ & 0xFF),        // Start Address (Low Byte)
        0x00, 0x01   // Quantity (Read 2 Registers = 32 bits for FP32)
        };
    ESP_LOGD("TX", "Address: %d >>>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X",
                        this->register_address_,
                        request[0], request[1], request[2], request[3], request[4]
                        , request[5], request[6], request[7], request[8]
                        , request[9], request[10], request[11]
                        );

client.write(request, sizeof(request));
delay(100);

uint8_t response[256];
    size_t response_len = client.read(response, sizeof(response));

if (response[7] != request[7]) {
      ESP_LOGE("modbus_tcp", "Unexpected function code: 0x%02X", response[7]);
      return;
    }

if (response[1] != request[1]) {
      ESP_LOGE("modbus_tcp", "Unexpected Transaction ID: 0x%02X", response[1]);
      return;
    }

 
    ESP_LOGD("RX", "Address: %d <<<< %02X%02X %02X%02X %02X%02X %02X %02X %02X %02X%02X ",
                      this->register_address_,
                      response[0], response[1], response[2], response[3], response[4], 
                      response[5], response[6], response[7], response[8], response[9], 
                      response[10]                      );

unsigned int value = (response[9] << 8) | response[10];
publish_state(value);

}
 
void dump_config() override;

protected:
  uint16_t register_address_;
  uint8_t functioncode_;
//  uint32_t update_interval_;
  uint16_t port_;
  std::string host_;

};

}  // namespace dimplex
}  // namespace esphome
