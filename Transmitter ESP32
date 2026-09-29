#include <esp_now.h>
#include <WiFi.h>

#define PIN_VRX 34 // Steering (X-axis)
#define PIN_VRY 35 // Throttle (Y-axis)

const int DEADZONE = 100;

// Receiver MAC Address (ESP32-S3)
uint8_t receiverMAC[] = {0xE0, 0x72, 0xA1, 0xF6, 0x3F, 0xB4};

// Packet structure must match receiver
typedef struct struct_message {
  int command; // 0=Stop, 1=Forward, 2=Backward, 3=Right, 4=Left
} struct_message;

struct_message controlData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer!");
    return;
  }

  Serial.println("Transmitter online (Sending 0-4 commands).");
}

void loop() {
  int rawX = analogRead(PIN_VRX);
  int rawY = analogRead(PIN_VRY);

  // Map to -255 to +255
  int throttle = map(rawY, 0, 4095, 255, -255);
  int steer    = map(rawX, 0, 4095, 255, -255);

  int cmd = 0; // Default Stop

  // Determine dominant axis outside the deadzone
  if (abs(throttle) > DEADZONE || abs(steer) > DEADZONE) {
    if (abs(throttle) >= abs(steer)) {
      cmd = (throttle > 0) ? 1 : 2; // 1 = Fwd, 2 = Back
    } else {
      cmd = (steer > 0) ? 3 : 4;    // 3 = Right, 4 = Left
    }
  }

  controlData.command = cmd;
  esp_now_send(receiverMAC, (uint8_t *)&controlData, sizeof(controlData));

  delay(40);
}
