#include <Arduino.h>

#ifndef DEVICE_BOARD_NAME
#  define DEVICE_BOARD_NAME "MQTTswitch"
#endif

static int no_port_pin = 20;

static int cs_port_pin = 17;
static int inc_port_pin = 18;
static int ud_port_pin = 2;

#define DEVICE_HOSTNAME "MQTTswitch"

#include "controlWiFi.h"

#include "MQTT_task.h"

#include <X9C103S.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");

  Serial.print("Start WiFi on ");
  Serial.println(DEVICE_BOARD_NAME);

  pinMode(no_port_pin, OUTPUT);
  digitalWrite(no_port_pin, HIGH);

  X9C103S varistor(inc_port_pin, ud_port_pin, cs_port_pin);
  varistor.initializePot();

  initMQTT();
  
  initializeWiFi(DEVICE_HOSTNAME);
  
  establishWiFi();

  // you're connected now, so print out the data
  printWifiStatus();
  lt_wdt_enable(30 * 1000);
}

void loop()
{
  MQTTLoop();
  varistor.setResistance(SensorsCurrentValues.varistor);
  digitalWrite(no_port_pin, SensorsCurrentValues.no_switch);
  lt_wdt_feed();
}