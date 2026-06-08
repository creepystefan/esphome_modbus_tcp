#include "esphome/core/log.h"
#include "realdash.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace realdash {

static const char *TAG = "Realdash";
const byte serialBlockTag[4] = { 0x44, 0x33, 0x22, 0x11 };
const byte nodata[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void RealdashComponent::setup() {
}

void RealdashComponent::update() {
  for (sensors_t s : this->sensors_) {
    double val = 0;
    switch (s.type) {
 #ifdef USE_SENSOR
      case TYPE_SENSOR:
        if (!s.sensor->has_state()) {
          continue;
        }
        val = s.sensor->state;
        break;
 #endif
  default:
        ESP_LOGE(TAG, "type not known, name:  type: %d", s.type);
       continue;
}
  uint16_t valuint16 = (uint16_t)val;
  uint8_t valbyte[2] = {
        (uint8_t)((valuint16 >> 8) & 0xFF),
        (uint8_t)(valuint16 & 0xFF)
        };
uint8_t canID8[4] = {
  
        (uint8_t)(canid_ &  0xFF),
        (uint8_t)((canid_ >> 8) & 0xFF),
        (uint8_t)((canid_ >> 16) & 0xFF),
        (uint8_t)((canid_ >> 24) & 0xFF)
        //(uint8_t)(canid_ &  0xFF)
};

  byte buf[6];
  memcpy(buf, &canID8, 4);
  memcpy(buf + 4, &valbyte, 2);
//memcpy(buf + 6, &data2_, 2);
//memcpy(buf + 8, 3, 2);
//memcpy(buf + 10, 4, 2);
//memcpy(buf + 10, 0 &data4_, 2);
    
  write_array(serialBlockTag, 4);
  write_array(buf, 6);
  write_array(nodata, 6);
  
     
ESP_LOGV("custom", "Frame44-ID: 0x%02X 0x%02X ", valbyte[0], valbyte[1]);

}
   delay(9);
}


#ifdef USE_SENSOR
void RealdashComponent::register_sensor(const char *name, esphome::sensor::Sensor *sensor) {
  sensors_t s;
  s.name = name;
  s.sensor = sensor;
  s.type = TYPE_SENSOR;
   
  this->sensors_.push_back(s);
}
#endif


void RealdashComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "Frame44 ");
 }


}  // namespace realdashdata
}  // namespace esphome