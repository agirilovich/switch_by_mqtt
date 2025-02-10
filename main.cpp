#include <Arduino.h>

#ifndef DEVICE_BOARD_NAME
#  define DEVICE_BOARD_NAME "MQTTswitch"
#endif

#define DEVICE_HOSTNAME "MQTTswitch"

#include "controlWiFi.h"

#include "MQTT_task.h"

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");

  Serial.print("Start WiFi on ");
  Serial.println(DEVICE_BOARD_NAME);

  initMQTT();
  
  initializeWiFi(DEVICE_HOSTNAME);
  
  establishWiFi();

  // you're connected now, so print out the data
  printWifiStatus();

}

void loop()
{
  MQTTLoop();
}