# 🚗 Smart Parking System using ESP32

A real-time smart parking system using ESP32, ultrasonic sensor, and IR sensor with a live web dashboard.

---

## 🔥 Features

- 📡 Ultrasonic distance detection
- 👁️ IR obstacle detection
- 🧠 Smart Occupancy Score (0–100%)
- 🚦 Real-time status (EMPTY / HALF / OCCUPIED)
- 🌐 Live Web Dashboard (WiFi-based)
- 📊 Sensor fusion algorithm

---

## 🧰 Hardware Required

- ESP32 Dev Board
- HC-SR04 Ultrasonic Sensor
- IR Sensor Module
- Jumper wires
- Breadboard (optional)

---

## 🔌 Circuit Connections

### Ultrasonic Sensor
| Pin | ESP32 |
|-----|------|
| VCC | 5V |
| GND | GND |
| TRIG | GPIO 5 |
| ECHO | GPIO 18 |

### IR Sensor
| Pin | ESP32 |
|-----|------|
| VCC | 3.3V / 5V |
| GND | GND |
| OUT | GPIO 4 |

---

## ⚙️ How It Works

### 🧠 Occupancy Score Logic

- IR detects object → strong weight
- Ultrasonic measures distance → proportional weight

Final Score:
- 0–40% → EMPTY 🟢
- 40–70% → HALF FILLED 🟠
- 70–100% → OCCUPIED 🔴

---

## 📡 Web Dashboard

After uploading code:

1. Open Serial Monitor
2. Copy ESP32 IP address
3. Open browser:
