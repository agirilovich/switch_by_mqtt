#include <Arduino.h>
#include <libretiny.h>

#ifndef DEVICE_BOARD_NAME
#  define DEVICE_BOARD_NAME "MQTTswitch"
#endif

static int no_port_pin = 3;

static int cs_port_pin = 17;
static int inc_port_pin = 18;
static int ud_port_pin = 2;

int mqtt_num_attempts = 0;
const int max_mqtt_attempts = 60;

#define DEVICE_HOSTNAME "MQTTswitch"

#include "controlWiFi.h"

#include "MQTT_task.h"

#include <X9C103S.h>
X9C103S board_varistor(inc_port_pin, ud_port_pin, cs_port_pin);

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(no_port_pin, OUTPUT);
  digitalWrite(no_port_pin, LOW);

  Serial.print("Initialize varistor board....   ");
  board_varistor.initializePot();
  Serial.println("Done");

  Serial.print("Initialize MQTT connection...");
  initMQTT();
  Serial.println("Done");

  Serial.print("Configuring WiFi client...");
  initializeWiFi(DEVICE_HOSTNAME);
  Serial.println("Done");
  Serial.print("Start WiFi on ");
  establishWiFi();
  Serial.println("Done");

  // you're connected now, so print out the data
  printWifiStatus();
  
  Serial.println("Set Watchdog timer.");
  lt_wdt_enable(30 * 1000);
  Serial.println("Running the main program...");
}

void loop()
{
  if(MQTTgetstate()) {
    struct SensorsData CurVal;
    CurVal = MQTTLoop();
    board_varistor.setResistance(CurVal.varistor);
    digitalWrite(no_port_pin, CurVal.no_switch);
  } else {
    mqtt_num_attempts++;
    Serial.print("Failed to send MQTT message. Skip the cycle. Number of failed cycles: ");
    Serial.println(mqtt_num_attempts);
  }

  if (mqtt_num_attempts < max_mqtt_attempts)
  {
    lt_wdt_feed();
  } else {
    lt_reboot_wdt();
  }

  delay(100);
}