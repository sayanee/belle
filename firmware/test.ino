// INFO: Flash this firmware to test the board functions

#define LED 2 // GPIO02 on ESP-12 module is linked to on-board LED

#include <ESP8266WiFi.h>

// Edit "secret" ssid and password below
const char* ssid = "secret";
const char* password = "secret";

void setup() {
  pinMode(LED, OUTPUT);
	Serial.begin(115200);

  Serial.println("[INFO] Starting test in 20 seconds...");
  delay(10000);

  testBlink();
  testWiFiConnectivity();
  testWakeUp();
}

void loop() { }

void testBlink() {
  for (int i=0; i <= 3; i++) {
    Serial.println("[TEST] Blink high");
    digitalWrite(2, HIGH);
    delay(1000);

    Serial.println("[TEST] Blink low");
    digitalWrite(2, LOW);
    delay(1000);
  }
}

void testWiFiConnectivity() {
  Serial.print("[TEST] Connecting to WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("[TEST] WiFi IP address: ");
  Serial.println(WiFi.localIP());
}

void testWakeUp() {
  Serial.print("[TEST] Sleeping now...");
  ESP.deepSleep(0, WAKE_RF_DEFAULT);
}
