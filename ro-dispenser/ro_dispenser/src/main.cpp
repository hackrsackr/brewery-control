/*  
  Rui Santos & Sara Santos - Random Nerd Tutorials
  https://RandomNerdTutorials.com/esp32-web-server-beginners-guide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include "Arduino.h"
#include "ArduinoJson.h"
#include <WiFi.h>
#include <WebServer.h>
#include <EspMQTTClient.h>

#include <vector>

#include <FlowMeter.hpp>
#include <flow_config.h>
#include <secrets.h>


std::vector<FlowMeter *> _FLOWMETERS;

// Replace with your network credentials
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Assign output variables to GPIO pins
const int RELAY_PIN_1 = 16;
const int LED_PIN = 23;

String relay_state = "off";
String ready_state = "Ready";
String flow_state = "0";

auto target_pulses = 6900;

// Create a web server object
WebServer server(80);
EspMQTTClient client(ssid, password, "10.0.0.115");

void onConnectionEstablished();
void handleRoot();

// Function to handle turning GPIO 16 on
void activateRelay1() {
  relay_state = "on";
  digitalWrite(RELAY_PIN_1, HIGH);
  handleRoot();
}

// Function to handle turning GPIO 16 off
void deactivateRelay1() {
  relay_state = "off";
  digitalWrite(RELAY_PIN_1, LOW);
  ready_state = "Not Ready";
  handleRoot();
}

// Function to handle turning GPIO 27 on
void lockDispenser() {
  ready_state = "Not Ready";
  handleRoot();
}

// Function to handle turning GPIO 27 off
void resetDispenser() {
  ready_state = "Ready";
  handleRoot();
}


// Function to handle the root URL and show the current states
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #555555; }</style></head>";
  html += "<body><h1>RO Dispenser</h1>";

  // Display GPIO 26 controls
  html += "<p>Relay State = " + relay_state + " </p>";
  if (relay_state == "off") {
    html += "<p><a href=\"/Relay/on\"><button class=\"button\">DO IT LADY!</button></a></p>";
  } else {
    html += "<p><a href=\"/Relay/off\"><button class=\"button button2\">STOP</button></a></p>";
  }

  // Display GPIO 27 controls
  // html += "<p>Ready State = " + ready_state + "</p>";
  // if (ready_state == "Ready") {
  //   html += "<p><a href=\"/Ready\"><button class=\"button\">Ready</button></a></p>";
  // } else {
  //   html += "<p><a href=\"/Not Ready\"><button class=\"button button2\">Reset</button></a></p>";
  // }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // client.enableDebuggingMessages();
  client.enableOTA();
  
  for (auto &cfg : FLOW_CFGS)
  {
    FlowMeter *f = new FlowMeter(cfg);
    _FLOWMETERS.push_back(f);
  }

  // Initialize the output variables as outputs
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Set outputs to LOW
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server to handle different routes
  server.on("/", handleRoot);
  server.on("/Relay/on", activateRelay1);
  server.on("/Relay/off", deactivateRelay1);
  server.on("/Ready", lockDispenser);
  server.on("/Not Ready", resetDispenser);
  // Start the web server
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // Handle incoming client requests
  server.handleClient();
  client.loop();

  StaticJsonDocument<400> message;
  message["key"] = _CLIENTID;

  for (auto &flowmeter : _FLOWMETERS)
  {
    flowmeter->run();

    message["data"][flowmeter->id]["Total_Pulses"] = flowmeter->total_pulse_count;
    // message["data"][flowmeter->id]["Flow_rate[LPM]"] = flowmeter->flow_rate;
    // message["data"][flowmeter->id]["Total[L]"] = flowmeter->total_liters;
    message["data"][flowmeter->id]["Total[mL]"] = flowmeter->total_milliliters;

    if (flowmeter->total_pulse_count >= target_pulses) {
      deactivateRelay1();
      flowmeter->total_pulse_count = 0;
    }

    flow_state = String(flowmeter->total_milliliters);
  }
  // message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

  // serializeJsonPretty(message["data"], Serial);
  serializeJson(message["data"], Serial);
  Serial.println("");

  delay(1000);
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