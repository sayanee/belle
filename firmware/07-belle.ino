// INFO:
// Connect to "Bell XXX" Access Point for initial configuration at http://belle.local
// Get IFTTT key at https://ifttt.com/services/maker_webhooks/settings
// Send HTTP POST to IFTTT only when waking up from deep sleep

#define WAKE_UP_REASON_DEEP_SLEEP 5
#define LED 2

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

extern "C" {
  #include "user_interface.h"
}

String AP_NamePrefix = "Belle ";
const char WiFiAPPSK[] = "beautyandbeast";
const char* DomainName = "belle"; // set domain name domain.local
char ssid [50] = "";
char password [50] = "";
String key = "";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;

ESP8266WebServer server(80);

void setup() {
  // GPIO02 on ESP-12 module is linked to on-board LED
  pinMode(LED, OUTPUT);
	Serial.begin(115200);

  if (!hasWiFiCredentials()) {
    setupAP();
  }
}

void loop() {
  Serial.println();

  if (!hasWiFiCredentials()) {
    blink(200);
    Serial.println("[INFO] WiFi is not configured!");
    Serial.println("[INFO] Connect to 'Belle XXX' WiFi and visit http://belle.local/");
    server.handleClient();
  } else {
    blink(2000);

    if (getResetReason() == WAKE_UP_REASON_DEEP_SLEEP && connectToWiFi() == true) {
      httpPost();
    }

    goToSleep();
  }
}

void goToSleep() {
  Serial.println("[INFO] Sleeping in 2");
  delay(100);
  Serial.println("[INFO] Sleeping in 1");
  delay(100);

  ESP.deepSleep(0, WAKE_RF_DEFAULT);
}

void httpPost() {
  WiFiClientSecure client;

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // Get the "secret" from https://ifttt.com/services/maker_webhooks/settings
  String url = "/trigger/bell_pressed/with/key/";
  key = readKey();
  Serial.print("[INFO] EEPROM Key stored: ");
  Serial.println(key);

  client.print(String("GET ") + url + key + " HTTP/1.1\r\n" +
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

  return;
}

int getResetReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL) {
    return -1;
  }

  // REASON_DEFAULT_RST		= 0,	/* normal startup by power on */
	// REASON_WDT_RST			= 1,	/* hardware watch dog reset */
	// REASON_EXCEPTION_RST	= 2,	/* exception reset, GPIO status won’t change */
	// REASON_SOFT_WDT_RST   	= 3,	/* software watch dog reset, GPIO status won’t change */
	// REASON_SOFT_RESTART 	= 4,	/* software restart ,system_restart , GPIO status won’t change */
	// REASON_DEEP_SLEEP_AWAKE	= 5,	/* wake up from deep-sleep */
	// REASON_EXT_SYS_RST      = 6		/* external system reset */

  return ri->reason;
}

bool hasWiFiCredentials() {
  if (WiFi.SSID() == "") {
    return false;
  }

  return true;
}

void blink(int interval) {
  digitalWrite(LED, HIGH);
  delay(interval);

  digitalWrite(LED, LOW);
  delay(interval);
}

bool connectToWiFi() {
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;

    if (count % 10 == 0) {
      Serial.println("");
    }

    if (count > 120) {
      Serial.println("Could not connect to WiFi. Please try again.");
      return false;
    }
  }

  return true;
}

void setupAP() {
  Serial.println("[TRACE] Configuring access point");
  WiFi.mode(WIFI_AP);

  String AP_NameString = getAPName();

  // convert String to char array
  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);

  startMDNS();
  startServer();
  Serial.println("[INFO] Started access point!");
}

String getAPName() {
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return AP_NamePrefix + macID;
}

void startMDNS() {
  if (!MDNS.begin(DomainName)) {
    Serial.println("[ERROR] MDNS responder did not setup");
    while(1) {
      delay(1000);
    }
  }

  MDNS.addService("http", "tcp", 80);
}

void startServer() {
  server.on("/", handleRoot);

  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);

  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
}

void handleRoot() {
  if (server.hasArg("ssid") && server.hasArg("password")){
    Serial.print("[INFO] SSID:");
    Serial.println(server.arg("ssid"));
    server.arg("ssid").toCharArray(ssid, 50);

    Serial.print("[INFO] Password received!");
    server.arg("password").toCharArray(password, 50);

    Serial.println("[INFO] IFTTT key received!");
    writeKey(server.arg("key"));
    readKey();

    if (server.arg("key") == readKey()) {
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
          WiFi.disconnect();
          setupAP();
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
  }

  String content = "<html><body><form action='/' method='post'>";
  content += "SSID: <input type='text' name='ssid' placeholder='ssid'><br>";
  content += "Password:<input type='password' name='password' placeholder='secret'><br>";
  content += "IFTTT Key:<input type='text' name='key' placeholder='IFTTT Key'><br>";
  content += "<input type='submit' name='submit' value='Submit'></form></body></html>";
  server.send(200, "text/html", content);
}

void writeKey(String writeStr) {
  EEPROM.begin(512);
  delay(10);

  for (int i = 0; i < writeStr.length(); ++i) {
    EEPROM.write(i, writeStr[i]);
    Serial.print("[INFO] Writing to EEPROM: ");
    Serial.println(writeStr[i]);
  }

  EEPROM.commit();
}

String readKey() {
  EEPROM.begin(512);

  String readStr;
  char readChar;
  Serial.print("[INFO] Reading from EEPROM: ");

  // TODO: Store length of key
  for (int i = 0; i < 22; ++i) {
    readChar = char(EEPROM.read(i));
    readStr += readChar;

    Serial.print("Char: ");
    Serial.println(readChar);
  }

  Serial.println("");
  Serial.println(readStr);
  return readStr;
}
