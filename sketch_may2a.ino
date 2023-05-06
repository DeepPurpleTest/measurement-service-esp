#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

HTTPClient http;
StaticJsonDocument<200> doc;  //creating json for POST method
DynamicJsonDocument jsonDoc(1024); //Create dynamic JSON buffer
String requestBody;

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

// Header parameters
const String CONTENT = "Content-Type";
const String TYPE_JSON = "application/json";
const String AUTHORIZATION = "Authorization";

// Token for authentication
String token;
// event for sensor data
//sensors_event_t event;

// sensor id from db
//long sensorId;
//double value;

void getWiFiConnection();
void sendAuthoRequest();
void createJson(String endPoint);
void sendSensorData();

class Sensor {
public:
  int id;

public:  //constructor
  Sensor(int id) : id(id) {
    Serial.println("Sensor() старт");
    Serial.println("Sensor() конец");
  }

public:
  int getId() {
    return id;
  }

public:
  virtual float measurements() = 0;
};

class SensorWithTemperature : public Sensor {
  public:
  DHT_Unified dht;
  
  public:
  SensorWithTemperature(int id, int pin, int type) : Sensor(id), dht(pin, type) {
    this->dht.begin();
  }

public:
  float measurements() override {
    Serial.println("SensorWithTemperature.measurements() стартанул");

    //this->dht.begin();
    sensors_event_t event;
    this->dht.temperature().getEvent(&event);
    
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
      return this->measurements();
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
      
      Serial.println("SensorWithTemperature.measurements() закончил");
      return event.temperature;
    }
  }
};

class SensorWithHumidity : public Sensor {
  public:
  DHT_Unified dht;
  
  public:
  SensorWithHumidity(int id, int pin, int type) : Sensor(id), dht(pin, type) {
    this->dht.begin();
  }

public:
  float measurements() override {
    Serial.println("SensorWithHumidity.measurements() стартанул");

    sensors_event_t event;
    this->dht.humidity().getEvent(&event);
    
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
      return this->measurements();
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));

      Serial.println("SensorWithHumidity.measurements() закончил");
      return event.relative_humidity;
    }
  }
};

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

SensorWithTemperature sensorWithTemperature(1, 12, DHT11); //Sensor with temperature object
SensorWithHumidity sensorWithHumidity(2, 12, DHT11); //Sensor with humidity object
//SensorWithSound sensorWithSound(3, 13);

void setup() {
  Serial.begin(115200);
  //Serial.println(sensorWithSound.getPin());
  //pinMode(sensorWithSound.getPin(), INPUT);
  getWiFiConnection();
  sendAuthoRequest();
}

void getWiFiConnection() {
  Serial.println("getWiFiConnection() стартанул");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Подключаемся...");
    delay(500);
  }

  Serial.println("Подключились!!");
  Serial.println("getWiFiConnection() закончил");
}

void sendAuthoRequest() {
  Serial.println("sendAuthoRequest() стартанул");

  createJson(AUTH);
  doc["username"] = "admin";                // key and value of our json
  doc["password"] = "admin";

  serializeJson(doc, requestBody);                // serialization of json
  int httpResponseCode = http.POST(requestBody); 

  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {            //if code > 0 (any, 200, 400, 404 etc.)
    String response = http.getString();  //so we have some answer and we can print our response
    Serial.println(httpResponseCode);
    Serial.println(response);
    
    DeserializationError error = deserializeJson(jsonDoc, response); //Deserialize the JSON response
    
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        const char* jsonToken = jsonDoc["token"]; //Extract value for token
        token = String(jsonToken);
        Serial.println(token); //Print token value
      }

    jsonDoc.clear();  
  }
  requestBody.clear();
  doc.clear();

  Serial.println("sendAuthoRequest() закончил");
}

void sendSensorData(Sensor &sensor) {
  Serial.println("sendSensorData() стартанул");

  createJson(CREATE_MEASURE);
  doc["sensorId"] = sensor.getId();
  doc["value"] = sensor.measurements();

  serializeJson(doc, requestBody);
  int httpResponseCode = http.POST(requestBody); 

  Serial.println(requestBody);
  if (httpResponseCode > 0) {            //if code > 0 (any, 200, 400, 404 etc.)
    String response = http.getString();  //so we have some answer and we can print our response
    Serial.println(httpResponseCode);
    Serial.println(response);
  }
  requestBody.clear();

  Serial.println("sendSensorData() закончил");
}

void createJson(String endPoint) {
  Serial.println("createJson(String endPoint) стартанул");

  http.begin(URL + endPoint);    //link to api via http
  http.addHeader(CONTENT, TYPE_JSON);
  http.addHeader(AUTHORIZATION, "Bearer " + token);

  Serial.println("createJson(String endPoint) закончил");
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
    
    sendSensorData(sensorWithTemperature);
    delay(2000);
    sendSensorData(sensorWithHumidity);
    
    //sendSensorData(sensorWithSound);
    delay(2000);
  } else {
    sendAuthoRequest();
  }
}


