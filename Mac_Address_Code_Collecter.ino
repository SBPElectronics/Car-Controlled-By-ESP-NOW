#include "WiFi.h"
#include "esp_mac.h"  // Required for native MAC functions

void setup() {
  Serial.begin(115200);

  // Give the Serial Monitor 1 second to connect after opening
  delay(1000);

  uint8_t mac[6];
  // Reads the hardcoded Wi-Fi Station MAC address directly from the chip
  esp_read_mac(mac, ESP_MAC_WIFI_STA);

  Serial.println("\n--- YOUR ESP32 MAC ADDRESS ---");
  c
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("------------------------------");
}

void loop() {
  // Leave empty
}
