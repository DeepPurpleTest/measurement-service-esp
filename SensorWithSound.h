#include "Sensor.h"

class SensorWithSound : public Sensor {
  public:
  int analogInPin;

  public:
  SensorWithSound(int id, int pin) : Sensor(id), analogInPin(pin) {}

public:
  int getPin() {
    return this->analogInPin;
  }

public:
  float measurements() override {
    return analogRead(this->analogInPin);
  }
};