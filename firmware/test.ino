// INFO: Flash this firmware to test the board functions

#define LED 2 // GPIO02 on ESP-12 module is linked to on-board LED
#define WAKE_UP_REASON_DEEP_SLEEP 5

#include <ESP8266WiFi.h>

// Edit "secret" ssid and password below
const char* ssid = "secret";
const char* password = "secret";

extern "C" {
  #include "user_interface.h"
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  if (getResetReason() == WAKE_UP_REASON_DEEP_SLEEP) {
    testWakeUp();
  }

  Serial.println("[INFO] Starting test in 5 seconds...");
  delay(5000);

  testBlink();
  testWiFiConnectivity();
  eraseStoredConfig();
  testSleep();
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

void testSleep() {
  Serial.print("[TEST] Sleeping now...");
  ESP.deepSleep(0, WAKE_RF_DEFAULT);
}

void testWakeUp() {
  Serial.println("");
  Serial.println("[TEST] Wake up after reset");
}

int getResetReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL) {
    return -1;
  }

  // REASON_DEFAULT_RST   = 0,  /* normal startup by power on */
  // REASON_WDT_RST     = 1,  /* hardware watch dog reset */
  // REASON_EXCEPTION_RST = 2,  /* exception reset, GPIO status won’t change */
  // REASON_SOFT_WDT_RST    = 3,  /* software watch dog reset, GPIO status won’t change */
  // REASON_SOFT_RESTART  = 4,  /* software restart ,system_restart , GPIO status won’t change */
  // REASON_DEEP_SLEEP_AWAKE  = 5,  /* wake up from deep-sleep */
  // REASON_EXT_SYS_RST      = 6    /* external system reset */

  return ri->reason;
}

void eraseStoredConfig() {
  // Erase stored WiFi SSID and password
  WiFi.disconnect();
  Serial.println("[INFO] WiFi credentials are erased");
}
