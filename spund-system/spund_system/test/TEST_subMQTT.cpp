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
            spunder->tempC = input["data"][spunder->temp_sensor_id]["value[degC]"];
            spunder->tempF = spunder->tempC * 1.8 + 32;
            
            // DEBUG:
	    	// Serial.println(spunder->temp_sensor_id);
	    	// Serial.println(spunder->tempC);

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
            	message["data"][spunder->spunder_id]["Volts"] = spunder->readVolts();
            	message["data"][spunder->spunder_id][spunder->getSensorUnit()] = spunder->readSensorUnits();
            	message["data"][spunder->spunder_id]["PSI_setpoint"] = spunder->computePSISetpoint();
            	message["data"][spunder->spunder_id]["Desired_vols"] = spunder->desired_vols;
            	message["data"][spunder->spunder_id]["Vols"] = spunder->computeVols();
            	message["data"][spunder->spunder_id]["Relay_Toggled"] = spunder->testForVent();
            	message["data"][spunder->spunder_id]["Minutes_since_vent"] = spunder->getLastVent();
	        }

        }
    
        message["data"]["memory"]["Input_memory_size"] = input.memoryUsage();
        message["data"]["memory"]["Output_memory_size"] = message.memoryUsage();

        // DEBUG:
        // serializeJsonPretty(message, Serial);

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

        } 

        if (!_PUBLISHMQTT) 
        {
            serializeJson(message["data"], Serial);
            Serial.println("");
        } });
}
