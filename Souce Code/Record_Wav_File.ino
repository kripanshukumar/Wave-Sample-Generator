#include "Variables.h"
#include "LocalTime.h"
#include "WavRecorder.h"
#include "I2S_SAMPLER.h"
#include "Storage.h"
#include "LED.h"

int16_t buff[1000];

int16_t dat = 0xF4AC;


//uint8_t buff2[44100];
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  initTime();
  initStorage();
  I2S_INIT();
  create_led_task();
  create_wav_task();

}

void loop() {
 //Write code to run in loop
}
