// INFO: On-board ESP12 LED will blink
// HARDDWARE: ESP12E module with FTDI chip
// USAGE: Put the ESP12 into flashing firmware mode

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
