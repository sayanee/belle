// Usage: Set AP and then on button press send HTTP POST
// 1. Select AP name "Belle XXX"
// 2. Put in AP password
// 3. Go to browser http://belle.local
// 4. View Hello World!

// #define DEBUG
#define WAKE_UP_REASON_DEEP_SLEEP 5

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>

extern "C" {
  #include "user_interface.h"
}

String AP_NamePrefix = "Belle "; // set AP name prefix
const char WiFiAPPSK[] = "beautyandthebeast"; // set AP password
const char* DomainName = "belle"; // set domain name domain.local
char ssid [50]     = "";
char password [50] = "";

ESP8266WebServer server(80);

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  if (WiFi.SSID() == "") {
    setupAP();
  } else {
#ifdef DEBUG
    Serial.println("Wake up!");
#endif
  }
}

void loop() {
  if (WiFi.SSID() == "") {
    server.handleClient();
  } else {
    int resetReason = getResetReason();

#ifdef DEBUG
    Serial.print("system_get_rst_info: ");
    Serial.println(resetReason);
#endif

    if (resetReason == WAKE_UP_REASON_DEEP_SLEEP) {
      if (connectToWiFi() == true) {
#ifdef DEBUG
        Serial.println("Making a request to IFTTT...");
#endif
        httpPost();
      }

    }

#ifdef DEBUG
    Serial.println("Sleeping in 2");
    delay(100);
    Serial.println("Sleeping in 1");
    delay(100);
#endif

    ESP.deepSleep(0, WAKE_RF_DEFAULT);
  }
}

void setupAP() {
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
    Serial.println("Error setting up MDNS responder!");
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

#ifdef DEBUG
  Serial.println("Started AP");
#endif
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

void handleRoot() {
  if (server.hasArg("ssid") && server.hasArg("password")){
    Serial.print("SSID:");
    Serial.println(server.arg("ssid"));
    server.arg("ssid").toCharArray(ssid, 50);

    Serial.print("Password:");
    Serial.println(server.arg("password"));
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

#ifdef DEBUG
      if (count % 10 == 0) {
        Serial.println("");
      }
#endif

      if (count > 120) {
#ifdef DEBUG
        Serial.println("Could not connect to WiFi. Please try again.");
#endif
        break;
      }
    }

    if (WiFi.status() == WL_CONNECTED) {
#ifdef DEBUG
      Serial.println("");
      Serial.print("WiFi connected! ");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
#endif

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
  content += "<input type='submit' name='submit' value='Submit'></form></body></html>";
  server.send(200, "text/html", content);
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

void httpPost() {
  WiFiClientSecure client;

  if (!client.connect("maker.ifttt.com", 443)) {
    Serial.println("connection failed");
    return;
  }

  String PostData="sample";

  // Amend "secret" to IFTTT web request URL
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
