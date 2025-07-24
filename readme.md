# 🚰 Water Wastage Alarm System for Overhead Tanks

## 🧠 Project Overview
The **Water Wastage Alarm System** is an automated solution for Indian households designed to prevent water overflow in overhead tanks. The system uses an **ESP32-WROOM** to monitor the water level via an **HC-SR04 ultrasonic sensor**, control the water pump using a **relay module**, and send water level data to **Home Assistant** via **MQTT**. The system is also capable of turning on the pump when the water level falls below 30% and turning it off when the tank is full (above 95%).

---

## 🛠️ Features
- **Real-time Water Level Monitoring:** Uses an ultrasonic sensor to measure water level and display it on an OLED screen.
- **Overflow Detection:** Triggers a **buzzer** and sends an **SMS alert** when the tank is full.
- **Home Assistant Integration:** Sends water level data and controls the pump via **MQTT** for remote monitoring and automation.
- **Automated Pump Control:** Turns on the water pump when the level is low and turns it off when full.
- **Low Power Consumption:** Designed to operate on minimal power.
- **Affordable and Easy to Install:** Low-cost components, suitable for all households.

---

## 🧑‍💻 Installation & Setup

### 🔧 Hardware Setup:
1. **H2S-Dev Board (ESP32-WROOM)**:
   - **HC-SR04 Ultrasonic Sensor** (for water level detection)
   - **Relay Module** (for controlling the pump)
   - **OLED Display** (for displaying water level status)
   - **Buzzer** (for overflow alert)

2. **Connections:**
   - **HC-SR04**:
     - TRIG_PIN (5)
     - ECHO_PIN (18)
   - **Relay Module** (connected to GPIO 23)
   - **OLED Display** (connected via I2C, default SDA and SCL pins)
   - **Buzzer** (connected to GPIO 22)

### 🧑‍💻 Software Setup:

1. **Install Libraries:**
   Install the following libraries in the Arduino IDE:
   - **ESP32 Board** (Arduino IDE > Preferences > Additional Boards Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`)
   - **PubSubClient** (for MQTT communication)
   - **Adafruit SSD1306** (for OLED display)
   - **Adafruit GFX** (for graphics on OLED)

