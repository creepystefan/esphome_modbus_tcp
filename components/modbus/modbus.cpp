#include "modbus.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/network/util.h"

namespace esphome {
namespace modbus {

static const char *const TAG = "modbus-TCP";
static constexpr size_t MODBUS_MAX_LOG_BYTES = 64;

void Modbus::setup() {
    //   client.connect(host_.c_str(), port_);
    }

void Modbus::loop() {
  const uint32_t now = App.get_loop_component_start_time();
     
  while (this->client.available()) {
  uint8_t byte[256];
  this->client.read(byte, sizeof(byte));
  //this->client.clear();
  std::string res;
  char buf[5];
  size_t data_len = byte[8];
  for (size_t i = 9; i < data_len + 9; i++) {
   sprintf(buf, "%02X", byte[i]);
   res += buf;
   res += ":"; 
  }
  ESP_LOGD(TAG, "<<< %02X%02X %02X%02X %02X%02X %02X %02X %02X %s ",
                      byte[0], byte[1], byte[2], byte[3], byte[4], 
                      byte[5], byte[6], byte[7], byte[8], res.c_str());
  
   if (this->parse_modbus_byte_(byte)) {
      this->last_modbus_byte_ = now;
    }
  }
      // stop blocking new send commands after sent_wait_time_ ms after response received
    if (now - this->last_send_ > send_wait_time_) {
      if (waiting_for_response > 0) {
        ESP_LOGV(TAG, "Stop waiting for response from %d", waiting_for_response);
      }
      waiting_for_response = 0;
    }
  }
  


bool Modbus::parse_modbus_byte_(uint8_t byte[256]) {
  uint8_t bytelen_len = 9;
  size_t data_len = byte[8];
  
  std::vector<uint8_t> data(byte + bytelen_len, byte + bytelen_len + bytelen_len + data_len);
  bool found = false;
  uint8_t address = byte[6];
  uint8_t function_code = byte[7];

 // logging onyl DATA Bytes
std::string resdata;
  char bufdata[5];
  //size_t data_len = byte[8];
  for (size_t i = 0; i < data_len; i++) {
   sprintf(bufdata, "%02X", data[i]);
   resdata += bufdata;
   resdata += ":"; 
  }
  ESP_LOGD(TAG, "data %s ", resdata.c_str());


  for (auto *device : this->devices_) {
    if (device->address_ == address) {
      found = true;
      // Is it an error response?
      if ((function_code & FUNCTION_CODE_EXCEPTION_MASK) == FUNCTION_CODE_EXCEPTION_MASK) {
        ESP_LOGD(TAG, "Modbus error function code: 0x%X exception: %d", function_code, byte[8]);
        if (waiting_for_response != 0) {
          device->on_modbus_error(function_code & FUNCTION_CODE_MASK, byte[8]);
        } else {
          // Ignore modbus exception not related to a pending command
          ESP_LOGD(TAG, "Ignoring Modbus error - not expecting a response");
        }
        continue;
      }
          
      device->on_modbus_data(data);
    }
    waiting_for_response = 0;
    
   }
  return true;
}

void Modbus::dump_config() {
  ESP_LOGCONFIG(TAG, "Modbus_TCP:");
  ESP_LOGCONFIG(TAG, "  Client: %s:%d \n"
                     "  Send Wait Time: %d ms\n",
                         host_.c_str(), port_, this->send_wait_time_);
}


float Modbus::get_setup_priority() const { return setup_priority::AFTER_WIFI; }

void Modbus::send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len, const uint8_t *payload) {
  static const size_t MAX_VALUES = 128;
 const uint32_t now = App.get_loop_component_start_time();

  // Only check max number of registers for standard function codes
  // Some devices use non standard codes like 0x43
  //if (number_of_entities > MAX_VALUES && function_code <= 0x10) {
  //  ESP_LOGE(TAG, "send too many values %d max=%zu", number_of_entities, MAX_VALUES);
  //  return;
  //}
  if (!client.connect(host_.c_str(), port_)) {
      ESP_LOGE("modbus_tcp", "Failed to connect to Modbus server %s:%d", host_.c_str(), port_);
      return;
    }

 std::vector<uint8_t> data_send;
 Transaction_Identifier++;
      data_send.push_back(Transaction_Identifier >> 8);
      data_send.push_back(Transaction_Identifier >> 0);
      data_send.push_back(0x00);
      data_send.push_back(0x00);
      data_send.push_back(0x00);
      if (payload != nullptr) { 
        data_send.push_back(0x04 + payload_len);
      }else {
        data_send.push_back(0x06);      // how many bytes next comes
      }
     data_send.push_back(address);
     data_send.push_back(function_code);
     data_send.push_back(start_address >> 8);
     data_send.push_back(start_address >> 0);
    // function nicht 5 oder nicht 6
     if (function_code != 0x05 && function_code != 0x06) {
      data_send.push_back(number_of_entities >> 8);
      data_send.push_back(number_of_entities >> 0);
     }
  

  if (payload != nullptr) {
    if (function_code == 0x0F || function_code == 0x17) {  // Write multiple
      data_send.push_back(payload_len);                    // Byte count is required for write
    } else {
      payload_len = 2;  // Write single register or coil
    }
    for (int i = 0; i < payload_len; i++) {
      data_send.push_back(payload[i]);
    }
  }


      
 std::string res1;
 char buf1[5];
 size_t len1 = 11; 
 for (size_t i = 12; i < data_send[5] + 6; i++) {
 sprintf(buf1, "%02X", data_send[i]);
res1 += buf1;
res1 += ":";
}
    this->client.write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send));

    //client.write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send));
    ESP_LOGD(TAG, ">>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X %s",
                   data_send[0], data_send[1],  data_send[2], data_send[3], data_send[4], data_send[5],
                   data_send[6], data_send[7],  data_send[8], data_send[9], data_send[10], data_send[11], res1.c_str());
   // this->tcp_client.clear();


waiting_for_response = address;
last_send_ = millis();

}
// Helper function for lambdas
// Send raw command. Except CRC everything must be contained in payload


void Modbus::send_raw(const std::vector<uint8_t> &payload) {
  if (payload.empty()) {
    return;
  }

 // this->write_array(payload);
  this->client.write(reinterpret_cast<const char*>(payload.data()), sizeof(payload));
  //this->client.clear();
  waiting_for_response = payload[0];
  ESP_LOGV(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
  last_send_ = millis();
}

}  // namespace modbus
}  // namespace esphome
