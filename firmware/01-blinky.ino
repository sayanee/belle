// INFO: On-board LED will blink every second

// HARDWARE: ESP12E module with FTDI chip and 2 AA batteries
// `GND` pin on ESP12E to `GND`
// `GPIO0` pin on ESP12E to `GND` for boot loader mode
// `RX` pin on ESP12E to `TX` on FTDI chip
// `TX` pin on ESP12E to `RX` on FTDI chip
// `VCC` pin to 3V from battery
// `EN` pin to 3V from battery

// Flashing via Arduino IDE
// Ensure Baud rate is 74880
// Plug in the FTDI chip to laptop
// Ensure serial monitor shows "boot mode ... waiting for host"
// Close the serial monitor
// Flash the firmware

void setup() {
  pinMode(2, OUTPUT); // GPIO02 on ESP-12 module is linked to on-board LED
  Serial.begin(115200);
  Serial.println("setup");
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(2, LOW);
  Serial.println("LOW");
  delay(1000);
}
