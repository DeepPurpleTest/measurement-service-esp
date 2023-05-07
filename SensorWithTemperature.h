#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorWithTemperature : public Sensor {
public:
    DHT_Unified dht;

public:
    SensorWithTemperature(int id, int pin, int type) : Sensor(id), dht(pin, type) {
        this->dht.begin();
    }

public:
    float measurements() override {
        Serial.println("SensorWithTemperature.measurements() start");

        //this->dht.begin();
        sensors_event_t event;
        this->dht.temperature().getEvent(&event);

        if (isnan(event.temperature)) {
            Serial.println(F("Error reading temperature!"));
            delay(5000);
            return this->measurements();
        }
        else {
            Serial.print(F("Temperature: "));
            Serial.print(event.temperature);
            Serial.println(F("�C"));

            Serial.println("SensorWithTemperature.measurements() end");
            return event.temperature;
        }
    }
};