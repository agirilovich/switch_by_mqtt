#include "MQTT_task.h"
#include "controlWiFi.h"


const char *mqtt_host = mqtt_server;
const int mqtt_port = 1883;
const char *mqtt_user = mqtt_username;
const char *mqtt_pass = mqtt_password;

WiFiClient client;

#include <PubSubClient.h>
PubSubClient mqtt(client);

// Define Device in Home Assistant scope of integrations
HADevice ha_device(DEVICE_BOARD_NAME, DEVICE_BOARD_NAME, "1.0");

// Mode
#define OPTIONS_COUNT 4
const char *pump_mode_options[4] PROGMEM = {
     "Grzanie domu",
     "Priorytet bojlera",
     "Pompy równoległe",
     "Tryb letni"
     };
HASelect pump_mode = HASelect("pump_mode", "Mode", ha_device, OPTIONS_COUNT, pump_mode_options);


void initMQTT() {
  //Initialise MQTT autodiscovery topic and sensor
  mqtt.setServer(mqtt_host, mqtt_port);
  HAMQTT.begin(mqtt, 22);

  ext_temp.addFeature(HA_FEATURE_DEVICE_CLASS, "TEMPERATURE");
  ext_temp.addFeature(HA_FEATURE_ICON,"mdi:home-thermometer-outline");
  
  HAMQTT.addEntity(pump_mode);
}

bool MQTTpublish(struct SensorsData* SensorsCurrentValues)
{
  if (WiFi.status() == WL_CONNECTED && !HAMQTT.connected())
  {
    if (HAMQTT.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass))
      Serial.println("Connected to MQTT");
    else
    {
      Serial.println("Failed to connect to MQTT");
      return(false);
    }
  }
  
  pump_mode.setState(pump_mode_options[(int)SensorsCurrentValues->pump_mode]);

  return(true);
}

/*
bool MQTTMessageCallback()
{
  char MessageBuf[16];
  //Publish MQTT messages
  Serial.println("Publishing MQTT messages...");
  //mqtt.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass);
  if (mqtt.connected()) {

    sprintf(MessageBuf, "%d", int(SetPoint));
    mqtt.publish(MQTTTSetTopicState, MessageBuf, false);

    sprintf(MessageBuf, "%s", FlameOn?"ON":"OFF");
    mqtt.publish(MQTTFlameOnTopicState, MessageBuf, false);

    sprintf(MessageBuf, "%d", int(MaxModulationLevel));
    mqtt.publish(MQTTMaxRelModLevelSettingTopicState, MessageBuf, false);

    sprintf(MessageBuf, "%d", int(RoomSetPoint));
    mqtt.publish(MQTTTrSetTopicState, MessageBuf, false);

    sprintf(MessageBuf, "%d", int(RoomTemperature));
    mqtt.publish(MQTTTrTopicState, MessageBuf, false);

    Serial.println("Done");
  
  }
  else {
    Serial.println("Unable to connect to MQTT broker");
    Serial.println("Cycle is skipped");
    Serial.println("Trying to reconnect");
    initMQTT();
    return(false);

  }
  //mqtt.disconnect();
  return(true);
}
*/

void MQTTLoop()
{
  HAMQTT.loop();
}