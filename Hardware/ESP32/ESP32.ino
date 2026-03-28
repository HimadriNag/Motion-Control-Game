#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

const char* ssid = "AndroidAP_4378";           // Replace with your WiFi SSID
const char* password = "VIRATK18";   // Replace with your WiFi Password
const char* server_address = "10.110.106.109"; // Replace with your Node.js server IP or hostname
const uint16_t server_port = 3000;
const char* ws_path = "/ws/esp32";

MPU6050 mpu;
WebsocketsClient client;

unsigned long lastJumpTime = 0;
const unsigned long jumpCooldown = 1000; // 1 second between jumps

const float thresholdUp = 1.5;    // upward accel threshold (g)
const float thresholdDown = -1.0; // downward accel threshold (g)
const int samplesToCheck = 10;

float accelZBuffer[samplesToCheck];
int bufferIndex = 0;

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Received message from server: ");
  Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("WebSocket connection opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("WebSocket connection closed");
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("WebSocket got ping");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("WebSocket got pong");
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void connectWebSocket() {
  String ws_url = String("ws://") + server_address + ":" + String(server_port) + String(ws_path);
  Serial.print("Connecting to WebSocket: ");
  Serial.println(ws_url);
  
  if (client.connect(ws_url)) {
    Serial.println("WebSocket connected");
  } else {
    Serial.println("WebSocket connection failed");
  }
}

void sendJump() {
  if (client.available()) {
    String msg = "{\"event\":\"game_jump\"}";
    client.send(msg);
    Serial.println("Jump detected and sent to server");
  } else {
    Serial.println("WebSocket not connected, jump not sent");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  connectToWiFi();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1) delay(100);
  }
  Serial.println("MPU6050 connected");

  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  connectWebSocket();

  for (int i = 0; i < samplesToCheck; i++) {
    accelZBuffer[i] = 0;
  }
}

void loop() {
  client.poll();  // Keep WebSocket alive and handle incoming events

  if (!client.available()) {
    Serial.println("WebSocket disconnected, attempting reconnect...");
    connectWebSocket();
    delay(2000);
    return;
  }

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float accelZ = (float)az / 16384.0;
  accelZBuffer[bufferIndex] = accelZ;
  bufferIndex = (bufferIndex + 1) % samplesToCheck;

  static bool jumpDetected = false;
  if (millis() - lastJumpTime > jumpCooldown) {
    bool upward = false;
    bool downward = false;

    for (int i = 0; i < samplesToCheck; i++) {
      if (accelZBuffer[i] > thresholdUp) upward = true;
      if (accelZBuffer[i] < thresholdDown) downward = true;
    }

    if (upward && downward && !jumpDetected) {
      sendJump();
      jumpDetected = true;
      lastJumpTime = millis();
    } else if (!upward && !downward) {
      jumpDetected = false;
    }
  }

  delay(20);
}
