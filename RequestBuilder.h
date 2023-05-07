#ifndef REQUEST_BUILDER_H
#define REQUEST_BUILDER_H

#include <HTTPClient.h>
#include <ArduinoJson.h>

class RequestBuilder {
  // Header parameters
  private:
    const String CONTENT = "Content-Type";
    const String TYPE_JSON = "application/json";
    const String AUTHORIZATION = "Authorization";
    HTTPClient &http;
    String url;

  public:
    RequestBuilder(HTTPClient &http, const String &url) : http(http), url(url) {
    }

  StaticJsonDocument<200> createJson(String endPoint, String &token) {
        Serial.println("createJson(String endPoint) стартанул");

        http.begin(url + endPoint);    //link to api via http
        http.addHeader(CONTENT, TYPE_JSON);
        http.addHeader(AUTHORIZATION, "Bearer " + token);

        Serial.println("createJson(String endPoint) закончил");
        StaticJsonDocument<200> doc;
        return doc;
    }

  DynamicJsonDocument getDynamicJsonDocument(){
    DynamicJsonDocument jsonDoc(1024); //Create dynamic JSON buffer
    return jsonDoc;
  }
};

#endif