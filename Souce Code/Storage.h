//===================================================================================================================================
void write_wav_file(void *parameters) {
  while (1) {
    if (digitalRead(BUTTON) == 0) {
      File file = SD.open("/audio.tmp", FILE_WRITE);
      if (file) {
        state = RECORDING;
        Serial.println("Recording Started...");
        //for (int i = 0; i < 600; i++) {
        int lineEnd = 0;
        while (digitalRead(BUTTON) == 0) {
          Serial.print('.');
          if (lineEnd >= 50) {
            lineEnd = 0;
            Serial.println();
          }
          while (writeEnable == 0) {
            delay(1);
          }
          if (writeEnable == 1) {
            writeEnable = 0;
            //Serial.println("Writing");
            if (!file.write((uint8_t*)buffer1, sizeof(int16_t) * 1000)) {
              Serial.println("Write Failed....");
            }
            if (writeEnable == 1) {
              Serial.println("The sampling speed is greater than writing speed!!!");
            }
          }
        }
        Serial.println();
      }
      else {
        Serial.println("Failed to open the file");
      }
      file.close();

      delay(100);
      file = SD.open("/audio.tmp", FILE_READ);
      if (file) {
        int fileSize = file.size();
        Serial.print("Size of previous file: "); Serial.println(fileSize);
        getLocalTime(&timeinfo);
        filename = '/' +String(timeinfo.tm_hour) + '_' + String(timeinfo.tm_min) + '_' + String(timeinfo.tm_sec) +".wav";
        Serial.println("Creating Audio File with name: " + filename);
        File file2 = SD.open(filename.c_str(), FILE_WRITE);
        state = PROCESSING;
        if (file2) {
          wavHeader(header, fileSize);
          if (!file2.write(header, 44)) {
            Serial.println("Write failed...");
          }
          while (fileSize >= sizeof(int16_t) * 1000) {
            if (!file.read((uint8_t*)tempBuff, sizeof(int16_t) * 1000)) {
              Serial.println("Reading failed!!");
            }
            delay(2);
            if (!file2.write((uint8_t*)tempBuff, sizeof(int16_t) * 1000)) {
              Serial.println("Write to file2 failed!!");
            }
            delay(2);
            fileSize -= sizeof(int16_t) * 1000;
          }
          if (fileSize > 0) {
            if (!file.read((uint8_t*)tempBuff, fileSize)) {
              Serial.println("Reading failed!!");
            }
            if (!file2.write((uint8_t*)tempBuff, fileSize)) {
              Serial.println("Write to file2 failed!!");
            }
            Serial.print("Done with remaining file size: "); Serial.println(fileSize);
          }
        }
        else {
          Serial.println("Failed to opn the file for writing!!!");
        }
        file2.close();
      }
      else {
        Serial.println("Failed to open the file for writing!!");
      }
      file.close();
    }
    state = READY;
    delay(2);
    //Serial.println("Exiting the Function...");
  }
}

//===================================================================================================================================

void listDir(const char * dirname, uint8_t levels) {
  Serial.println("===========================Contents of Internal Storage================================");
  Serial.printf("Listing directory: %s\n", dirname);

  File root = SD.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.print (file.name());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
      if (levels) {
        listDir(file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.print(file.size());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
    }
    file = root.openNextFile();
  }
  file.close();
  Serial.println("=======================================================================================");
}

//===========================================================================================================================================//

void initStorage() {
  pinMode(4, INPUT_PULLUP);
  spiSD.begin(14, 4, 13, SD_CS); //CLK,MISO,MOIS,SS
  if (!SD.begin(SD_CS, spiSD))
  {
    Serial.println("Card Mount Failed");
  }
  else
  {
    Serial.println("Card Mount Successful");
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  delay(100);
  listDir("/", 0);
}

void create_wav_task() {
  // Create a task that will read the data
  xTaskCreatePinnedToCore (
    write_wav_file, /* Task function. */
    "ADC_reader", /* String with name of task. */
    50000, /* Stack size in bytes. */
    NULL, /* Parameter passed as input of the task */
    configMAX_PRIORITIES - 2, /* Priority of the task. */
    NULL, /* Task handle. */
    1);
}

//===========================================================================================================================================//
