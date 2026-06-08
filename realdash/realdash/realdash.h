#pragma once

#include <vector>

#include "esphome/core/component.h"
#include "esphome.h"
#include "esphome/components/uart/uart.h"


namespace esphome::realdash {
 
 class RealdashDevice;

class Realdash : public uart::UARTDevice, public Component {
 public:
 
 Realdash() = default;

void register_device(RealdashDevice *device) { this->devices_.push_back(device); }
void setup() override;
void loop() override;
void dump_config() override;
void send44(uint32_t canid_, float data2, float data1);
//void send44(uint32_t canid_, float data2 = 0, float data1 = 0);
//void send66(uint32_t canid_);
void send66(uint32_t canid_, uint8_t size_identifier, const uint8_t *payload);
void send_text(uint32_t canid_);
//void read44();
//void read66();
void send_data(const uint8_t *data, uint8_t len);
void can_speed(uint8_t can_speed);
void can_mode(uint8_t can_mode);

//float get_setup_priority() const override;


 private:
  uint32_t canid_;

 protected:
  //uint32_t canid_;
   //uint32_t serialBlockTag_44 = { 0x11223344 };
   //uint32_t serialBlockTag_66 = { 0x00223366 };
   //uint32_t serialBlockTag_text = { 0x11223355};
   //uint32_t nodata = { 0x00000000 };
   std::vector<RealdashDevice *> devices_;
  
    
};

class RealdashDevice {
 public:
  void set_parent(Realdash *parent) { parent_ = parent; }
  //void send44(uint32_t canid_, float data2 = 0, float data1 = 0) {
  //  this->parent_->send44(canid_, data2, data1);
  //}
  //void send_text();
  //void read44();
  //void send_raw();
 protected:
  friend Realdash;
  Realdash *parent_;
};

}  // namespace esphome::realdash


