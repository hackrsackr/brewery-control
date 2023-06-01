// #include <Arduino.h>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <array>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Relay.h"

#include "config.h"
#include "server.h"

std::array<Spund_System, _NUMBER_OF_SPUNDERS> spund_arr;
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
AsyncWebServer server(80);
StaticJsonDocument<4000> input;

void initWiFi();
void onConnectionEstablished();
void publishData();

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    client.enableOTA();

    initWiFi();

    // Initialize each Spunder in spund_arr
    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].begin(
            ADS1115_ADDRESS1,     // ads_address
            GAIN_TWOTHIRDS,       // ads_gain
            _I2C_SDA,             // i2c_sda
            _I2C_SCL,             // i2c_scl
            i,                    // ads_channel
            _MIN_SENSOR_VOLTS[i], // volts at ZERO PSI
            _MAX_SENSOR_VOLTS[i], // volts at MAX PSI
            _MAX_PSIS[i],         // MAX PSI sensor can read
            _RELAY_PINS[i]);      // relay pin
    }

    // Webserver
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;

    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        if (request->hasParam(_SETPOINT_INPUTS[i])) {
            _SETPOINT_MESSAGES[i] = request->getParam(_SETPOINT_INPUTS[i])->value();
            _DESIRED_VOLS[i] = _SETPOINT_MESSAGES[i].toDouble();
            inputMessage = _SETPOINT_MESSAGES[i];
            inputParam = _SETPOINT_INPUTS[i];
        }
        if (request->hasParam(_MQTT_INPUTS[i])) {
            _MQTT_MESSAGES[i] = request->getParam(_MQTT_INPUTS[i])->value();
            _MQTT_FIELDS[i] =  _MQTT_MESSAGES[i];
            inputMessage = _MQTT_MESSAGES[i];
            inputParam = _MQTT_INPUTS[i];
        }

    }

    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
    server.onNotFound(notFound);
    server.begin();
}

void loop(void)
{
    client.loop();
}

void initWiFi()
{
    WiFi.disconnect(true);
    delay(1000);
    WiFi.begin(_SSID, _PASS);
    uint8_t failed_connections = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("connecting..");
        failed_connections++;
        if (failed_connections > 10)
        {
            Serial.println("restarting..");
            ESP.restart();
        }
    }
    Serial.print("Connected to ");
    Serial.println(WiFi.localIP());
}

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     {
    // Serial.println(payload);
    deserializeJson(input, payload);
    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        spund_arr[i].tempC = input["data"][_MQTT_FIELDS[i]]["value[degC]"];
        spund_arr[i].tempF = spund_arr[i].tempC * 1.8 + 32;
        spund_arr[i].vols_setpoint = _DESIRED_VOLS[i];
    }
    publishData(); });
}

void publishData()
{
    StaticJsonDocument<1000> message;

    if (!client.isConnected())
    {
        ESP.restart();
    }

    for (uint8_t i = 0; i < _NUMBER_OF_SPUNDERS; ++i)
    {
        if (!spund_arr[i].tempC)
        {
            Serial.println(" no temp reading");
            continue;
        }
        else
        {
            message[_SPUNDER_NAMES[i]]["TempC"] = spund_arr[i].tempC;
            message[_SPUNDER_NAMES[i]]["Volts"] = spund_arr[i].getVolts();
            message[_SPUNDER_NAMES[i]]["PSI"] = spund_arr[i].getPSI();
            message[_SPUNDER_NAMES[i]]["PSI_setpoint"] = spund_arr[i].computePSISetpoint();
            message[_SPUNDER_NAMES[i]]["Vols_setpoint"] = spund_arr[i].vols_setpoint;
            message[_SPUNDER_NAMES[i]]["Vols"] = spund_arr[i].computeVols();
            message[_SPUNDER_NAMES[i]]["Minutes_since_vent"] = spund_arr[i].test_carb();
        }
    }

    message["memory"]["Input_memory_size"] = input.memoryUsage();
    message["memory"]["Output_memory_size"] = message.memoryUsage();

    // print to serial to be read by pi
    // serializeJson(message, Serial);

    // pretty print for debug
    serializeJsonPretty(message, Serial);

    // print to serial and publish to brewblox over MQTT
    // serializeJsonPretty(message, Serial);
    // client.publish(_PUBTOPIC, message.as<String>());

    Serial.println("");
    delay(5000);
}