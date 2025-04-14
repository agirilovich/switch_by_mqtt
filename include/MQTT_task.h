#include <HaMqttEntities.h>
#include "Credentials.h"

#ifndef DEVICE_BOARD_NAME
#  define DEVICE_BOARD_NAME "TechRSMQTTgateway"
#endif

#ifndef MQTT_GENERAL_PREFIX
#  define MQTT_GENERAL_PREFIX "home"
#endif

#undef HA_TOPIC_HEAD
#define HA_TOPIC_HEAD MQTT_GENERAL_PREFIX

struct SensorsData
{
  bool no_switch;
  int varistor;
};

void initMQTT();

bool MQTTgetstate();

struct SensorsData MQTTLoop();


