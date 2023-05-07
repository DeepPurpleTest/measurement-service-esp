#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorWithHumidity : public Sensor {
public:
    DHT_Unified dht;

public:
    SensorWithHumidity(int id, int pin, int type) : Sensor(id), dht(pin, type) {
        this->dht.begin();
    }

public:
    float measurements() override {
        Serial.println("SensorWithHumidity.measurements() start");

        sensors_event_t event;
        this->dht.humidity().getEvent(&event);

        if (isnan(event.relative_humidity)) {
            Serial.println(F("Error reading humidity!"));
            delay(5000);
            return this->measurements();
        }
        else {
            Serial.print(F("Humidity: "));
            Serial.print(event.relative_humidity);
            Serial.println(F("%"));

            Serial.println("SensorWithHumidity.measurements() end");
            return event.relative_humidity;
        }
    }
};