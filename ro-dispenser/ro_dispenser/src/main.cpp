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
std::vector<TDSSensor *> _TDSSENSORS;

// Replace with your network credentials
const char *ssid = SECRET_SSID;
const char *password = SECRET_PASS;

// Assign output variables to GPIO pins
const int RELAY_PIN_1 = 16;
const int LED_PIN = 23;

String relay_state = "off";
String ready_state = "Ready";
String flow_state = "0";

// auto target_pulses = 6900; // Do_It_Lady *both ladies*
int target_pulses = 6600; // Two_It_Lady

// Create a web server object
WebServer server(80);
EspMQTTClient client(ssid, password, "10.0.0.115");

void onConnectionEstablished();
void handleRoot();
void handleData();
void handleConfig();
void handleSetTarget();

// Turn the fill relay on. No HTTP here, so it is safe to call from loop().
void setRelayOn()
{
  relay_state = "on";
  digitalWrite(RELAY_PIN_1, HIGH);
}

// Turn the fill relay off. No HTTP here, so it is safe to call from loop().
void setRelayOff()
{
  relay_state = "off";
  digitalWrite(RELAY_PIN_1, LOW);
  ready_state = "Not Ready";
}

// Route handlers: change state, then redirect to "/" so the address bar never
// keeps /Relay/on or /Relay/off. A browser reload then re-fetches "/" and cannot
// start another fill.
void activateRelay1()
{
  setRelayOn();
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

void deactivateRelay1()
{
  setRelayOff();
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

// Function to handle turning GPIO 27 on
void lockDispenser()
{
  ready_state = "Not Ready";
  handleRoot();
}

// Function to handle turning GPIO 27 off
void resetDispenser()
{
  ready_state = "Ready";
  handleRoot();
}

void handleSetTarget()
{
  if (server.hasArg("target_pulses"))
  {
    int requested = server.arg("target_pulses").toInt();
    if (requested > 0)
    {
      target_pulses = requested;
    }
  }

  // Post/redirect/get: send the browser back to the config page
  server.sendHeader("Location", "/config");
  server.send(303, "text/plain", "");
}

// Function to handle the root URL and show the current states
void handleRoot()
{
  String html = R"HTML(<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<style>
html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
.button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
.button2 { background-color: #555555; }
.field { font-size: 18px; margin: 12px; }
.input { font-size: 18px; padding: 8px; width: 140px; text-align: center; }
.submit { background-color: #2196F3; color: white; border: none; padding: 10px 18px; font-size: 18px; cursor: pointer; }
.cards { display: flex; flex-wrap: wrap; justify-content: center; }
.card { background: #f2f2f2; border-radius: 8px; margin: 8px; padding: 12px 18px; min-width: 200px; }
.value { font-size: 32px; font-weight: bold; }
.label { font-size: 14px; color: #555; }
</style></head>
<body><h1>RO Dispenser</h1>
)HTML";

  // Relay control
  html += "<p>Relay State = " + relay_state + " </p>";
  if (relay_state == "off")
  {
    html += "<p><a href=\"/Relay/on\"><button class=\"button\">DO IT LADY!</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/Relay/off\"><button class=\"button button2\">STOP</button></a></p>";
  }

  html += "<p><a href=\"/config\"><button class=\"submit\">Config</button></a></p>";

  // Live status, filled in by /data every second
  html += R"HTML(
<div class="cards">
  <div class="card"><div class="label">Pulses this fill</div><div class="value" id="pulses">--</div><div class="label">target <span id="target">--</span></div></div>
  <div class="card"><div class="label">Dispensed</div><div class="value"><span id="ml">--</span> mL</div></div>
  <div class="card"><div class="label">Flow</div><div class="value"><span id="lpm">--</span></div><div class="label">L/min</div></div>
</div>
<h2>TDS</h2>
<div class="cards" id="tds"><p class="label">waiting for sensors...</p></div>
<p class="label" id="status"></p>
<script>
function poll() {
  fetch('/data', { cache: 'no-store' })
    .then(function (r) { return r.json(); })
    .then(function (d) {
      document.getElementById('pulses').textContent = d.pulses;
      document.getElementById('target').textContent = d.target;
      document.getElementById('ml').textContent = d.ml;
      document.getElementById('lpm').textContent = Number(d.lpm).toFixed(2);

      var html = '';
      d.tds.forEach(function (t) {
        html += '<div class="card"><div class="label">' + t.id + '</div>'
              + '<div class="value">' + Number(t.tds).toFixed(0) + ' ppm</div>'
              + '<div class="label">EC ' + Number(t.ec).toFixed(0) + ' | ' + Number(t.voltage).toFixed(3) + ' V | ADC ' + t.adc + '</div></div>';
      });
      document.getElementById('tds').innerHTML = html;
      document.getElementById('status').textContent = 'updated ' + new Date().toLocaleTimeString();
    })
    .catch(function () {
      document.getElementById('status').textContent = 'lost connection to dispenser';
    });
}
poll();
setInterval(poll, 1000);
</script>
</body></html>)HTML";

  server.send(200, "text/html", html);
}

// JSON snapshot of the current readings, polled by the status page
void handleData()
{
  DynamicJsonDocument doc(1024);
  doc["relay"] = relay_state;
  doc["target"] = target_pulses;

  if (!_FLOWMETERS.empty())
  {
    FlowMeter *f = _FLOWMETERS[0];
    doc["pulses"] = f->total_pulse_count;
    doc["ml"] = f->total_milliliters;
    doc["lpm"] = f->flow_rate;
  }

  JsonArray tds = doc.createNestedArray("tds");
  for (auto *t : _TDSSENSORS)
  {
    JsonObject o = tds.createNestedObject();
    o["id"] = t->id.c_str();
    o["tds"] = t->TDS;
    o["ec"] = t->EC;
    o["voltage"] = t->voltage;
    o["adc"] = t->adc;
  }

  String out;
  serializeJson(doc, out);
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "application/json", out);
}

// Config page: settings that used to live on the home page
void handleConfig()
{
  String html = R"HTML(<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<style>
html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
.field { font-size: 18px; margin: 12px; }
.input { font-size: 18px; padding: 8px; width: 140px; text-align: center; }
.submit { background-color: #2196F3; color: white; border: none; padding: 10px 18px; font-size: 18px; cursor: pointer; }
a { font-size: 18px; }
</style></head>
<body><h1>RO Dispenser Config</h1>
)HTML";

  html += "<form method=\"POST\" action=\"/settarget\">";
  html += "<p class=\"field\">Target Pulses: <input class=\"input\" type=\"number\" name=\"target_pulses\" value=\"" + String(target_pulses) + "\" min=\"1\" step=\"1\"></p>";
  html += "<p><input class=\"submit\" type=\"submit\" value=\"Update\"></p>";
  html += "</form>";
  html += "<p><a href=\"/\">Back to dashboard</a></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup()
{
  Serial.begin(115200);

  // client.enableDebuggingMessages();
  client.enableOTA();

  for (auto &cfg : FLOW_CFGS)
  {
    FlowMeter *f = new FlowMeter(cfg);
    _FLOWMETERS.push_back(f);
  }

  for (auto &cfg : TDS_CFGS)
  {
    TDSSensor *t = new TDSSensor(cfg);
    _TDSSENSORS.push_back(t);
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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server to handle different routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/config", handleConfig);
  server.on("/settarget", HTTP_POST, handleSetTarget);
  server.on("/Relay/on", activateRelay1);
  server.on("/Relay/off", deactivateRelay1);
  server.on("/Ready", lockDispenser);
  server.on("/Not Ready", resetDispenser);
  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
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

    if (flowmeter->total_pulse_count >= target_pulses)
    {
      setRelayOff();
      flowmeter->total_pulse_count = 0;
    }

    flow_state = String(flowmeter->total_milliliters);
  }

  for (auto &tdssensor : _TDSSENSORS)
  {
    tdssensor->readTDSSensor();
    message["data"][tdssensor->id]["ADC"] = tdssensor->adc;
    message["data"][tdssensor->id]["Voltage"] = tdssensor->voltage;
    message["data"][tdssensor->id]["EC"] = tdssensor->EC;
    message["data"][tdssensor->id]["TDS"] = tdssensor->TDS;
  }

  // message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

  serializeJsonPretty(message["data"], Serial);
  // serializeJson(message["data"], Serial);
  Serial.println("");

  delay(1000);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String &payload)
                   { Serial.println(payload); });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true
}