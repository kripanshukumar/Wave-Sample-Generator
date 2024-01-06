//===================================================================================================================================
void led(int x){
  digitalWrite(LED_PIN, x);
}

//===================================================================================================================================

void runLED(void *Parameters){
  int loopCount = 0;
  int prevCount = 0;
  while(1){
    //Serial.print("STATE: ");Serial.println(state);
    switch(state){
      case READY:{
        led(0);
        break;
      }

      case RECORDING:{
        led(1);
        break;
      }

      case PROCESSING:{
        
        if((loopCount - prevCount  == 2)||(loopCount - prevCount == -19)){
          led(!digitalRead(LED_PIN));
          prevCount = loopCount;
        }
        break;
      }
    }
    loopCount++;
    if(loopCount > 20){
      loopCount = 0;
    }
    delay(50);
  }
}

//===================================================================================================================================

void create_led_task(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  // Create a task that will read the data
  xTaskCreatePinnedToCore (
    runLED, /* Task function. */
    "ADC_reader", /* String with name of task. */
    1024, /* Stack size in bytes. */
    NULL, /* Parameter passed as input of the task */
    1, /* Priority of the task. */
    NULL, /* Task handle. */
    1);
}

//===================================================================================================================================
