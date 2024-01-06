#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include <driver/i2s.h>

#define SD_CS 32
#define FORMAT_SPIFFS_IF_FAILED true

#define I2S_WS 15
#define I2S_SD 21
#define I2S_SCK 19

#define I2S_PORT I2S_NUM_0

#define LED_PIN 18
#define BUTTON  5

SPIClass spiSD(HSPI);
//=========================================Variables================================================

enum ledStat{READY, RECORDING, PROCESSING};
enum ledStat state = READY;

//WAVE HEADER
bool writeEnable = 0;
int SAMPLE_RATE = 32000;
int CHANNELS = 1;
int RESOLUTION = 16;
int BYTES_PER_FRAME = 0;
int BYTE_RATE = 0;
int BITS_PER_SAMPLE = 0;
int startRecording = 0;

//Storage
String filename = "";
uint8_t tempBuff[2000] = {0};

//Date and Time
const char* ssid       = "Alpha 4G";
const char* password   = "kissmyass";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 *5.5;
const int   daylightOffset_sec = 3600;

struct tm timeinfo;

//I2S Sampler
uint8_t buffer1[2000] = {0};
int32_t dataBuffer[1000] = {0};
