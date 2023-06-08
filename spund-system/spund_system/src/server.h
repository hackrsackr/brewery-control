#pragma once

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
            setpoint-1:
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
            setpoint-2:
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
            setpoint-3:
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
            setpoint-4:
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
            <input type="text" name="mqtt_input-1" 
            value="%MQTT1%" 
            required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-2:
            <input type="text" name="mqtt_input-2" 
            value="%MQTT2%" 
            required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-3:
            <input type="text" name="mqtt_input-3" 
            value="%MQTT3%" 
            required />
            <input type="submit" value="Submit" />
        </form>
        <br />
        <form action="/get">
            mqtt_temp_field-4:
            <input type="text" name="mqtt_input-4" 
            value="%MQTT4%" 
            required />
            <input type="submit" value="Submit" />
        </form>
    </body>
</html>)rawliteral";
