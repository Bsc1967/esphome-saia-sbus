#include "saia_sbus.h"
#include "esphome/core/log.h"
#include <cmath>

namespace esphome { namespace saia_sbus {
static const char *const TAG = "saia_sbus";
uint16_t SaiaSBus::crc(const std::vector<uint8_t> &data, size_t length) {
  uint16_t result = 0;
  for (size_t index = 0; index < length; ++index) {
    result ^= uint16_t(data[index]) << 8;
    for (uint8_t bit = 0; bit < 8; ++bit)
      result = (result & 0x8000) ? (result << 1) ^ 0x1021 : result << 1;
  }
  return result;
}
void SaiaSBus::setup() {
  direction_->setup();
  direction_->digital_write(false);
  boot_ = millis();
  check_uart_settings(38400, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}
void SaiaSBus::update() {
  if (pending_ || millis() - boot_ < 30000) return;
  uint8_t discard;
  while (available()) read_byte(&discard);
  std::vector<uint8_t> packet{0xB5, 0x00, address_, 0x06, 0x08, 0x00, 0x14};
  const uint16_t checksum = crc(packet, packet.size());
  packet.push_back(checksum >> 8);
  packet.push_back(checksum & 0xFF);
  std::vector<uint8_t> wire{0xB5};
  for (size_t index = 1; index < packet.size(); ++index) {
    const uint8_t value = packet[index];
    if (value == 0xB5 || value == 0xC5) {
      wire.push_back(0xC5);
      wire.push_back(value == 0xB5 ? 0 : 1);
    } else wire.push_back(value);
  }
  rx_.clear();
  escaped_ = false;
  direction_->digital_write(true);
  delayMicroseconds(20);
  write_array(wire);
  flush();
  direction_->digital_write(false);
  sent_ = millis();
  pending_ = true;
}
void SaiaSBus::finish_() {
  const size_t length = rx_.size();
  if (escaped_ || length < 4) return;
  const uint16_t received = (uint16_t(rx_[length - 2]) << 8) | rx_[length - 1];
  if (crc(rx_, length - 2) != received) {
    ESP_LOGW(TAG, "CRC error");
    return;
  }
  if (rx_[1] == 2) {
    ESP_LOGW(TAG, "Unexpected ACK/NAK");
    return;
  }
  if (rx_[1] != 1 || length != 40) return;
  auto value = [this](size_t index) -> uint32_t {
    size_t offset = 2 + 4 * index;
    return (uint32_t(rx_[offset]) << 24) | (uint32_t(rx_[offset + 1]) << 16) |
           (uint32_t(rx_[offset + 2]) << 8) | rx_[offset + 3];
  };
  energy_->publish_state(value(0) * 0.01f);
  voltage_->publish_state(value(4));
  current_->publish_state(value(5) * 0.1f);
  power_->publish_state(value(6) * 10.0f);
  if (partial_energy_ != nullptr) partial_energy_->publish_state(value(1) * 0.01f);
  if (reactive_power_ != nullptr) reactive_power_->publish_state(value(7) * 10.0f);
  if (power_factor_ != nullptr) power_factor_->publish_state(value(8) * 0.01f);
  pending_ = false;
  status_clear_warning();
}
void SaiaSBus::loop() {
  uint8_t value;
  while (available() && read_byte(&value)) {
    if (!pending_) continue;
    last_rx_ = millis();
    if (value == 0xB5) { rx_ = {value}; escaped_ = false; continue; }
    if (rx_.empty()) continue;
    if (escaped_) {
      escaped_ = false;
      if (value > 1) { rx_.clear(); continue; }
      value = value == 0 ? 0xB5 : 0xC5;
    } else if (value == 0xC5) { escaped_ = true; continue; }
    rx_.push_back(value);
    if (rx_.size() > 64) rx_.clear();
  }
  if (pending_ && !rx_.empty() && millis() - last_rx_ >= 10) {
    finish_();
    rx_.clear();
    escaped_ = false;
  }
  if (pending_ && millis() - sent_ >= 500) {
    pending_ = false;
    status_set_warning();
    ESP_LOGW(TAG, "No valid S-Bus reply (address %u)", address_);
    for (auto *sensor : {energy_, power_, voltage_, current_}) sensor->publish_state(NAN);
    for (auto *sensor : {partial_energy_, reactive_power_, power_factor_})
      if (sensor != nullptr) sensor->publish_state(NAN);
  }
}
}}
