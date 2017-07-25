// INFO: On-board LED will blink every second
// HARDWARE: ESP12E module with FTDI chip

void setup() {
  pinMode(2, OUTPUT); // GPIO02 on ESP-12 module is linked to on-board LED
  Serial.begin(9600);
  Serial.println("setup");
}

void loop() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}