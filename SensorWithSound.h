#include "Sensor.h"

class SensorWithSound : public Sensor {
  public:
  float measure;

  public:
  SensorWithSound(int id) : Sensor(id) {}

public:
  float getMeasure() override {
    return map(measure, 0, 1023, 30, 90);
  }

public: 
  void setMeasure(float measure) override {
    this->measure = measure;
  }
};