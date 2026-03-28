# 🎮 Motion Control Game using ESP32 & MPU6050

## 📌 Project Overview
This project is a motion-controlled game system where player movements are captured using an MPU6050 accelerometer sensor connected to an ESP32, and actions are sent to a game running on a computer via WebSockets.

The system detects physical motion (like jumping) and translates it into in-game actions — similar to games like Subway Surfers.

---

## ⚙️ Hardware Used

| Component        | Description |
|----------------|------------|
| ESP32           | Microcontroller with WiFi capability |
| MPU6050         | Accelerometer + Gyroscope sensor for motion detection |
| Jumper Wires    | For connections |
| USB Cable       | For power & programming |

---

## 🧠 Software Used

- Arduino IDE (for ESP32 programming)
- Node.js (WebSocket server)
- JavaScript (Game logic)
- WebSockets (Real-time communication)

---

## 🔌 System Architecture

MPU6050 → ESP32 → WiFi → WebSocket Server → Game (Browser)

---

## 🚀 Features

- Real-time motion tracking
- Jump detection using acceleration (Z-axis)
- Wireless communication using WiFi
- Low latency WebSocket communication
- Cooldown system to prevent multiple false jumps

---

## 📊 How It Works

1. MPU6050 reads acceleration data continuously.
2. ESP32 processes Z-axis acceleration.
3. If motion pattern matches a jump (up + down movement):
   - A signal is sent via WebSocket.
4. The game receives the signal and triggers a jump action.

---

## 🧮 Jump Detection Logic

- Upward threshold: +1.5g  
- Downward threshold: -1.0g  
- Buffer size: 10 samples  
- Cooldown: 1 second  

Jump is detected when both upward and downward motion occur within the buffer window.

---

## 📡 WiFi & Server Configuration

Update these in your ESP32 code:

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_PASSWORD";
const char* server_address = "YOUR_SERVER_IP";
const uint16_t server_port = 3000;

---

## 🛠️ Setup Instructions

### 1. Hardware Setup

Connect MPU6050 to ESP32 using I2C:

- VCC → 3.3V  
- GND → GND  
- SDA → GPIO21  
- SCL → GPIO22  

---

### 2. ESP32 Setup

Install required libraries in Arduino IDE:

- Wire.h  
- MPU6050  
- WiFi.h  
- ArduinoWebsockets  

Upload the ESP32 code.

---

### 3. Server Setup (Node.js)

Install dependencies:

npm install ws express

Run the server:

node app.js

---

### 4. Run the Game

- Start the frontend game (browser/app)
- Ensure ESP32 and server are on same WiFi
- Move/jump → game reacts

---

## 📁 Project Structure

Motion-Control-Game/
│
├── Demo/
│   ├── Demo_Video.mp4
│   ├── pic1.png
│   └── pic2.png
│
├── Hardware/
│   └── ESP32/
│       └── ESP32.ino
│
├── Software/
│   └── CAP-project-Draft-main/
│       ├── app.js
│       ├── package.json
│       └── public/
│
└── README.md

---

## 🎥 Demo

Check the Demo folder for:
- Project video  
- Screenshots  

---

## ⚠️ Important Notes

- ESP32 and server must be on the same WiFi network  
- Adjust thresholds if jump detection is too sensitive  
- Ensure stable power supply  

---

## 🔥 Future Improvements

- Add left/right movement detection  
- Improve motion accuracy using filtering  
- Add multiplayer support  
- Mobile app integration  

---

## 👨‍💻 Author

Himadri Nag  
ECE Student @ NIT Calicut  

---

## ⭐ If you like this project

Give it a star on GitHub and share it
