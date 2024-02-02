#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "ADS_Sensor.h"
#include "Spund_System.h"
#include "Relay.h"

#include "server.h"
#include "config.h"

#include <vector>

std::vector<Spund_System *> _SPUNDERS;

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
AsyncWebServer server(80);

void onConnectionEstablished();
void notFound(AsyncWebServerRequest *request);
String processor(const String &var);

void setup(void)
{
    Serial.begin(115200);

    // client.enableDebuggingMessages();
    client.setMaxPacketSize(4096);
    client.enableOTA();

    WiFi.mode(WIFI_STA);
    WiFi.begin(_SSID, _PASS);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    for (auto &spund_cfg : spund_cfgs)
    {
        Spund_System *s = new Spund_System(spund_cfg);
        _SPUNDERS.push_back(s);
    }

    // // Webserver
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;

    for (auto& spunder : _SPUNDERS) {
        if (request->hasParam(spunder->server_setpoint_input)) {
            spunder->server_setpoint = request->getParam(spunder->server_setpoint_input)->value();
            spunder->desired_vols = spunder->server_setpoint.toDouble();
            inputMessage = spunder->server_setpoint;
            inputParam = spunder->server_setpoint_input;
        }
        if (request->hasParam(spunder->server_sensor_input)) {
            spunder->temp_sensor_id = request->getParam(spunder->server_sensor_input)->value();
            spunder->server_sensor = spunder->temp_sensor_id;
            inputMessage = spunder->server_sensor;
            inputParam = spunder->server_sensor_input;
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

void onConnectionEstablished()
{
    client.subscribe(_SUBTOPIC, [](const String &payload)
                     {
        //  Serial.println(payload);
        StaticJsonDocument<4000> input;
        deserializeJson(input, payload);

        StaticJsonDocument<1000> message;
        message["key"] = _CLIENTID;

        for (auto &spunder : _SPUNDERS)
        {
            spunder->tempC = input["data"][spunder->temp_sensor_id]["value[degC]"];
            spunder->tempF = spunder->tempC * 1.8 + 32;

	        if (!spunder->tempC) 
	        { 
	    	    Serial.print(spunder->spunder_id);
	    	    Serial.println(": no temp reading!");
	    	    continue;
	        } 
	        else 
	        {
            	message["data"][spunder->spunder_id]["TempC"] = spunder->tempC;
            	message["data"][spunder->spunder_id]["Temp_Sensor"] = spunder->temp_sensor_id;
            	message["data"][spunder->spunder_id]["Volts"] = spunder->getVolts();
            	message["data"][spunder->spunder_id]["PSI"] = spunder->getPSI();
            	message["data"][spunder->spunder_id]["PSI_setpoint"] = spunder->computePSISetpoint();
            	message["data"][spunder->spunder_id]["Desired_vols"] = spunder->desired_vols;
            	message["data"][spunder->spunder_id]["Vols"] = spunder->computeVols();
            	message["data"][spunder->spunder_id]["Relay_Toggled"] = spunder->testCarb();
            	message["data"][spunder->spunder_id]["Minutes_since_vent"] = spunder->getLastVent();
	        }
        }

        message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
        message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

        if (_PUBLISHMQTT)
        {
            client.executeDelayed(5000, [&message]()
            {
                if (!client.publish(_PUBTOPIC, message.as<String>()))
                {   
                    Serial.println("restarting due to failed MQTT publish");
                    ESP.restart();
                }
            });

            serializeJsonPretty(message, Serial);
        } 

        if (!_PUBLISHMQTT) 
        {
            serializeJson(message["data"], Serial);
            Serial.println("");
            // serializeJsonPretty(message["data"], Serial);
        } });
}

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

String processor(const String &var)
{
    if (var == "SETPOINT1")
    {
        return _SPUNDERS[0]->server_setpoint;
    }
    if (var == "SETPOINT2")
    {
        return _SPUNDERS[1]->server_setpoint;
    }
    if (var == "SETPOINT3")
    {
        return _SPUNDERS[2]->server_setpoint;
    }
    if (var == "SETPOINT4")
    {
        return _SPUNDERS[3]->server_setpoint;
    }
    if (var == "MQTT1")
    {
        return _SPUNDERS[0]->server_sensor;
    }
    if (var == "MQTT2")
    {
        return _SPUNDERS[1]->server_sensor;
    }
    if (var == "MQTT3")
    {
        return _SPUNDERS[2]->server_sensor;
    }
    if (var == "MQTT4")
    {
        return _SPUNDERS[3]->server_sensor;
    }
    return String();
}
