
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiConnection.h"

#include "UserEntity.h"
#include "RequestBuilder.h"
#include "RequestSender.h"
#include "Sensor.h"
#include "SensorWithTemperature.h"
#include "SensorWithHumidity.h"
#include "SensorWithSound.h"

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

WiFiConnection connection(SSID, PASSWORD);
UserEntity user("admin", "admin");
RequestBuilder requestBuilder(http, URL);
RequestSender requestSender(requestBuilder, http);
SensorWithTemperature sensorWithTemperature(1, 12, DHT11); //Sensor with temperature object
SensorWithHumidity sensorWithHumidity(2, 12, DHT11); //Sensor with humidity object
//SensorWithSound sensorWithSound(3, 13); //Sensor with sound

void setup() {
  Serial.begin(115200);
  connection.getWiFiConnection();
  requestSender.sendAuthoRequest(user, AUTH, token);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!token.isEmpty()) {
    requestSender.sendSensorData(sensorWithTemperature, CREATE_MEASURE, token);
    delay(5000);
    requestSender.sendSensorData(sensorWithHumidity, CREATE_MEASURE, token);
    //sendSensorData(sensorWithSound);
    delay(10000);
  } else {
    requestSender.sendAuthoRequest(user, AUTH, token);
  }
}


