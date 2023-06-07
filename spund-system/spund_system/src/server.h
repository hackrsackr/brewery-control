#pragma once

#include "new_config.h"

// HTML web page to handle 4 input fields (Setpoint 1, Setpoint 2, Setpoint 3, Setpoint 4)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Spunder Setpoints</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <style>
            body {
                padding: 25px;
                background-color: black;
                color: grey;
                font-size: 25px;
            }
        </style>
    </head>
    <body>
        <p>Setpoints Vols[CO2]</p>
        <form action="/get">
            sepoint-1:
            <input
                type="number"
                step="0.01"
                name="setpoint_input-1"
                value="%SETPOINT1%"
                required
            />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            sepoint-2:
            <input
                type="number"
                step="0.01"
                name="setpoint_input-2"
                value="%SETPOINT2%"
                required
            />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            sepoint-3:
            <input
                type="number"
                step="0.01"
                name="setpoint_input-3"
                value="%SETPOINT3%"
                required
            />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            sepoint-4:
            <input
                type="number"
                step="0.01"
                name="setpoint_input-4"
                value="%SETPOINT4%"
                required
            />
            <input type="submit" value="Submit" />
        </form>
        <p>MQTT Temp Fields</p>
        <form action="/get">
            mqtt_temp_field-1:
            <input type="text" name="mqtt_input-1" value="%MQTT1%" required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-2:
            <input type="text" name="mqtt_input-2" value="%MQTT2%" required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-3:
            <input type="text" name="mqtt_input-3" value="%MQTT3%" required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-4:
            <input type="text" name="mqtt_input-4" value="%MQTT4%" required />
            <input type="submit" value="Submit" />
        </form>
    </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

String processor(const String &var)
{
    if (var == "SETPOINT1")
    {
        // return _SETPOINT_MESSAGES[0];
        return _SETTINGS[0].server_setpoint;
    }
    if (var == "SETPOINT2")
    {
        // return _SETPOINT_MESSAGES[1];
        return _SETTINGS[1].server_setpoint;
        // return String(_SETTINGS[1].desired_vols);
    }
    if (var == "SETPOINT3")
    {
        // return _SETPOINT_MESSAGES[2];
        return _SETTINGS[2].server_setpoint;
        // return String(_SPUNDERS[2].vols_setpoint);
    }
    if (var == "SETPOINT4")
    {
        // return _SETPOINT_MESSAGES[3];
        return _SETTINGS[3].server_setpoint;
        // return String(_SETTINGS[3].desired_vols);
    }
    if (var == "MQTT1")
    {
        // return _MQTT_MESSAGES[0];
        return _SETTINGS[0].server_sensor;
    }
    if (var == "MQTT2")
    {
        // return _MQTT_MESSAGES[1];
        return _SETTINGS[1].server_sensor;
    }
    if (var == "MQTT3")
    {
        // return _MQTT_MESSAGES[2];
        return _SETTINGS[2].server_sensor;
    }
    if (var == "MQTT4")
    {
        // return _MQTT_MESSAGES[3];
        return _SETTINGS[3].server_sensor;
    }
    return String();
}