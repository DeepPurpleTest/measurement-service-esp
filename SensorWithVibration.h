#include "Sensor.h"

class SensorWithVibration : public Sensor {
  public:
  float measure;

  public:
  SensorWithVibration(int id) : Sensor(id) {}

public:
  float getMeasure() override {
    return measure;
  }

public: 
  void setMeasure(float measure) override {
    this->measure = measure;
  }
};