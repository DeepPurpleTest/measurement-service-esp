
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiConnection.h"
#include "Time.h"

#include "SDStorageService.h"
#include "UserEntity.h"
#include "RequestBuilder.h"
#include "RequestSender.h"
#include "Sensor.h"
#include "SensorWithTemperature.h"
#include "SensorWithHumidity.h"
#include "SensorWithSound.h"
#include "SensorWithWaterLevel.h"
#include "SensorWithVibration.h"
#include "ForbiddenException.h"

#include <regex>

using namespace std;

HTTPClient http;

// Connection for Wifi
const char* SSID = "TP-Link_BE7A";
const char* PASSWORD = "456123mrVSDzxczaebal";

// Service to take curren time of EET timezone
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 60 * 60;
const int   daylightOffset_sec = 3600;

// Creat URL for requests
const String IP = "192.168.0.103";
const String SERVER_PORT = "8080";
const String URL = "https://" + IP + ":" + SERVER_PORT;

// Endpoints
const String AUTH = "/auth/_login";
const String CREATE_MEASURE = "/measures";

// Token for authentication
String token;
String fileName = "/data.txt";


WiFiConnection connection(SSID, PASSWORD);
SDStorageService sdStorageService(fileName);
UserEntity user("admin", "admin");
RequestBuilder requestBuilder(http, URL);
RequestSender requestSender(requestBuilder, http);

SensorWithSound sensorWithSound(1); //Sensor with sound
SensorWithWaterLevel sensorWithWaterLevel(2); //Sensor with water level 
SensorWithTemperature sensorWithTemperature(3); //Sensor with temperature
SensorWithHumidity sensorWithHumidity(4); //Sensor with humidity
SensorWithVibration sensorWithVibration(5); //Sensor with water level

Sensor* sensors[] = {&sensorWithSound, &sensorWithWaterLevel, &sensorWithTemperature, &sensorWithHumidity, &sensorWithVibration};


const int RXp2 = 3; // RX pin for ESP32-CAM
const int TXp2 = 1; // TX pin for ESP32-CAM


vector<int> getValuesFromString(String data);
void fillSensorsMeasure(vector<int> values);


void setup() {
  Serial.begin(9600, SERIAL_8N1, RXp2, TXp2);
  connection.getWiFiConnection();
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  sdStorageService.initialize();
  requestSender.sendAuthoRequest(user, AUTH, token);
}

void loop() {
    if (Serial.available()) {
      String dataString = Serial.readStringUntil('\n');  // Reading a data string up to a newline character
      
      vector<int> values = getValuesFromString(dataString);
      fillSensorsMeasure(values);

      if(sdStorageService.getInit()) {
        // sizeof return mas of bytes / size of one object
        sdStorageService.writeData(sensors, (sizeof(sensors) / sizeof(sensors[0])));
      }

      try {
        for(int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); i++) {
          requestSender.sendSensorData(*sensors[i], CREATE_MEASURE, token);
        }
      } catch(const ForbiddenException& e) {
        Serial.println("Caught ForbiddenException: " + String(e.what()));
        requestSender.sendAuthoRequest(user, AUTH, token);
      }  
    }
    delay(300);
}

vector<int> getValuesFromString(String data) {
  regex pattern("(A\\d+) = (\\d+)");
  smatch matches;
  string inputStr = data.c_str();
  vector<int> values;

  while (regex_search(inputStr, matches, pattern)) {
    if (matches.size() > 2) {
      string valueStr = matches[2].str();
      int value = atoi(valueStr.c_str());
      values.push_back(value);
    }
    inputStr = matches.suffix().str();
  }
  return values;
}

void fillSensorsMeasure(vector<int> values) {
  for(int i = 0; i < values.size(); i++) {
    sensors[i]->setMeasure(values[i]);
  }
} 



