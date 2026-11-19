#pragma once
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include <vector>

namespace esphome { namespace saia_sbus {
class SaiaSBus : public PollingComponent, public uart::UARTDevice {
 public:
  void set_direction_pin(GPIOPin *pin) { direction_ = pin; }
  void set_address(uint8_t address) { address_ = address; }
  void set_energy(sensor::Sensor *value) { energy_ = value; }
  void set_power(sensor::Sensor *value) { power_ = value; }
  void set_voltage(sensor::Sensor *value) { voltage_ = value; }
  void set_current(sensor::Sensor *value) { current_ = value; }
  void set_partial_energy(sensor::Sensor *value) { partial_energy_ = value; }
  void set_reactive_power(sensor::Sensor *value) { reactive_power_ = value; }
  void set_power_factor(sensor::Sensor *value) { power_factor_ = value; }
  void setup() override;
  void update() override;
  void loop() override;
 protected:
  static uint16_t crc(const std::vector<uint8_t> &data, size_t length);
  void finish_();
  GPIOPin *direction_;
  sensor::Sensor *energy_, *power_, *voltage_, *current_;
  sensor::Sensor *partial_energy_{nullptr}, *reactive_power_{nullptr}, *power_factor_{nullptr};
  uint8_t address_{1};
  bool pending_{false}, escaped_{false};
  uint32_t boot_{0}, sent_{0}, last_rx_{0};
  std::vector<uint8_t> rx_;
};
}}
