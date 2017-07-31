// INFO:
// Wakeup on bell press
// Send HTTP POST to IFTTT only when waking up from deep sleep
// PREPARE:
// Edit "secret"s in Line 17, 18, 88
// Connect an external button to the connector

#define WAKE_UP_REASON_DEEP_SLEEP 5

#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

// Edit "secret" ssid and password below
const char* ssid = "secret";
const char* password = "secret";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;

void setup() {
  // GPIO02 on ESP-12 module is linked to on-board LED
  pinMode(2, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int wakeupReason = getWakeupReason();

  digitalWrite(2, HIGH);
  Serial.println(WiFi.localIP());
  delay(500);

  digitalWrite(2, LOW);
  delay(500);

  digitalWrite(2, HIGH);
  delay(500);

  digitalWrite(2, LOW);
  delay(500);

  if (wakeupReason == WAKE_UP_REASON_DEEP_SLEEP) {
    Serial.println("Making a request to IFTTT...");
    httpPost();
  }

  digitalWrite(2, HIGH);
  Serial.println("Sleep.... 2");
  delay(500);

  digitalWrite(2, LOW);
  Serial.println("Sleep.... 1");
  delay(500);

  // sleep indefinitely until wakeup
  ESP.deepSleep(0, WAKE_RF_DEFAULT);
}

void httpPost() {
  WiFiClientSecure client;

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // Get the "secret" from https://ifttt.com/services/maker_webhooks/settings
  String url = "/trigger/bell_pressed/with/key/secret";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("[ERROR] Client Timeout!");
      client.stop();
      return;
    }
  }

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println("[INFO] Closing connection");
  return;
}

int getWakeupReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL) {
    return -1;
  }

  // REASON_DEFAULT_RST       = 0,  /* normal startup by power on */
  // REASON_WDT_RST           = 1,  /* hardware watch dog reset */
  // REASON_EXCEPTION_RST     = 2,  /* exception reset, GPIO status won’t change */
  // REASON_SOFT_WDT_RST      = 3,  /* software watch dog reset, GPIO status won’t change */
  // REASON_SOFT_RESTART      = 4,  /* software restart ,system_restart , GPIO status won’t change */
  // REASON_DEEP_SLEEP_AWAKE  = 5,  /* wake up from deep-sleep */
  // REASON_EXT_SYS_RST       = 6   /* external system reset */

  return ri->reason;
}
