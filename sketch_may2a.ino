
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiConnection.h"
#include <HardwareSerial.h>
//#include <SoftwareSerial.h> // include the SoftwareSerial library

#include "UserEntity.h"
#include "RequestBuilder.h"
#include "RequestSender.h"
#include "Sensor.h"
#include "SensorWithTemperature.h"
#include "SensorWithHumidity.h"
#include "SensorWithSound.h"
#include "SensorWithWaterLevel.h"

#include <regex>

using namespace std;

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
SensorWithTemperature sensorWithTemperature(1); //Sensor with temperature
SensorWithHumidity sensorWithHumidity(2); //Sensor with humidity
SensorWithSound sensorWithSound(3); //Sensor with sound
SensorWithWaterLevel sensorWithWaterLevel(4); //Sensor with water level 


const int RXp2 = 3; // RX pin for ESP32-CAM
const int TXp2 = 1; // TX pin for ESP32-CAM

void setup() {
  Serial.begin(9600, SERIAL_8N1, RXp2, TXp2);
  connection.getWiFiConnection();
  requestSender.sendAuthoRequest(user, AUTH, token);
}

void loop() {
  if(!token.isEmpty()) {
    if (Serial.available()) {
      String dataString = Serial.readStringUntil('\n');  // Чтение строки данных до символа новой строки

      regex pattern("(A\\d+) = (\\d+)");
      smatch matches;
      string inputStr = dataString.c_str();

      vector<int> values;

      while (regex_search(inputStr, matches, pattern)) {
        if (matches.size() > 2) {
          string valueStr = matches[2].str();
          int value = atoi(valueStr.c_str());

          values.push_back(value);
        }
  
        inputStr = matches.suffix().str();
      }

      sensorWithSound.setMeasure(values[0]);
      sensorWithWaterLevel.setMeasure(values[1]);
      sensorWithTemperature.setMeasure(values[2]);
      sensorWithHumidity.setMeasure(values[3]);

      requestSender.sendSensorData(sensorWithSound, CREATE_MEASURE, token);
      requestSender.sendSensorData(sensorWithWaterLevel, CREATE_MEASURE, token);
      requestSender.sendSensorData(sensorWithTemperature, CREATE_MEASURE, token);
      requestSender.sendSensorData(sensorWithHumidity, CREATE_MEASURE, token);
    }
    delay(500);
  }
  else {
    requestSender.sendAuthoRequest(user, AUTH, token);
  }
}


