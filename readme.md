# 🚰 Water Wastage Alarm System for Overhead Tanks

## 🧠 Project Overview
The **Water Wastage Alarm System** is a smart, low-cost solution designed for Indian households to prevent water overflow in overhead tanks. Using an **ultrasonic sensor**, the system monitors the water level in real-time and provides alerts through a **buzzer**, an **OLED display**, and optional **SMS notifications** (via **SIM800L**). This project aims to conserve water and reduce unnecessary electricity wastage by automating the process of monitoring the water level in overhead tanks.

---

## 🛠️ Features
- **Real-time Water Level Monitoring:** Using an ultrasonic sensor to measure the distance from the top of the tank to the water surface.
- **Overflow Detection:** A buzzer sounds when the tank is full (95%+).
- **OLED Display:** Displays the current water level percentage and the status of the tank.
- **LED Indicators:** Visual indicators for **Safe**, **Caution**, and **Full** water levels.
- **Optional SMS Alerts:** Sends an SMS to the user when the tank is full using the **SIM800L** module.
- **Low Power Consumption:** Designed to run efficiently on minimal power.
- **Affordable & Compact:** Low-cost components and easy to install in any household.

---

## 🌐 Working Principle
1. **Water Level Measurement:**  
   The ultrasonic sensor measures the distance between the top of the tank and the water surface. This distance is then converted to a percentage to represent the current water level.

2. **Overflow Detection:**  
   If the water level exceeds 95%, the system:
   - Triggers a **buzzer** alarm.
   - Sends an **SMS** (optional) using **SIM800L**.
   - Displays the "FULL" warning on the OLED screen.

3. **Status Indicators:**  
   The system uses **LEDs** to show the current water level:
   - **Safe:** Water level below 60%.
   - **Caution:** Water level between 60% and 95%.
   - **Full:** Water level above 95%.

---

## 🛠️ Components
| **Component**             | **Description**                                      |
|---------------------------|------------------------------------------------------|
| **H2S-Dev Board**         | Main controller (CH32V203 MCU)                       |
| **HC-SR04 Ultrasonic Sensor** | Used for measuring the water level                  |
| **OLED 128x64 Display**   | Displays water level and system status               |
| **SIM800L Module**        | Sends SMS alerts (optional)                          |
| **Buzzer**                | Sound alarm for overflow                            |
| **LEDs (3)**              | Visual indicators for safe, caution, and full levels |
| **5V Regulator**          | Powers the system                                    |

---

## 💻 Installation & Setup

### 🔧 Hardware Setup:
1. **H2S-Dev Board:**
   - Connect the **HC-SR04 ultrasonic sensor** to the **TRIG_PIN** and **ECHO_PIN** on the H2S-Dev board.
   - Attach the **SIM800L** to pins 7 (RX) and 8 (TX) for communication (if SMS alert functionality is desired).
   - Wire up the **buzzer** and **LEDs** to the appropriate pins.

2. **OLED Display:**
   - Connect the **OLED display** to the **I2C pins** (SCL, SDA) on the H2S-Dev Board.

### 🧑‍💻 Software Setup:
1. Install the required libraries:
   ```bash
   # For OLED display:
   $ pip install Adafruit-GFX
   $ pip install Adafruit_SSD1306

   # For SIM800L communication:
   $ pip install SoftwareSerial
