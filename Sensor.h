#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
    int id;

public:  //constructor
    Sensor(int id) : id(id) {
        Serial.println("Sensor() constructor start");
        Serial.println("Sensor() constructor end");
    }

public:
    int getId() {
        return id;
    }

public:
    virtual float getMeasure() = 0;
    virtual void setMeasure(float measure) = 0;
};

#endif