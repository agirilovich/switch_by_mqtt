#include "MQTT_task.h"
#include "controlWiFi.h"


const char *mqtt_host = mqtt_server;
const int mqtt_port = 1883;
const char *mqtt_user = mqtt_username;
const char *mqtt_pass = mqtt_password;

WiFiClient client;

#include <PubSubClient.h>
PubSubClient mqtt(client);

struct SensorsData SensorsCurrentValues;

// Define Device in Home Assistant scope of integrations
HADevice ha_device(DEVICE_BOARD_NAME, DEVICE_BOARD_NAME, "1.0");

HASwitch no_switch = HASwitch("no_swtich", "NO swtich", ha_device);
HANumber varistor = HANumber("varistor", "Emulator KTY-150", ha_device, 1, 100, 1);

void ha_callback(HAEntity *entity, char *topic, byte *payload, unsigned int length){
  if(entity == &varistor){
      SensorsCurrentValues.varistor = varistor.getState();
      Serial.printf("Changed KTY-150 state to %d \n", SensorsCurrentValues.varistor);
  }
  if(entity == &no_switch) {
    SensorsCurrentValues.no_switch = no_switch.getState();
    Serial.printf("Changed no switch state to %d \n", SensorsCurrentValues.no_switch);
  }
}

void initMQTT() {
  //Initialise MQTT autodiscovery topic and sensor

  mqtt.setServer(mqtt_host, mqtt_port);
  HAMQTT.begin(mqtt, 2);

  no_switch.addFeature(HA_FEATURE_ICON,"mdi:electric-switch");
  varistor.addFeature(HA_FEATURE_ICON,"mdi:tune-variant");
  varistor.addFeature(HA_FEATURE_DEVICE_CLASS, "power_factor");
  
  HAMQTT.addEntity(no_switch);
  HAMQTT.addEntity(varistor);
  varistor.setState(0);
  no_switch.setState(0);

  HAMQTT.setCallback(ha_callback);

}

struct SensorsData MQTTLoop()
{
  HAMQTT.loop();
  return SensorsCurrentValues;
}

bool MQTTgetstate()
{
  if (WiFi.isConnected() && !HAMQTT.connected())
  {
    Serial.println("Reconnecting to MQTT server.");
    if (HAMQTT.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass))
      Serial.println("Connected to MQTT");
    else
    {
      Serial.println("Failed to connect to MQTT");
      return(false);
    }
  }
  return(true);
}