#include <HTTPClient.h>
#include "UserEntity.h"
#include "Sensor.h"
#include "ForbiddenException.h"


class RequestSender {
  private: 
    RequestBuilder &requestBuilder;
    HTTPClient &http;
    
  public:
    RequestSender(RequestBuilder &requestBuilder, HTTPClient &http) : requestBuilder(requestBuilder), http(http){}

    void sendAuthoRequest(UserEntity user, String endpoint, String &token) {
        Serial.println("sendAuthoRequest() start");

        String requestBody;
        StaticJsonDocument<200> doc = requestBuilder.createJson(endpoint, token);
        doc["username"] = user.getLogin();                // key and value of our json
        doc["password"] = user.getPassword();

        serializeJson(doc, requestBody);                // serialization of json
        int httpResponseCode = http.POST(requestBody);

        Serial.println(httpResponseCode);

        if (httpResponseCode == 200) {            //if code > 0 (any, 200, 400, 404 etc.)
            String response = http.getString();  //so we have some answer and we can print our response
            Serial.println(response);

            DynamicJsonDocument jsonDoc = requestBuilder.getDynamicJsonDocument();
            DeserializationError error = deserializeJson(jsonDoc, response); //Deserialize the JSON response

            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            } else {
                const char* jsonToken = jsonDoc["token"]; //Extract value for token
                token = String(jsonToken);
                Serial.println(token); //Print token value
            }
        } else {
          String response = http.getString();
          Serial.println(response);
          sendAuthoRequest(user, endpoint, token);
          delay(5000);
        }

        Serial.println("sendAuthoRequest() end");
    }

    void sendSensorData(Sensor& sensor, String endpoint, String &token) {
        Serial.println("sendSensorData() start");

        String requestBody;
        StaticJsonDocument<200> doc = requestBuilder.createJson(endpoint, token);
        doc["sensorId"] = sensor.getId();
        doc["value"] = sensor.getMeasure();

        serializeJson(doc, requestBody);
        int httpResponseCode = http.POST(requestBody);

        Serial.println(requestBody);
        Serial.println(httpResponseCode);
        if(httpResponseCode == 403) {
          throw ForbiddenException();
        }

        if (httpResponseCode > 0) {            //if code > 0 (any, 200, 400, 404 etc.)
            String response = http.getString();  //so we have some answer and we can print our response
            Serial.println(response);
        }

        Serial.println("sendSensorData() end");
    }
};