// INFO:
// Create an Access Point for initial configuration at http://belle.local
// Send HTTP POST to IFTTT only when waking up from deep sleep

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

String AP_NamePrefix = "Belle ";
const char WiFiAPPSK[] = "secret";
const char* DomainName = "belle"; // set domain name domain.local
char ssid [50] = "";
char password [50] = "";

ESP8266WebServer server(80);

void setup() {
  // GPIO02 on ESP-12 module is linked to on-board LED
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setupAP();
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("[INFO] LED on");
  delay(500);

  digitalWrite(2, LOW);
  Serial.println("[INFO] LED off");
  delay(500);

  server.handleClient();
}

void setupAP() {
  Serial.print("[TRACE] Configuring access point");

  WiFi.mode(WIFI_AP);

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = AP_NamePrefix + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);

  if (!MDNS.begin(DomainName)) {
    Serial.println("[ERROR] MDNS responder did not setup");
    while(1) {
      delay(1000);
    }
  }

  MDNS.addService("http", "tcp", 80);

  server.on("/", handleRoot);

  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);

  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();

  Serial.println("[INFO] Started access point!");
}

void handleRoot() {
  if (server.hasArg("ssid") && server.hasArg("password")){
    Serial.print("[INFO] SSID:");
    Serial.println(server.arg("ssid"));
    server.arg("ssid").toCharArray(ssid, 50);

    Serial.print("[INFO] Password received!");
    server.arg("password").toCharArray(password, 50);

    WiFi.begin(ssid, password);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      count++;

      if (count % 10 == 0) {
        Serial.println("");
      }

      if (count > 120) {
        Serial.println("[ERROR] Could not connect to WiFi. Please try again.");
        break;
      }
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("[INFO] WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());

      server.sendHeader("Location","/");
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);

      return;
    }
  }

  String content = "<html><body><form action='/' method='post'>";
  content += "SSID: <input type='text' name='ssid' placeholder='ssid'><br>";
  content += "Password:<input type='password' name='password' placeholder='secret'><br>";
  content += "IFTTT Key:<input type='text' name='key' placeholder='IFTTT Key'><br>";
  content += "<input type='submit' name='submit' value='Submit'></form></body></html>";
  server.send(200, "text/html", content);
}

