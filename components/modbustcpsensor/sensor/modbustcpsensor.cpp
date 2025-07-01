#include "modbustcpsensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace modbustcpsensor {

static const char *const TAG = "Modbus-TCP-Sensor";


void ModbusTCPSensor::update() {
this->send(functioncode_, address_);
}

void ModbusTCPSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Address: %d", this->address_);
}

}  // namespace modbustcpsensor
}  // namespace esphome