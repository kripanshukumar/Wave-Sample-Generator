//===================================================================================================================================

void printLocalTime()
{
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    ESP.restart();
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//===================================================================================================================================

void initTime(){
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

}

//===================================================================================================================================

//tm_sec
//Seconds, between 0 and 60 inclusive (60 allows for leap seconds).
//
//tm_min
//Minutes, between 0 and 59 inclusive.
//
//tm_hour
//Hours, between 0 and 23 inclusive.
//
//tm_mday
//Day of the month, between 1 and 31 inclusive.
//
//tm_mon
//Month, between 0 (January) and 11 (December).
//
//tm_year
//Year (since 1900), can be negative for earlier years.
//
//tm_wday
//Day of week, between 0 (Sunday) and 6 (Saturday).
//
//tm_yday
//Number of days elapsed since last January 1, between 0 and 365 inclusive.
//
//tm_isdst
