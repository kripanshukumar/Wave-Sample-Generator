//=====================================================================================================================================

void i2s_install() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 1000,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

//=====================================================================================================================================

void i2s_setpin() {
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

//=====================================================================================================================================

void reader(void *pvParameters) {

  size_t bytes_read;
  //uint32_t t = millis();
  //int i = 0;
  while (1) {
    i2s_read(I2S_NUM_0, dataBuffer, sizeof(int32_t) * 1000, &bytes_read, portMAX_DELAY);
    if (bytes_read > 0) {
      int16_t temp = 0;
      for (int i = 0; i < 1000; i++) {
        temp = dataBuffer[i]/65536;
        buffer1[2 * i] = lowByte(temp);
        buffer1[2 * i + 1] = highByte(temp);
        //Serial.println(temp);
      }
      writeEnable = 1;
    }
  }
}

//=====================================================================================================================================

void I2S_INIT() {
  Serial.println("Setup I2S ...");

  delay(500);
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);
  delay(500);

  // Create a task that will read the data
  xTaskCreatePinnedToCore (
    reader, /* Task function. */
    "ADC_reader", /* String with name of task. */
    20480, /* Stack size in bytes. */
    NULL, /* Parameter passed as input of the task */
    configMAX_PRIORITIES -2, /* Priority of the task. */
    NULL, /* Task handle. */
    1);
}

//=====================================================================================================================================
