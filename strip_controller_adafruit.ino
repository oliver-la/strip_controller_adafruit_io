#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include "structs.h"
#include "config.h"
#include "functions.h"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *feed_color = io.feed(IO_GROUP ".color");
AdafruitIO_Feed *feed_white = io.feed(IO_GROUP ".white");
AdafruitIO_Feed *feed_brightness = io.feed(IO_GROUP ".brightness");

void setup() {
  Serial.begin(115200);
  delay(10);

  #ifdef ESP8266
    analogWriteRange(255);
  #endif
  
  pinMode(pin.r, OUTPUT);
  pinMode(pin.g, OUTPUT);
  pinMode(pin.b, OUTPUT);
  pinMode(pin.w, OUTPUT);

  change_to_rgbw_target((RGBW){0,0,0,0});

  //////////////
  // MQTT
  //////////////

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  feed_color->onMessage(handleColorMessage);
  feed_white->onMessage(handleWhiteMessage);
  feed_brightness->onMessage(handleBrightnessMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  feed_color->get();
  feed_white->get();
  feed_brightness->get();
  
  //////////////
  // OTA
  //////////////
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(OTA_IDENTIFIER);

  // No authentication by default
  ArduinoOTA.setPassword((const char *)OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void loop() {
  io.run();
}

void handleColorMessage(AdafruitIO_Data *data) {
  apply_effect_fade((RGBW){
      data->toRed(),
      data->toGreen(),
      data->toBlue(),
      current_value.w
  });
}

void handleWhiteMessage(AdafruitIO_Data *data) {
  apply_effect_fade((RGBW){
    current_value.r,
    current_value.g, 
    current_value.b,
    data->toInt()
  });
}

void handleBrightnessMessage(AdafruitIO_Data *data) {
  apply_effect_brightness_fade(data->toInt());
}
