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
      self->handle_message(reinterpret_cast<uint8_t*>(data), len);
    }, this);
  }
  

      void ModbusTCP::connect() {
    if (!client_->connecting()) {
      if (!client_->connect(host_.c_str(), port_)) {
        ESP_LOGW("tcp", "Connection failed, will retry...");
      }
    }
  }

  void ModbusTCP::send_message(const uint8_t *send_byte) {
    if (connected_ && client_->canSend()) {
      //client_->write(message.c_str());
      //client_->write(reinterpret_cast<const char*>(frame.data.get()), sizeof(frame.data));

    
   //for (size_t i = 0; i < sizeof(send_byte); i++) {
    //sprintf(buf1, "%02X", send_byte[i]);

      client_->write(reinterpret_cast<const char*>(send_byte), (send_byte[5] + 6));
      
        
     std::string res1;
     char buf1[5];
      
     //for (size_t i = 12; i < send_byte[5] + 6; i++) {
     for (size_t i = 12; i < send_byte[5] + 6; i++) {
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

  // Appends the freshly-received bytes to rx_buffer_ and hands off to process_buffer_() to pull
  // out however many complete frames are now available. A single onData call is not guaranteed
  // to line up with frame boundaries — TCP can fragment a frame across several calls, or coalesce
  // more than one frame (e.g. from a busy Modbus TCP proxy) into a single call.
  void ModbusTCP::handle_message(const uint8_t *data, size_t len) {
    if (len == 0) {
      return;
    }
    rx_buffer_.insert(rx_buffer_.end(), data, data + len);
    process_buffer_();
  }

  // Extracts and dispatches every complete Modbus TCP ADU currently sitting in rx_buffer_,
  // leaving any trailing partial frame in the buffer for the next call.
  void ModbusTCP::process_buffer_() {
    static const size_t MBAP_HEADER_LEN = 7;  // transaction id(2) + protocol id(2) + length(2) + unit id(1)
    static const size_t MAX_FRAME_LEN = 260;  // Modbus TCP ADUs are capped well below this

    while (true) {
      if (rx_buffer_.size() < MBAP_HEADER_LEN) {
        return;  // not enough for a header yet
      }

      uint16_t length_field = (uint16_t(rx_buffer_[4]) << 8) | rx_buffer_[5];
      size_t frame_len = 6 + length_field;  // total ADU length, per the MBAP length field

      if (length_field == 0 || frame_len > MAX_FRAME_LEN) {
        ESP_LOGW(TAG, "Implausible Modbus TCP frame length %zu, discarding %zu buffered bytes", frame_len,
                 rx_buffer_.size());
        rx_buffer_.clear();
        return;
      }

      if (rx_buffer_.size() < frame_len) {
        return;  // frame not fully received yet
      }

      handle_frame_(rx_buffer_.data(), frame_len);
      rx_buffer_.erase(rx_buffer_.begin(), rx_buffer_.begin() + frame_len);
    }
  }

  void ModbusTCP::handle_frame_(const uint8_t *byte, size_t frame_len) {
    uint16_t transaction_id = (uint16_t(byte[0]) << 8) | byte[1];

    std::string res;
    char buf[5];
    for (size_t i = 8; i < frame_len; i++) {
      sprintf(buf, "%02X", byte[i]);
      res += buf;
      res += ":";
    }

    ESP_LOGD("modbus_tcp", " <<< %02X%02X %02X%02X %02X%02X %02X %02X %s ", byte[0], byte[1], byte[2], byte[3],
             byte[4], byte[5], byte[6], byte[7], res.c_str());

    if (transaction_id != expected_transaction_id_) {
      // Late/stray response (e.g. arrived after the controller already gave up and moved on to
      // the next command) or a response meant for another client multiplexed through a proxy.
      // Decoding it as if it belonged to the current pending command would silently hand
      // garbage values to whichever sensor is currently waiting. Drop it instead.
      ESP_LOGW(TAG, "Ignoring Modbus response with unexpected transaction id 0x%04X (expected 0x%04X)",
               transaction_id, expected_transaction_id_);
      return;
    }

    if ((byte[7] & 0x80) == 0x80) {
      ESP_LOGE(TAG, "Error: ");
      if (frame_len > 8) {
        switch (byte[8]) {
          case 0x01: ESP_LOGE(TAG, "Failure Code 0x01 ILLEGAL FUNCTION"); break;
          case 0x02: ESP_LOGE(TAG, "Failure Code 0x02 ILLEGAL DATA ADDRESS"); break;
          case 0x03: ESP_LOGE(TAG, "Failure Code 0x03 ILLEGAL DATA VALUE"); break;
          case 0x04: ESP_LOGE(TAG, "Failure Code 0x04 SERVER FAILURE"); break;
          case 0x05: ESP_LOGE(TAG, "Failure Code 0x05 ACKNOWLEDGE"); break;
          case 0x06: ESP_LOGE(TAG, "Failure Code 0x06 SERVER BUSY"); break;
        }
      }
      return;
    }

    if (frame_len < 9) {
      ESP_LOGW(TAG, "Modbus response too short to contain a byte count (%zu bytes), discarding", frame_len);
      return;
    }

    size_t data_len = byte[8];
    if (9 + data_len > frame_len) {
      ESP_LOGW(TAG, "Modbus response byte count (%zu) exceeds frame length (%zu), discarding", data_len, frame_len);
      return;
    }

    std::vector<uint8_t> data(byte + 9, byte + 9 + data_len);
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

float ModbusTCP::get_setup_priority() const { return setup_priority::AFTER_WIFI - 1.0f; }

void ModbusTCP::send(uint8_t address, uint8_t function_code, uint16_t start_address, uint16_t number_of_entities, uint8_t payload_len, const uint8_t *payload) {
  static const size_t MAX_VALUES = 128;
 const uint32_t now = App.get_loop_component_start_time();
  // Only check max number of registers for standard function codes
  // Some devices use non standard codes like 0x43
  if (number_of_entities > MAX_VALUES && function_code <= 0x10) {
    ESP_LOGE(TAG, "send too many values %d max=%zu", number_of_entities, MAX_VALUES);
    return;
  }
    
  size_t MAX_FRAME_SIZE = 256;
  uint8_t data[MAX_FRAME_SIZE];
  size_t pos = 0;
      data[pos++] = Transaction_Identifier >> 8;
      data[pos++] = Transaction_Identifier >> 0;
      data[pos++] = 0x00;
      data[pos++] = 0x00;
      data[pos++] = 0x00;
        if (payload != nullptr) { 
          data[pos++] = (0x04 + payload_len);
        }else {
        data[pos++] = 0x06;      // how many bytes next comes for request
      }
      data[pos++] = address;
      data[pos++] = function_code;
      data[pos++] = start_address >> 8;
      data[pos++] = start_address >> 0;

      if (function_code != 0x05 && function_code != 0x06) {
      data[pos++] = number_of_entities >> 8;
      data[pos++] = number_of_entities >> 0;
      }
   if (payload != nullptr) {
     if (function_code == 0x10 || function_code == 0x0f) {           // Write multiple
      data[pos++] = payload_len;                                     // Byte count is required for write
    } else {
      payload_len = 2;  // Write single register or coil
    }

    if (payload_len + pos > MAX_FRAME_SIZE) {  // Check if send more as 256 bytes
      ESP_LOGE(TAG, "Payload too large to send: %d bytes", payload_len);
      return;
    }
    for (int i = 0; i < payload_len; i++) {
      data[pos++] = payload[i];
    }
  }
       
       
       expected_transaction_id_ = Transaction_Identifier;
       Transaction_Identifier++;
 send_message(data);

waiting_for_response = address;
last_send_ = millis();
}


void ModbusTCP::send_raw(const std::vector<uint8_t> &payload) {
  if (payload.empty()) {
    return;
  }

 // this->write_array(payload);
  client_->write(reinterpret_cast<const char*>(payload.data()), payload.size());
  //this->client.clear();
  waiting_for_response = payload[0];
  ESP_LOGV(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
  last_send_ = millis();
}


}  // namespace esphome::modbustcp
