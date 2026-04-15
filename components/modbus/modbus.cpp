#include "modbus.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/network/util.h"

namespace esphome::modbus {


static const char *const TAG = "modbus";
static constexpr size_t MODBUS_MAX_LOG_BYTES = 64;

void Modbus::setup() {
  client_ = new AsyncClient();
  //this->modbus_tcp_client->connect(host_.c_str(), port_);
  //    ESP_LOGD("tcp", "TCP Client setup");
  

    client_->onConnect([](void* arg, AsyncClient* c) {
      auto* self = (Modbus*)arg;
      ESP_LOGI("tcp", "Connected to %s:%d", self->host_.c_str(), self->port_);
      self->connected_ = true;
    }, this);

    client_->onDisconnect([](void* arg, AsyncClient* c) {
      auto* self = (Modbus*)arg;
      ESP_LOGW("tcp", "Disconnected, will retry...");
      self->connected_ = false;
      self->last_attempt_ = 0;
    }, this);

    client_->onError([](void* arg, AsyncClient* c, int8_t err) {
      auto* self = (Modbus*)arg;
      ESP_LOGE("tcp", "Error: %s", c->errorToString(err));
      self->connected_ = false;
    }, this);
/*
    client_->onData([](void* arg, AsyncClient* c, void *data, size_t len) {
      auto* self = (Modbus*)arg;
      //self->rx_buf_.append((char*)data, len);
      uint8_t self->rx_buf_ = (uint8_t *)data;
      ESP_LOGD(TAG, "received %02x", data);     
      size_t pos;
      while ((pos = self->rx_buf_.find('\n')) != std::string::npos) {
       std::string line = self->rx_buf_.substr(0, pos);
        self->rx_buf_.erase(0, pos + 1);
        self->handle_message(line);
        
      }
    }, this);
    */
    client_->onData([](void* arg, AsyncClient* c, void *data, size_t len) {
      auto* self = (Modbus*)arg;
      //uint8_t byte[len] = (uint8_t*)data;
      //uint8_t *byte = (uint8_t*)&data;
      uint8_t *byte = reinterpret_cast<uint8_t*>(data);
      //ESP_LOGD(TAG, "received %02x, Length: %02x", data, len);     
      //size_t pos;
        self->handle_message(byte);
        //self->parse_modbus_byte_(byte);
      //this->last_modbus_byte_ = now;
    
    }, this);

    //connect();
  
   }

      void Modbus::connect() {
    if (!client_->connecting()) {
      if (!client_->connect(host_.c_str(), port_)) {
        ESP_LOGW("tcp", "Connection failed, will retry...");
      }
    }
  }
/*
  void Modbus::send_message(const std::string& message) {
    if (connected_ && client_->canSend()) {
      client_->write(message.c_str());
      ESP_LOGD("tcp", "Sent: %s", message.c_str());
    } else {
      ESP_LOGW("tcp", "Cannot send, not connected");
    }
  }
*/

  void Modbus::handle_message(uint8_t byte[256]) {
    //const uint32_t now = App.get_loop_component_start_time();

    //uint8_t *bytedata = (uint8_t*)&byte;
   //size_t len = sizeof(rx_data_);
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
     // this->last_modbus_byte_ = now;
    }
    
   
    }

  void Modbus::on_shutdown() {
    if (client_) {
      client_->close();
    }
  }

void Modbus::loop() {

  const uint32_t now = App.get_loop_component_start_time();
      if (!connected_) {
      uint32_t nowtcp = millis();
      if (nowtcp - last_attempt_ > 5000) {
        last_attempt_ = nowtcp;
        ESP_LOGW("tcp", "Reconnecting...");
        connect();
      }
    }
    if (now - this->last_send_ > send_wait_time_) {
      if (waiting_for_response_ > 0) {
        ESP_LOGV(TAG, "Stop waiting for response from %d", waiting_for_response_);
      }
      waiting_for_response_ = 0;
    }
  //std::vector<uint8_t> byte;
  //std::vector<uint8_t> byte;
  //read_data_tcp(byte);
  //uint8_t byte[256];
  //AsyncClient *tcp_client = new AsyncClient;
  //this->tcp_client.onData(AcDataHandler (void data)) {
 //   tcp_client->onData([this](void *s, AsyncClient *c, void *byte, size_t len) {
 //   ESP_LOGD(TAG, "test %s", byte);
 // }, 0);
  //ESP_LOGD(TAG, "test %s", byte);
  //this->tcp_client.onData(AcDataHandler, void* (data, size_t len)) {
  //ESP_LOGD(TAG, "test %s", data);
  //}
  // if (this->tcp_client.available());
  //while (this->client.available()) {
  
  /*
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
  */
  // if (this->parse_modbus_byte_(byte)) {
  //    this->last_modbus_byte_ = now;
  //  }
//}
/*
// stop blocking new send commands after sent_wait_time_ ms after response received
    if (now - this->last_send_ > send_wait_time_) {
      if (waiting_for_response > 0) {
        ESP_LOGV(TAG, "Stop waiting for response from %d", waiting_for_response);
      }
      waiting_for_response = 0;
    }
  }
*/  
}

