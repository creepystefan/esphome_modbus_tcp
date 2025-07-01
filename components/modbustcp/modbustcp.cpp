#include "modbustcp.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modbustcp {

static const char *const TAG = "modbustcp";

void ModbusTCP::setup() {}
void ModbusTCP::loop() {
}

void ModbusTCP::dump_config() {
  ESP_LOGCONFIG(TAG, "ModbusTCP:");
  }


void ModbusTCP::send(uint8_t functioncode_, uint16_t address_) {
    WiFiClient client; 
   //ESP_LOGD("TAG", "Port %d", this->port_);

uint8_t request[] = {
        0x00, 0x08,  // Transaction ID
        0x00, 0x00,  // Protocol ID
        0x00, 0x06,  // Length
        0x01,        // Unit ID
        functioncode_,
        (uint8_t)((address_ >> 8) & 0xFF),  // Start Address (High Byte)
        (uint8_t)(address_ & 0xFF),        // Start Address (Low Byte)
        0x00, 0x01   // Quantity (Read 2 Registers = 32 bits for FP32)
        };
    ESP_LOGD("TX", "Address:  >>>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X",
                        request[0], request[1], request[2], request[3], request[4]
                        , request[5], request[6], request[7], request[8]
                        , request[9], request[10], request[11]
                        );
}

}  // namespace modbustcp
}  // namespace esphome
