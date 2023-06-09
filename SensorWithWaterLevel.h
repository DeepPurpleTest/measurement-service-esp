#include "Sensor.h"

class SensorWithWaterLevel : public Sensor {
  public:
  float measure;

  public:
  SensorWithWaterLevel(int id) : Sensor(id) {}

public:
  float getMeasure() override {
    return map(measure, 0, 300, 1, 100);;
  }

public: 
  void setMeasure(float measure) override {
    this->measure = measure;
  }
};