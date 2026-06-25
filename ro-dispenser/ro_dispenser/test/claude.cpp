/*
 * RO Water Dispenser Controller
 * ESP32 sketch with flowmeter, relay control, TDS sensor, webserver, and OTA
 * Features: Automatic shutoff at predetermined pulse count
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>

// ===== CONFIGURATION =====
// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Pin definitions
#define FLOWMETER_PIN 25      // GPIO pin for flowmeter pulse input
#define RELAY_PIN 26          // GPIO pin for relay control
#define TDS_INCOMING_PIN 34   // Analog pin for incoming water TDS sensor (ADC1)
#define TDS_RO_PIN 35         // Analog pin for RO output TDS sensor (ADC1)

// Dispense settings
#define DEFAULT_TARGET_PULSES 500  // Default pulses before auto-shutoff
#define VREF 3.3                   // ADC reference voltage
#define SCOUNT 30                  // Number of sample points for TDS averaging

// ===== GLOBAL VARIABLES =====
WebServer server(80);
volatile unsigned long pulseCount = 0;
unsigned long targetPulses = DEFAULT_TARGET_PULSES;
bool dispensing = false;

// TDS sensor variables
int tdsIncoming = 0;          // Incoming water TDS (ppm)
int tdsRO = 0;                // RO output water TDS (ppm)
float tdsReductionRate = 0.0; // TDS reduction percentage
int tdsIncomingBuffer[SCOUNT];
int tdsROBuffer[SCOUNT];
int tdsIncomingBufferIndex = 0;
int tdsROBufferIndex = 0;

// ===== INTERRUPT SERVICE ROUTINE =====
void IRAM_ATTR flowmeterISR() {
  pulseCount++;
}

// ===== TDS SENSOR FUNCTIONS =====
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    return bTab[(iFilterLen - 1) / 2];
  else
    return (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
}

void readTDSSensor() {
  static unsigned long tdsTimer = millis();
  
  if (millis() - tdsTimer > 40U) {  // Sample every 40ms
    tdsTimer = millis();
    
    // Read incoming water TDS sensor
    tdsIncomingBuffer[tdsIncomingBufferIndex] = analogRead(TDS_INCOMING_PIN);
    tdsIncomingBufferIndex++;
    
    // Read RO output TDS sensor
    tdsROBuffer[tdsROBufferIndex] = analogRead(TDS_RO_PIN);
    tdsROBufferIndex++;
    
    // Calculate incoming TDS when buffer is full
    if (tdsIncomingBufferIndex == SCOUNT) {
      tdsIncomingBufferIndex = 0;
      int averageVoltage = getMedianNum(tdsIncomingBuffer, SCOUNT);
      float voltage = averageVoltage * VREF / 4096.0;  // ESP32 12-bit ADC
      
      // Temperature compensation (assuming 25°C)
      float compensationCoefficient = 1.0 + 0.02 * (25.0 - 25.0);
      float compensationVoltage = voltage / compensationCoefficient;
      
      // TDS calculation (ppm)
      tdsIncoming = (int)((133.42 * compensationVoltage * compensationVoltage * compensationVoltage 
                          - 255.86 * compensationVoltage * compensationVoltage 
                          + 857.39 * compensationVoltage) * 0.5);
    }
    
    // Calculate RO output TDS when buffer is full
    if (tdsROBufferIndex == SCOUNT) {
      tdsROBufferIndex = 0;
      int averageVoltage = getMedianNum(tdsROBuffer, SCOUNT);
      float voltage = averageVoltage * VREF / 4096.0;  // ESP32 12-bit ADC
      
      // Temperature compensation (assuming 25°C)
      float compensationCoefficient = 1.0 + 0.02 * (25.0 - 25.0);
      float compensationVoltage = voltage / compensationCoefficient;
      
      // TDS calculation (ppm)
      tdsRO = (int)((133.42 * compensationVoltage * compensationVoltage * compensationVoltage 
                    - 255.86 * compensationVoltage * compensationVoltage 
                    + 857.39 * compensationVoltage) * 0.5);
    }
    
    // Calculate TDS reduction rate
    if (tdsIncoming > 0) {
      tdsReductionRate = ((float)(tdsIncoming - tdsRO) / (float)tdsIncoming) * 100.0;
      if (tdsReductionRate < 0) tdsReductionRate = 0.0;
      if (tdsReductionRate > 100) tdsReductionRate = 100.0;
    } else {
      tdsReductionRate = 0.0;
    }
  }
}

// ===== RELAY CONTROL =====
void startDispensing() {
  if (!dispensing) {
    pulseCount = 0;
    dispensing = true;
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Dispensing started");
  }
}

void stopDispensing() {
  if (dispensing) {
    dispensing = false;
    digitalWrite(RELAY_PIN, LOW);
    Serial.print("Dispensing stopped. Pulses: ");
    Serial.println(pulseCount);
  }
}

void checkDispenseComplete() {
  if (dispensing && pulseCount >= targetPulses) {
    stopDispensing();
  }
}

// ===== WEB SERVER PAGES =====
String getWebPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>RO Water Dispenser</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; max-width: 600px; margin: 50px auto; padding: 20px; background: #f0f0f0; }";
  html += ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }";
  html += "h1 { color: #333; text-align: center; }";
  html += ".status { display: flex; justify-content: space-around; margin: 20px 0; }";
  html += ".status-box { background: #e8f4f8; padding: 15px; border-radius: 5px; text-align: center; flex: 1; margin: 0 10px; }";
  html += ".status-box h3 { margin: 0 0 10px 0; color: #0066cc; font-size: 14px; }";
  html += ".status-box p { margin: 0; font-size: 28px; font-weight: bold; color: #333; }";
  html += ".progress-bar { width: 100%; height: 30px; background: #e0e0e0; border-radius: 15px; overflow: hidden; margin: 20px 0; }";
  html += ".progress-fill { height: 100%; background: linear-gradient(90deg, #4CAF50, #45a049); transition: width 0.3s; }";
  html += "button { width: 100%; padding: 15px; font-size: 18px; margin: 10px 0; border: none; border-radius: 5px; cursor: pointer; }";
  html += ".btn-start { background: #4CAF50; color: white; }";
  html += ".btn-start:hover { background: #45a049; }";
  html += ".btn-start:disabled { background: #cccccc; cursor: not-allowed; }";
  html += ".btn-stop { background: #f44336; color: white; }";
  html += ".btn-stop:hover { background: #da190b; }";
  html += ".btn-settings { background: #2196F3; color: white; }";
  html += ".btn-settings:hover { background: #0b7dda; }";
  html += ".dispensing { background: #ffeb3b; padding: 15px; text-align: center; border-radius: 5px; margin: 10px 0; font-weight: bold; }";
  html += "</style>";
  html += "<script>";
  html += "setInterval(function() { location.reload(); }, 1000);";
  html += "</script>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>💧 RO Water Dispenser</h1>";
  
  if (dispensing) {
    html += "<div class='dispensing'>⚡ DISPENSING IN PROGRESS</div>";
    float progress = (float)pulseCount / targetPulses * 100.0;
    if (progress > 100) progress = 100;
    html += "<div class='progress-bar'><div class='progress-fill' style='width:" + String(progress) + "%'></div></div>";
  }
  
  html += "<div class='status'>";
  html += "<div class='status-box'><h3>Current Pulses</h3><p>" + String(pulseCount) + "</p></div>";
  html += "<div class='status-box'><h3>Target Pulses</h3><p>" + String(targetPulses) + "</p></div>";
  html += "</div>";
  
  html += "<div class='status'>";
  html += "<div class='status-box'><h3>Incoming TDS</h3><p>" + String(tdsIncoming) + " <span style='font-size:14px'>ppm</span></p></div>";
  html += "<div class='status-box'><h3>RO Output TDS</h3><p>" + String(tdsRO) + " <span style='font-size:14px'>ppm</span></p></div>";
  html += "</div>";
  
  html += "<div class='status'>";
  html += "<div class='status-box'><h3>TDS Reduction</h3><p>" + String(tdsReductionRate, 1) + " <span style='font-size:14px'>%</span></p></div>";
  html += "<div class='status-box'><h3>Progress</h3><p>" + String((pulseCount * 100) / targetPulses) + " <span style='font-size:14px'>%</span></p></div>";
  html += "</div>";
  
  if (!dispensing) {
    html += "<button class='btn-start' onclick='location.href=\"/start\"'>Start Dispensing</button>";
  } else {
    html += "<button class='btn-stop' onclick='location.href=\"/stop\"'>Stop Dispensing</button>";
  }
  
  html += "<button class='btn-settings' onclick='location.href=\"/settings\"'>Settings</button>";
  html += "</div></body></html>";
  
  return html;
}

String getSettingsPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Settings</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; max-width: 600px; margin: 50px auto; padding: 20px; background: #f0f0f0; }";
  html += ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }";
  html += "h1 { color: #333; text-align: center; }";
  html += "label { display: block; margin: 15px 0 5px 0; font-weight: bold; color: #555; }";
  html += "input[type=number] { width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ddd; border-radius: 5px; box-sizing: border-box; }";
  html += "button { width: 100%; padding: 15px; font-size: 18px; margin: 10px 0; border: none; border-radius: 5px; cursor: pointer; }";
  html += ".btn-save { background: #4CAF50; color: white; }";
  html += ".btn-save:hover { background: #45a049; }";
  html += ".btn-back { background: #9E9E9E; color: white; }";
  html += ".btn-back:hover { background: #757575; }";
  html += ".info { background: #e3f2fd; padding: 15px; border-radius: 5px; margin: 20px 0; font-size: 14px; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>⚙️ Settings</h1>";
  html += "<div class='info'>Set the number of pulses before the relay automatically shuts off.</div>";
  html += "<form action='/save' method='GET'>";
  html += "<label>Target Pulse Count:</label>";
  html += "<input type='number' name='pulses' value='" + String(targetPulses) + "' min='10' max='10000' required>";
  html += "<button type='submit' class='btn-save'>💾 Save Settings</button>";
  html += "</form>";
  html += "<button class='btn-back' onclick='location.href=\"/\"'>← Back to Home</button>";
  html += "</div></body></html>";
  
  return html;
}

// ===== WEB SERVER HANDLERS =====
void handleRoot() {
  server.send(200, "text/html", getWebPage());
}

void handleStart() {
  startDispensing();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStop() {
  stopDispensing();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSettings() {
  server.send(200, "text/html", getSettingsPage());
}

void handleSave() {
  if (server.hasArg("pulses")) {
    targetPulses = server.arg("pulses").toInt();
    if (targetPulses < 10) targetPulses = 10;
    if (targetPulses > 10000) targetPulses = 10000;
    
    // Save to EEPROM
    EEPROM.write(0, targetPulses & 0xFF);
    EEPROM.write(1, (targetPulses >> 8) & 0xFF);
    EEPROM.commit();
    
    Serial.print("Settings saved. Target pulses: ");
    Serial.println(targetPulses);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  Serial.println("\n\nRO Water Dispenser Controller");
  Serial.println("=============================");
  
  // Initialize EEPROM
  EEPROM.begin(512);
  
  // Load saved settings
  targetPulses = EEPROM.read(0) | (EEPROM.read(1) << 8);
  if (targetPulses < 10 || targetPulses > 10000) {
    targetPulses = DEFAULT_TARGET_PULSES;
    Serial.println("Using default target pulses");
  }
  Serial.print("Target pulses: ");
  Serial.println(targetPulses);
  
  // Initialize pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FLOWMETER_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, LOW);  // Ensure relay is off
  
  // Attach interrupt for flowmeter
  attachInterrupt(digitalPinToInterrupt(FLOWMETER_PIN), flowmeterISR, FALLING);
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup OTA
  ArduinoOTA.setHostname("ro-dispenser");
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
    stopDispensing();  // Stop dispensing during OTA
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update Complete");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  
  // Setup web server
  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.on("/settings", handleSettings);
  server.on("/save", handleSave);
  server.begin();
  
  Serial.println("Web server started");
  Serial.println("Setup complete!");
}

// ===== MAIN LOOP =====
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  
  // Check if dispense cycle is complete
  checkDispenseComplete();
  
  // Read TDS sensor
  readTDSSensor();
  
  // Small delay to prevent watchdog timer issues
  delay(10);
}