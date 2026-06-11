#include "modbustcp.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
//#include "esphome/components/network/util.h"

namespace esphome::modbustcp {

static const char *const TAG = "modbustcp";

void ModbusTCP::setup() {
       client_ = new AsyncClient();

    client_->onConnect([](void* arg, AsyncClient* c) {
      auto* self = (ModbusTCP*)arg;
      ESP_LOGI("tcp", "Connected to %s:%d", self->host_.c_str(), self->port_);
      self->connected_ = true;
    }, this);

    client_->onDisconnect([](void* arg, AsyncClient* c) {
      auto* self = (ModbusTCP*)arg;
      ESP_LOGW("tcp", "Disconnected, will retry...");
      self->connected_ = false;
      self->last_attempt_ = 0;
    }, this);

    client_->onError([](void* arg, AsyncClient* c, int8_t err) {
      auto* self = (ModbusTCP*)arg;
      ESP_LOGE("tcp", "Error: %s", c->errorToString(err));
      self->connected_ = false;
    }, this);

    client_->onData([](void* arg, AsyncClient* c, void *data, size_t len) {
      auto* self = (ModbusTCP*)arg;
      uint8_t *byte = reinterpret_cast<uint8_t*>(data);
      self->handle_message(byte);
    }, this);
  }
  

      void ModbusTCP::connect() {
    if (!client_->connecting()) {
      if (!client_->connect(host_.c_str(), port_)) {
        ESP_LOGW("tcp", "Connection failed, will retry...");
      }
    }
  }
/*
  void Modbus::send_message(const uint8_t *send_byte) {
    if (connected_ && client_->canSend()) {
      //client_->write(message.c_str());
      //client_->write(reinterpret_cast<const char*>(frame.data.get()), sizeof(frame.data));

    
   //for (size_t i = 0; i < sizeof(send_byte); i++) {
    //sprintf(buf1, "%02X", send_byte[i]);

      client_->write(reinterpret_cast<const char*>(send_byte), send_byte[5] + 6);
      
        
     std::string res1;
     char buf1[5];
      
     //for (size_t i = 12; i < send_byte[5] + 6; i++) {
     for (size_t i = 0; i < 12; i++) {
     sprintf(buf1, "%02X", send_byte[i]);
     res1 += buf1;
     res1 += ":";
     }
     ESP_LOGD("modbus_tcp", ">>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X %s",
                   send_byte[0], send_byte[1],  send_byte[2], send_byte[3], send_byte[4], send_byte[5],
                   send_byte[6], send_byte[7],  send_byte[8], send_byte[9], send_byte[10], send_byte[11], res1.c_str());
                  
      } else {
      ESP_LOGW("modbus_tcp", "Cannot send, not connected");
    }
  }
*/
  void ModbusTCP::handle_message(uint8_t byte[256]) {
    //const uint32_t now = App.get_loop_component_start_time();
    //const uint32_t now = App.get_loop_component_start_time();

    //uint8_t *bytedata = (uint8_t*)&byte;
   //size_t len = sizeof(rx_data_);
  
  uint8_t bytelen_len = 9;
  size_t data_len = byte[8];
  uint8_t address = byte[6];
  uint8_t function_code = byte[7];
  
  std::vector<uint8_t> data(byte + bytelen_len, byte + bytelen_len + bytelen_len + data_len);
  bool found = false;  
  
  
   std::string res;
      char buf[5];
      //size_t data_len = byte[8];
  for (size_t i = 9; i < data_len + 9; i++) {
      sprintf(buf, "%02X", byte[i]);
      res += buf;
      res += ":"; 
  }
  
  ESP_LOGD("modbus_tcp", " <<< %02X%02X %02X%02X %02X%02X %02X %02X %02X %s ",
                      byte[0], byte[1], byte[2], byte[3], byte[4], 
                      byte[5], byte[6], byte[7], byte[8], res.c_str());
   
   /*
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
  */
   for (auto *device : this->devices_) {
          device->on_modbus_data(data);
    }
   }
 



  void ModbusTCP::on_shutdown() {
    if (client_) {
      client_->close();
    }
  }

    

void ModbusTCP::loop() {
  if (!connected_) {
      uint32_t nowtcp = millis();
      if (nowtcp - last_attempt_ > 5000) {
        last_attempt_ = nowtcp;
        ESP_LOGW("tcp", "Reconnecting...");
        connect();
      }
    }
  const uint32_t now = App.get_loop_component_start_time();
  

 if (now - this->last_send_ > send_wait_time_) {
      if (waiting_for_response > 0) {
        //ESP_LOGD(TAG, "Stop waiting for response from %d", waiting_for_response);
      }
      waiting_for_response = 0;
    }
  


  }
void ModbusTCP::dump_config() {
  ESP_LOGCONFIG(TAG, "Modbus_TCP:");
  ESP_LOGCONFIG(TAG, "  Client: %s \n"
                     "  Port: %d \n"
                     "  Send Wait Time: %d ms\n",
                         host_.c_str(), port_, this->send_wait_time_);
}

float ModbusTCP::get_setup_priority() const { return setup_priority::AFTER_WIFI; }

void ModbusTCP::send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len, const uint8_t *payload) {
  static const size_t MAX_VALUES = 128;
 const uint32_t now = App.get_loop_component_start_time();
  // Only check max number of registers for standard function codes
  // Some devices use non standard codes like 0x43
  if (number_of_entities > MAX_VALUES && function_code <= 0x10) {
    ESP_LOGE(TAG, "send too many values %d max=%zu", number_of_entities, MAX_VALUES);
    return;
  }
  // if (!client.connect(host_.c_str(), port_)) {
  //    ESP_LOGE("modbus_tcp", "Failed to connect to Modbus server %s:%d", host_.c_str(), port_);
  //    return;
  //  }
 std::vector<uint8_t> data_send;
 
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
   if (connected_ && client_->canSend()) {
    //if (connected_) {
    client_->write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send));
    Transaction_Identifier++;
    //client.write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send));
    ESP_LOGD(TAG, ">>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X %s",
                   data_send[0], data_send[1],  data_send[2], data_send[3], data_send[4], data_send[5],
                   data_send[6], data_send[7],  data_send[8], data_send[9], data_send[10], data_send[11], res1.c_str());
   }

waiting_for_response = address;
last_send_ = millis();
}


void ModbusTCP::send_raw(const std::vector<uint8_t> &payload) {
  if (payload.empty()) {
    return;
  }

 // this->write_array(payload);
  client_->write(reinterpret_cast<const char*>(payload.data()), sizeof(payload));
  //this->client.clear();
  waiting_for_response = payload[0];
  ESP_LOGV(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
  last_send_ = millis();
}


}  // namespace esphome::modbustcp
