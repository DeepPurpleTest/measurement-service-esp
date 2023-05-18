#include "Sensor.h"

class SensorWithWaterLevel : public Sensor {
  public:
  float measure;

  public:
  SensorWithWaterLevel(int id) : Sensor(id) {}

public:
  float getMeasure() override {
    return measure;
  }

public: 
  void setMeasure(float measure) override {
    this->measure = measure;
  }
};