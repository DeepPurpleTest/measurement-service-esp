#include "Sensor.h"

class SensorWithSound : public Sensor {
  public:
  float measure;

  public:
  SensorWithSound(int id) : Sensor(id) {}

public:
  float getMeasure() override {
    return measure;
  }

public: 
  void setMeasure(float measure) override {
    this->measure = measure;
  }
};