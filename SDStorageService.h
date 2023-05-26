#ifndef SD_CARD_WORKER_H
#define SD_CARD_WORKER_H
#include "SD_MMC.h"
#include "Sensor.h"
#include <string>

class SDStorageService {
  private: 
    bool init = false;
    String fileName;

  public:
    SDStorageService(String &fileName) : fileName(fileName) {}
  
  public:
    void initialize() {
      if (!SD_MMC.begin()) {
        Serial.println("SD Card initialization error");
        return;
      }

      uint8_t cardType = SD_MMC.cardType();
      if(cardType == CARD_NONE){
        Serial.println("No SD Card attached");
        return;
      }

      init = true;
    }

    void writeData(Sensor* sensors[], unsigned int numSensors) {
      String currentTime = this->getTime();
      // Open file for writing
      File file = SD_MMC.open("/data.txt", FILE_APPEND);

      if (file) {
        // write to file
        String data = currentTime;
        for(int i = 0; i < numSensors; i++) {
          data = data + " id " + sensors[i]->getId() + ", value = " + sensors[i]->getMeasure() + "; ";
        }
        file.println(data);

        // close file
        file.close();
        Serial.println("Success writing to the file");
      } else {
      Serial.println("Error writing to the file");
      }
    }

    bool getInit() {
      return init;
    }

    String getTime() {
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return "";
      }

      char buffer[80];
      strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);
      return String(buffer);
    }
};

#endif