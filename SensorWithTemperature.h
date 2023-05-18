#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorWithTemperature : public Sensor {
public:
    float measure;

public:
    SensorWithTemperature(int id) : Sensor(id) {
    }

public:
    float getMeasure() override {
      return this->measure;
    }

    void setMeasure(float measure) {
      this->measure = measure;
    }
};