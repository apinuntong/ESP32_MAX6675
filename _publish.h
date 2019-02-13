#include <MqttConnector.h>

extern int relayPinState;
extern MqttConnector* mqtt;

extern int relayPin;
extern int LED_PIN;

extern char myName[];

static void readSensor(); 

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject *root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
    data["thermocouple_1"] = readThermocouple(12,14,27);
    data["thermocouple_2"] = readThermocouple(12,26,27);
  }, PUBLISH_EVERY);
  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });

  mqtt->on_published([&](const MQTT::Publish & pub) {
      Serial.println("Published.");
  });
}

static void readSensor() {
  Serial.print("thermocouple_1 = ");
  Serial.print(readThermocouple(12,14,27));
  Serial.print(" thermocouple_2 = ");
  Serial.println(readThermocouple(12,26,27));

}
