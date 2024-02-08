#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Spund_System.h"
#include "Relay.h"

#include "server.h"
#include "config.h"

#include <vector>

std::vector<Spund_System *> _SPUNDERS;

EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
// AsyncWebServer server(80);

// void notFound(AsyncWebServerRequest *request);
// String processor(const String &var);
void onConnectionEstablished();

void setup(void)
{
    Serial.begin(115200);
    client.setMaxPacketSize(4096);
    Wire.begin(_I2C_SDA, _I2C_SCL);

    for (auto &spund_cfg : spund_cfgs)
    {
        Spund_System *s = new Spund_System(spund_cfg);
        _SPUNDERS.push_back(s);

        if (!s->begin())
        {
            Serial.printf("ads failed to initialize");
        }
    }
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
	    	// Serial.println(spunder->temp_sensor_id);
            spunder->tempC = input["data"][spunder->temp_sensor_id]["value[degC]"];
	    	// Serial.println(spunder->tempC);
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
            	// message["data"][spunder->spunder_id]["adc"] = spunder->readADC();
            	message["data"][spunder->spunder_id]["Volts"] = spunder->readVolts();
            	message["data"][spunder->spunder_id][spunder->getSensorUnit()] = spunder->readSensorUnits();
            	message["data"][spunder->spunder_id]["PSI_setpoint"] = spunder->computePSISetpoint();
            	message["data"][spunder->spunder_id]["Desired_vols"] = spunder->desired_vols;
            	message["data"][spunder->spunder_id]["Vols"] = spunder->computeVols();
            	message["data"][spunder->spunder_id]["Relay_Toggled"] = spunder->testCarb();
            	message["data"][spunder->spunder_id]["Minutes_since_vent"] = spunder->getLastVent();
	        }

        }
            message["data"]["general"]["IP_address"] = WiFi.localIP();
            message["data"]["general"]["Input_memory_size"] = input.memoryUsage();
            message["data"]["general"]["Output_memory_size"] = message.memoryUsage();

            serializeJsonPretty(message["data"], Serial);

        if (_PUBLISHMQTT)
        {
            client.executeDelayed(5000, [&message]()
            {
                if (!client.publish(_PUBTOPIC, message.as<String>()))
                {   
                    Serial.println("restarting due to failed MQTT publish");
                    // ESP.restart();
                }
                serializeJsonPretty(message, Serial);
            });

        } 

        if (!_PUBLISHMQTT) 
        {
            // serializeJson(message["data"], Serial);
            // Serial.println("");
            serializeJsonPretty(message["data"], Serial);
        } });
}
