
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiConnection.h"
#include "Time.h"
//#include <HardwareSerial.h>

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
const String IP = "192.168.0.102";
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
      sensorWithVibration.setMeasure(values[4]);

      try {
        requestSender.sendSensorData(sensorWithSound, CREATE_MEASURE, token);
        requestSender.sendSensorData(sensorWithWaterLevel, CREATE_MEASURE, token);
        requestSender.sendSensorData(sensorWithTemperature, CREATE_MEASURE, token);
        requestSender.sendSensorData(sensorWithHumidity, CREATE_MEASURE, token);
        requestSender.sendSensorData(sensorWithVibration, CREATE_MEASURE, token);
      } catch(const ForbiddenException& e) {
        Serial.println("Caught ForbiddenException: " + String(e.what()));
        requestSender.sendAuthoRequest(user, AUTH, token);
      }

      if(sdStorageService.getInit()) {
        // sizeof return mas of bytes / size of one object
        sdStorageService.writeData(sensors, (sizeof(sensors) / sizeof(sensors[0])));
      }
    }
    delay(300);
}


