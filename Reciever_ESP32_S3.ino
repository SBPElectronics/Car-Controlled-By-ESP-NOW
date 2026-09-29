#include <esp_now.h>
#include <WiFi.h>

// Driver Pins
#define PIN_ENA  4   // Left Speed
#define PIN_IN1  5   // Left Fwd
#define PIN_IN2  6   // Left Rev
#define PIN_IN3  7   // Right Fwd
#define PIN_IN4  15  // Right Rev
#define PIN_ENB  16  // Right Speed

// PWM Configuration (ESP32 core v2.x)
#define PWM_FREQ       1000
#define PWM_RES        8
#define PWM_CH_LEFT    0
#define PWM_CH_RIGHT   1

const int DRIVE_SPEED = 200; // 0 to 255

// Packet structure: single integer command
typedef struct struct_message {
  int command; // 0=Stop, 1=Forward, 2=Backward, 3=Right, 4=Left
} struct_message;

struct_message incomingData;
volatile unsigned long lastRecvTime = 0;

void drive(int in1, int in2, int in3, int in4, int speedVal) {
  digitalWrite(PIN_IN1, in1);
  digitalWrite(PIN_IN2, in2);
  digitalWrite(PIN_IN3, in3);
  digitalWrite(PIN_IN4, in4);

  ledcWrite(PWM_CH_LEFT, speedVal);
  ledcWrite(PWM_CH_RIGHT, speedVal);
}

void applyCommand(int cmd) {
  switch (cmd) {
    case 1: // Forward
      drive(HIGH, LOW, HIGH, LOW, DRIVE_SPEED);
      break;
    case 2: // Backward
      drive(LOW, HIGH, LOW, HIGH, DRIVE_SPEED);
      break;
    case 3: // Right (Spin Turn)
      drive(HIGH, LOW, LOW, HIGH, DRIVE_SPEED);
      break;
    case 4: // Left (Spin Turn)
      drive(LOW, HIGH, HIGH, LOW, DRIVE_SPEED);
      break;
    case 0: // Stop
    default:
      drive(LOW, LOW, LOW, LOW, 0);
      break;
  }
}

// Callback for ESP32 core v2.x
void onDataRecv(const uint8_t *mac_addr, const uint8_t *incoming, int len) {
  memcpy(&incomingData, incoming, sizeof(incomingData));
  lastRecvTime = millis();
  applyCommand(incomingData.command);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  ledcSetup(PWM_CH_LEFT, PWM_FREQ, PWM_RES);
  ledcAttachPin(PIN_ENA, PWM_CH_LEFT);

  ledcSetup(PWM_CH_RIGHT, PWM_FREQ, PWM_RES);
  ledcAttachPin(PIN_ENB, PWM_CH_RIGHT);

  drive(LOW, LOW, LOW, LOW, 0);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("ESP32-S3 Receiver Ready (1=Fwd, 2=Back, 3=Right, 4=Left, 0=Stop)");
}

void loop() {
  // Failsafe: stop motors if no command received for > 400ms
  if (millis() - lastRecvTime > 400 && lastRecvTime != 0) {
    drive(LOW, LOW, LOW, LOW, 0);
  }
  delay(20);
}
