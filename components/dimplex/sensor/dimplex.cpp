#include "dimplex.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dimplex {

static const char *const TAG = "Dimplex-Sensor-Modbus";


void DimplexTCP::dump_config() {
//  ESP_LOGCONFIG(TAG, "Sensor");
  ESP_LOGCONFIG(TAG, "Address: %d", this->register_address_);
}

}  // namespace dimplex
}  // namespace esphome
