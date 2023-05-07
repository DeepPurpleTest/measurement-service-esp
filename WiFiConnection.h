#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WiFiConnection {
private:
    const char* ssid;
    const char* password;

public:
    WiFiConnection(const char* ssid, const char* password) : ssid(ssid), password(password) {}

public:
    void getWiFiConnection() {
        Serial.println("getWiFiConnection() start");

        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.println("Connecting...");
            delay(500);
        }

        Serial.println("Connected!!");
        Serial.println("getWiFiConnection() end");
    }
};

#endif