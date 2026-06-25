#include <WiFi.h>
#include <ArduinoOTA.h>
#include <EspMQTTClient.h>

#include <flow_config.h>
#include <secrets.h>

EspMQTTClient client;

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_SSID, _PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupOTA() {
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("OTA Start: " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
  });
  
  ArduinoOTA.begin();
}


// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String & payload) {
    Serial.println(payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true
}
void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupOTA();

}

void loop() {
  ArduinoOTA.handle();
  client.loop();
  delay(10);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("OTA_TEST");
}
