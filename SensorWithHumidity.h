#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorWithHumidity : public Sensor {
public:
    float measure;

public:
    SensorWithHumidity(int id) : Sensor(id) {
    }

public:
    float getMeasure() override {
      return this->measure;
    }

    void setMeasure(float measure) {
      this->measure = measure;
    }
};