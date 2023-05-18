#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


const int analogPin0 = A0; // номер аналогового 
const int analogPin1 = A1;
const int analogPin2 = A2;
const int analogPin3 = A3;
DHT_Unified dht1(analogPin2, DHT11);
DHT_Unified dht2(analogPin3, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t event1;
  sensors_event_t event2;
  dht1.temperature().getEvent(&event1);
  dht2.humidity().getEvent(&event2);


  int valueA0 = analogRead(analogPin0); // Считывание значения с пина A0
  int valueA1 = analogRead(analogPin1); // Считывание значения с пина A1
  int valueA2 = event1.temperature; // Считывание значения с пина A1
  int valueA3 = event2.relative_humidity; // Считывание значения с пина A1
  
  // Отправка значений на ESP32-CAM
  Serial.print("A0 = ");
  Serial.print(valueA0);
  Serial.print(", ");
  Serial.print("A1 = ");
  Serial.print(valueA1);
  Serial.print(", ");
  Serial.print("A2 = ");
  Serial.print(valueA2);
  Serial.print(", ");
  Serial.print("A3 = ");
  Serial.println(valueA3);
  
  delay(1000);
}