bool Modbus::parse_modbus_byte_(uint8_t byte[256]) {

  uint8_t bytelen_len = 9;
  size_t data_len = byte[8];
  uint8_t address = byte[6];
  uint8_t function_code = byte[7];
  
  std::vector<uint8_t> data(byte + bytelen_len, byte + bytelen_len + bytelen_len + data_len);
  bool found = false;
  
 // logging onyl DATA Bytes
 /*
  std::string resdata;
  char bufdata[5];
  //size_t data_len = byte[8];
  for (size_t i = 0; i < data_len; i++) {
   sprintf(bufdata, "%02X", data[i]);
   resdata += bufdata;
   resdata += ":"; 
  }
  ESP_LOGD(TAG, "data %s ", resdata.c_str());
  */

  for (auto *device : this->devices_) {
    if (device->address_ == address) {
      found = true;
      // Is it an error response?
      if ((function_code & FUNCTION_CODE_EXCEPTION_MASK) == FUNCTION_CODE_EXCEPTION_MASK) {
        ESP_LOGD(TAG, "Modbus error function code: 0x%X exception: %d", function_code, byte[8]);
        if (waiting_for_response_ != 0) {
          device->on_modbus_error(function_code & FUNCTION_CODE_MASK, byte[8]);
        } else {
          // Ignore modbus exception not related to a pending command
          ESP_LOGD(TAG, "Ignoring Modbus error - not expecting a response");
        }
        continue;
      }
 
      device->on_modbus_data(data);
    }
    waiting_for_response_ = 0;
    
   }
  return true;
}
 

void Modbus::dump_config() {
  if (tcp_or_rtu_) {
  ESP_LOGCONFIG(TAG,
                     "Modbus_TCP: \n"
                     "  Host: %s:%d \n"
                     "  Send Wait Time: %d ms\n"
                     "  Turnaround Time: %d ms\n",
                         host_.c_str(), port_, this->send_wait_time_,
                        this->turnaround_delay_ms_);

                        
  } else {
    ESP_LOGCONFIG(TAG,
                "Modbus_RTU:\n"
                "  Send Wait Time: %d ms\n"
                "  Turnaround Time: %d ms\n"
                "  CRC Disabled: %s",
                this->send_wait_time_, this->turnaround_delay_ms_, 
                YESNO(this->disable_crc_));
    //LOG_PIN("  Flow Control Pin: ", this->flow_control_pin_);
    }
  }

float Modbus::get_setup_priority() const { 
  if (tcp_or_rtu_) {
  return setup_priority::AFTER_WIFI;
  } else {
    return setup_priority::BUS - 1.0f;
  }
  }

void Modbus::send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len, const uint8_t *payload) {
  static const size_t MAX_VALUES = 128;
 const uint32_t now = App.get_loop_component_start_time();

 std::vector<uint8_t> data_send;
if (tcp_or_rtu_) { 
      Transaction_Identifier++;
      data_send.push_back(Transaction_Identifier >> 8);
      data_send.push_back(Transaction_Identifier >> 0);
      data_send.insert(data_send.end(), {0x00, 0x00, 0x00});
      //data_send.push_back(0x00);
      //data_send.push_back(0x00);
      //data_send.push_back(0x00);
      if (payload != nullptr) { 
        data_send.push_back(0x04 + payload_len);
      }else {
        data_send.push_back(0x06);      // how many bytes next comes
      }
    }  
     data_send.push_back(address);
     data_send.push_back(function_code);
     if (this->role == ModbusRole::CLIENT) {
       data_send.push_back(start_address >> 8);
       data_send.push_back(start_address >> 0);
        if (function_code != ModbusFunctionCode::WRITE_SINGLE_COIL &&
        function_code != ModbusFunctionCode::WRITE_SINGLE_REGISTER) {
    // function nicht 5 oder nicht 6
     //if (function_code != 0x05 && function_code != 0x06) {
      data_send.push_back(number_of_entities >> 8);
      data_send.push_back(number_of_entities >> 0);
     }
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
    if (tcp_or_rtu_) {
     std::string res1;
     char buf1[5];
     size_t len1 = 11; 
     for (size_t i = 12; i < data_send[5] + 6; i++) {
     sprintf(buf1, "%02X", data_send[i]);
     res1 += buf1;
     res1 += ":";
     }
  
    if (connected_) {
    client_->write(reinterpret_cast<const char*>(data_send.data()), sizeof(data_send)); 
    ESP_LOGD(TAG, ">>> %02X%02X %02X%02X %02X%02X %02X %02X %02X%02X %02X%02X %s",
                   data_send[0], data_send[1],  data_send[2], data_send[3], data_send[4], data_send[5],
                   data_send[6], data_send[7],  data_send[8], data_send[9], data_send[10], data_send[11], res1.c_str());
    }
  }
waiting_for_response_ = address;
last_send_ = millis();

}
// Helper function for lambdas
// Send raw command. Except CRC everything must be contained in payload


void Modbus::send_raw(const std::vector<uint8_t> &payload) {
  if (payload.empty()) {
    return;
  }
  this->client_->write(reinterpret_cast<const char*>(payload.data()), sizeof(payload));
  //this->client_->clear();
  waiting_for_response_ = payload[0];
  ESP_LOGV(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
  last_send_ = millis();
}


}  // namespace esphome::modbus
