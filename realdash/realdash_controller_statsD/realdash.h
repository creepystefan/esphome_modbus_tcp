#pragma once

#include <vector>

#include "esphome/core/component.h"
#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/defines.h"

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

namespace esphome {
namespace realdash {

using sensor_type_t = enum { TYPE_SENSOR };

using sensors_t = struct {
  const char *name;
  sensor_type_t type;
  union {
#ifdef USE_SENSOR
    esphome::sensor::Sensor *sensor;
#endif
  };
};
 
class RealdashComponent : public uart::UARTDevice, public PollingComponent {
 public:
  RealdashComponent() : PollingComponent(50) {}
  ~RealdashComponent();

void set_canid(uint32_t canid) { this->canid_ = canid; }
void setup() override;
void update() override;
void dump_config() override;

//float get_setup_priority() const override;

#ifdef USE_SENSOR
  void register_sensor(const char *name, esphome::sensor::Sensor *sensor);
#endif

 private:
  std::vector<sensors_t> sensors_;
   uint32_t canid_;

 protected:
  //uint32_t canid_;
    
};

}  // namespace realdash
}  // namespace esphome

