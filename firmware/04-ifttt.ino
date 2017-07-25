// INFO: Wakeup on bell press and HTTP POST to IFTTT
// PREPARE:
// Edit "secret"s in Line 7, 8, 78
// Connect an external button to the connector

#include <ESP8266WiFi.h>

// Edit "secret" ssid and password below
const char* ssid     = "secret";
const char* password = "secret";

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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println(WiFi.localIP());
  delay(500);

  digitalWrite(2, LOW);
  delay(500);

  digitalWrite(2, HIGH);
  delay(500);

  digitalWrite(2, LOW);
  delay(500);

  Serial.println("Making a request to IFTTT...");
  httpPost();

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

  if (!client.connect("maker.ifttt.com", 443)) {
    Serial.println("connection failed");
    return;
  }

  String PostData="sample";

  // Amend "secret" to IFTTT web request URL
  // Get the "secret" from https://ifttt.com/services/maker_webhooks/settings
  client.println("POST /trigger/bell_pressed/with/key/secret HTTP/1.1");
  client.println("Host:  maker.ifttt.com");
  client.println("User-Agent: nodeMCU/1.0");
  client.println("Connection: close");
  client.println("Content-Type: text/html; charset=utf-8;");
  client.print("Content-Length: ");
  client.println(PostData.length());
  client.println();
  client.println(PostData);

  return;
}
