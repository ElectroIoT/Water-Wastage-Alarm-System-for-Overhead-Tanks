#include <Wire.h>        // Library for I2C communication (OLED)
#include <Adafruit_GFX.h> // Library for graphics on OLED
#include <Adafruit_SSD1306.h> // Library for OLED display
#include <SoftwareSerial.h> // Library for communication with SIM800L (for SMS)

#define TRIG_PIN 12       // Ultrasonic sensor trigger pin
#define ECHO_PIN 13       // Ultrasonic sensor echo pin
#define BUZZER_PIN 9      // Buzzer pin
#define LED_SAFE 3        // Safe level LED
#define LED_CAUTION 4     // Caution level LED
#define LED_FULL 5        // Full level LED

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// SIM800L Configuration
SoftwareSerial sim800l(7, 8); // RX, TX for SIM800L
const char phoneNumber[] = "+911234567890"; // Replace with your phone number

// Define ultrasonic sensor pins
long duration;
int distance;
float waterLevel;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  sim800l.begin(9600);  // Initialize SIM800L communication
  delay(1000);

  // Initialize Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize Buzzer and LED pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_SAFE, OUTPUT);
  pinMode(LED_CAUTION, OUTPUT);
  pinMode(LED_FULL, OUTPUT);

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
  // Measure water level using Ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(TRIG_PIN, LOW);  
  duration = pulseIn(ECHO_PIN, HIGH);  

  // Calculate distance and water level percentage
  distance = duration * 0.034 / 2;  // Distance in cm
  waterLevel = (100 - (distance / 2));  // Convert to percentage (assuming a tank height of 200 cm)

  // Display water level on OLED screen
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Water Level: ");
  display.print(waterLevel);
  display.print("%");

  // Display status and control buzzer & LEDs
  if (waterLevel >= 95) {
    display.setCursor(0, 20);
    display.println("Status: FULL");
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
    digitalWrite(LED_FULL, HIGH);    // Turn on Full LED
    digitalWrite(LED_CAUTION, LOW);  // Turn off Caution LED
    digitalWrite(LED_SAFE, LOW);     // Turn off Safe LED
    sendSMS();  // Send SMS if water level is full
  } else if (waterLevel >= 60) {
    display.setCursor(0, 20);
    display.println("Status: Caution");
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
    digitalWrite(LED_CAUTION, HIGH); // Turn on Caution LED
    digitalWrite(LED_FULL, LOW);    // Turn off Full LED
    digitalWrite(LED_SAFE, LOW);    // Turn off Safe LED
  } else {
    display.setCursor(0, 20);
    display.println("Status: Safe");
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
    digitalWrite(LED_SAFE, HIGH);   // Turn on Safe LED
    digitalWrite(LED_CAUTION, LOW); // Turn off Caution LED
    digitalWrite(LED_FULL, LOW);    // Turn off Full LED
  }

  display.display();  // Update display
  delay(1000);  // Delay before next measurement
}

void sendSMS() {
  sim800l.println("AT+CMGF=1");  // Set SMS to text mode
  delay(1000);
  sim800l.print("AT+CMGS=\"");    // Send SMS to the specified phone number
  sim800l.print(phoneNumber);
  sim800l.println("\"");
  delay(1000);
  sim800l.println("Alert! Water tank is full. Please switch off the pump.");
  delay(1000);
  sim800l.write(26);  // Send the SMS (Ctrl+Z)
  delay(1000);
}
