#include "modbustcp.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modbustcp {

static const char *const TAG = "modbus_tcp";

void ModbusTCP::setup() {}

void ModbusTCP::loop() {

  const uint32_t now = App.get_loop_component_start_time();

 while (this->client.available()) {
  uint8_t byte1[256];
  this->client.read(byte1, sizeof(byte1));
  
  std::string res;
  char buf[5];
  size_t len = byte1[8];
  for (size_t i = 9; i < len + 9; i++) {
   sprintf(buf, "%02X", byte1[i]);
   res += buf;
   res += ":"; 
  }
ESP_LOGD(TAG, "<<< %02X%02X %02X%02X %02X%02X %02X %02X %02X %s ",
                      byte1[0], byte1[1], byte1[2], byte1[3], byte1[4], 
                      byte1[5], byte1[6], byte1[7], byte1[8], res.c_str());

if ((byte1[7] & 0x80) == 0x80) {
      ESP_LOGE(TAG,"Error:"); 
    if (byte1[8]  == 0x01) {
      ESP_LOGE(TAG,"Failure Code 0x01 ILLEGAL FUNCTION");
    }
    if (byte1[8]  == 0x02) {
      ESP_LOGE(TAG,"Failure Code 0x02 ILLEGAL DATA ADDRESS");
    }
    if (byte1[8]  == 0x03) {
      ESP_LOGE(TAG,"Failure Code 0x03 ILLEGAL DATA VALUE");
    }
    if (byte1[8]  == 0x04) {
      ESP_LOGE(TAG,"Failure Code 0x04 SERVER FAILURE");
    }
    if (byte1[8]  == 0x05) {
      ESP_LOGE(TAG,"Failure Code 0x05 ACKNOWLEDGE");
    }
    if (byte1[8]  == 0x06) {
      ESP_LOGE(TAG,"Failure Code 0x06 SERVER BUSY");
    }
    return;
  }

 uint8_t bytelen_len = 9;
  std::vector<uint8_t> data(byte1 + bytelen_len, byte1 + bytelen_len + 9);

//  ESP_LOGD(TAG, "Incomming Data %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
//                      data[0], data[1], data[2], data[3], data[4], 
//                      data[5], data[6], data[7], data[8]);
   


 for (auto *device : this->devices_) {
        device->on_modbus_data(data);
     }
 }
}


void ModbusTCP::dump_config() {
  ESP_LOGCONFIG(TAG, "Modbus_TCP:");
  ESP_LOGCONFIG(TAG,
                "  Client: %s:%d \n"
                "  Send Wait Time: %d ms\n",
                   host_.c_str(), port_, this->send_wait_time_);
}
float ModbusTCP::get_setup_priority() const {
  // After UART bus
  return setup_priority::BUS - 1.0f;

  }

void ModbusTCP::send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len, const uint8_t *payload) {
  static const size_t MAX_VALUES = 128;
 const uint32_t now = App.get_loop_component_start_time();
  // Only check max number of registers for standard function codes
  // Some devices use non standard codes like 0x43
  if (number_of_entities > MAX_VALUES && function_code <= 0x10) {
    ESP_LOGE(TAG, "send too many values %d max=%zu", number_of_entities, MAX_VALUES);
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
        data_send.push_back(0x06);      // wie viele Bytes kommen
      }
     data_send.push_back(address);
     data_send.push_back(function_code);
     data_send.push_back(start_address >> 8);
     data_send.push_back(start_address >> 0);
    // function nicht 5 oder nicht 6
     if (function_code != ModbusFunctionCode::WRITE_SINGLE_COIL &&
        function_code != ModbusFunctionCode::WRITE_SINGLE_REGISTER) {
      data_send.push_back(number_of_entities >> 8);
      data_send.push_back(number_of_entities >> 0);
    }
  

  if (payload != nullptr) {
    if (function_code == ModbusFunctionCode::WRITE_MULTIPLE_COILS ||
        function_code == ModbusFunctionCode::WRITE_MULTIPLE_REGISTERS) {  // Write multiple
      data_send.push_back(payload_len);                                        // Byte count is required for write
    } else {
      payload_len = 2;  // Write single register or coil
    }
    for (int i = 0; i < payload_len; i++) {
      data_send.push_back(payload[i]);
    }
  }



      /*     
    if (function_code != 0x5 && function_code != 0x6) {
     data_send.push_back(0x06);                          // byte 5
     data_send.push_back(address);
     data_send.push_back(function_code);
     data_send.push_back(start_address >> 8);
     data_send.push_back(start_address >> 0);
     data_send.push_back(number_of_entities >> 8);
     data_send.push_back(number_of_entities >> 0);
      }
    if (function_code == 0x5 && function_code == 0x6) {
     data_send.push_back(payload_len + 0x04);            // byte 5
     data_send.push_back(address);
     data_send.push_back(function_code);
     data_send.push_back(start_address >> 8);
     data_send.push_back(start_address >> 0);
     for (int i = 0; i < payload_len; i++) {
     data_send.push_back(payload[i]);
     }
        
    }
  if (payload != nullptr) {
     if (function_code == 0xF || function_code == 0x10) {  // Write multiple
       data_send.push_back(payload_len);  // Byte count is required for write
    } else {
      payload_len = 2;  // Write single register or coil
    }
    for (int i = 0; i < payload_len; i++) {
     data_send.push_back(payload[i]);
    }

  }
*/
   if (!client.connect(host_.c_str(), port_)) {
      ESP_LOGE(TAG, "Failed to connect to Modbus server %s:%d", host_.c_str(), port_);
      return;
    }
 std::string res1;
 char buf1[5];
 size_t len1 = 11; 
 for (size_t i = 12; i < data_send[5] + 6; i++) {
 sprintf(buf1, "%02X", data_send[i]);
res1 += buf1;
res1 += ":"; 
}
    client.write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send));
    ESP_LOGD(TAG, ">>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X %s",
                   data_send[0], data_send[1],  data_send[2], data_send[3], data_send[4], data_send[5],
                   data_send[6], data_send[7],  data_send[8], data_send[9], data_send[10], data_send[11], res1.c_str());
    

//    delay(200);
  last_send_ = millis();
}
// Helper function for lambdas
// Send raw command. Except CRC everything must be contained in payload


void ModbusTCP::send_raw(const std::vector<uint8_t> &payload) {
  if (payload.empty()) {
    return;
  }

 // this->write_array(payload);
 // this->write_byte(crc & 0xFF);
 // this->write_byte((crc >> 8) & 0xFF);
//  this->flush();
  waiting_for_response = payload[0];
//  ESP_LOGV(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
  last_send_ = millis();
}

}  // namespace modbustcp
}  // namespace esphome
