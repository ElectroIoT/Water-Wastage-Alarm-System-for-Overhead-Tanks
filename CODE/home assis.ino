#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Wi-Fi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// MQTT Broker details
const char* mqttServer = "your_mqtt_broker_ip"; // Replace with the MQTT broker IP
const int mqttPort = 1883;
const char* mqttUser = "your_mqtt_username";   // MQTT username
const char* mqttPassword = "your_mqtt_password"; // MQTT password

// MQTT topics
const char* waterLevelTopic = "home/water_level";
const char* pumpControlTopic = "home/pump_control";

// Relay pin for controlling the water pump
#define RELAY_PIN 23

// Ultrasonic sensor pins
#define TRIG_PIN 5
#define ECHO_PIN 18

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);

// Define ultrasonic sensor variables
long duration;
int distance;
float waterLevel;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);

  // Initialize Relay and Ultrasonic Sensor
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT, &Wire)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Water Wastage Alarm");
  display.display();
  delay(2000);  // Wait for a couple of seconds to display the welcome message
}

void loop() {
  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Read water level using ultrasonic sensor
  waterLevel = readWaterLevel();

  // Display water level on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Water Level: ");
  display.print(waterLevel);
  display.print("%");

  // Send water level to Home Assistant via MQTT
  char levelStr[8];
  dtostrf(waterLevel, 1, 2, levelStr);
  client.publish(waterLevelTopic, levelStr);

  // Control pump based on water level (low: start pump, full: stop pump)
  if (waterLevel < 30) {
    digitalWrite(RELAY_PIN, HIGH);  // Turn on the pump
  } else if (waterLevel >= 95) {
    digitalWrite(RELAY_PIN, LOW);   // Turn off the pump
  }

  // Update OLED display with pump status
  display.setCursor(0, 20);
  if (waterLevel < 30) {
    display.println("Pump Status: ON");
  } else if (waterLevel >= 95) {
    display.println("Pump Status: OFF");
  } else {
    display.println("Pump Status: OFF");
  }
  display.display();

  delay(2000);  // Update every 2 seconds
}

// MQTT Reconnection function
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("WaterLevelClient", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(pumpControlTopic);  // Subscribe to control topic for manual pump control
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// Callback function for MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, pumpControlTopic) == 0) {
    if (payload[0] == '1') {
      digitalWrite(RELAY_PIN, HIGH);  // Turn on the pump if MQTT message is '1'
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Turn off the pump if MQTT message is '0'
    }
  }
}

// Function to read water level from ultrasonic sensor
float readWaterLevel() {
  // Trigger the ultrasonic sensor to measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm and then the water level as a percentage
  distance = duration * 0.034 / 2;
  float waterLevel = (100 - (distance / 2));  // Assuming tank height is 200 cm
  return waterLevel;
}
