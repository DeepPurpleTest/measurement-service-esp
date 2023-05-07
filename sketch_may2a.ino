
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiConnection.h"
#include "RequestBuilder.h"
#include "RequestSender.h"
#include "Sensor.h"
#include "SensorWithTemperature.h"
#include "SensorWithHumidity.h"
#include "UserEntity.h"

HTTPClient http;

// Connection for Wifi
const char* SSID = "TP-Link_BE7A";
const char* PASSWORD = "456123mrVSDzxczaebal";

// Creat URL for requests
const String IP = "192.168.0.102";
const String SERVER_PORT = "8080";
const String URL = "https://" + IP + ":" + SERVER_PORT;

// Endpoints
const String AUTH = "/auth/_login";
const String CREATE_MEASURE = "/measures";



// Token for authentication
String token;

  //creating json for POST method

// event for sensor data
//sensors_event_t event;

// sensor id from db
//long sensorId;
//double value;

/*
class SensorWithSound : public Sensor {
  public:
  int analogInPin;

  public:
  SensorWithSound(int id, int pin) : Sensor(id), analogInPin(pin) {}

public:
  int getPin() {
    return this->analogInPin;
  }

public:
  float measurements() override {
    Serial.print("PIN: ");
    Serial.println(this->analogInPin);
    Serial.print("MEASURE VALUE: ");
    Serial.println(analogRead(this->analogInPin));
    //return analogRead(this->analogInPin);
    return analogRead(this->analogInPin);
  }
};
*/

/*
#define DHTPIN 12
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
*/

WiFiConnection connection(SSID, PASSWORD);
UserEntity user("admin", "admin");
JsonMaker jsonMaker(http, URL);
RequestSender requestSender(jsonMaker, http);
SensorWithTemperature sensorWithTemperature(1, 12, DHT11); //Sensor with temperature object
SensorWithHumidity sensorWithHumidity(2, 12, DHT11); //Sensor with humidity object
//SensorWithSound sensorWithSound(3, 13);

void setup() {
  Serial.begin(115200);
  //Serial.println(sensorWithSound.getPin());
  //pinMode(sensorWithSound.getPin(), INPUT);
  connection.getWiFiConnection();
  requestSender.sendAuthoRequest(user, AUTH, token);
}

/*
void checkDht() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  //sensor.measurements();
}
*/

void loop() {
  // put your main code here, to run repeatedly:
  if(!token.isEmpty()) {
    //checkDht();
    requestSender.sendSensorData(sensorWithTemperature, CREATE_MEASURE, token);
    delay(5000);
    requestSender.sendSensorData(sensorWithHumidity, CREATE_MEASURE, token);
    //sendSensorData(sensorWithSound);
    delay(10000);
  } else {
    requestSender.sendAuthoRequest(user, AUTH, token);
  }
}


