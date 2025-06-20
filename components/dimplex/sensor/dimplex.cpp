#include "dimplex.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dimplex {

static const char *const TAG = "Dimplex-Sensor-Modbus";



void DimplexTCP::update() {
WiFiClient client;
    if (!client.connect(host_.c_str(), port_)) {
      ESP_LOGE("modbus_tcp", "Failed to connect to Modbus server %s:%d", host_.c_str(), port_);
      return;
    }
  uint8_t request[] = {
        0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x01,
        functioncode_,
        (uint8_t)((register_address_ >> 8) & 0xFF),  
        (uint8_t)(register_address_ & 0xFF),
         0x00, 0x01
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
                      response[10]
                      );

unsigned int value = (response[9] << 8) | response[10];
publish_state(value);

}

void DimplexTCP::dump_config() {
  ESP_LOGCONFIG(TAG, "Address: %d", this->register_address_);
 }

}  // namespace dimplex
}  // namespace esphome
