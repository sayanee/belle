# 🛎 belle

> Door bell with a buzzer and notification to IFTTT.

![](screenshot.png)

## Getting started

1. Assemble the test hardware in a breadboard with [nodeMCU](http://nodemcu.com/index_en.html), button and buzzer

  ![](breadboard.JPG)
- Configure IFTTT with [Maker](https://ifttt.com/maker) and [Notification](https://ifttt.com/if_notifications) channels to Receive a web request
- Download the IFTTT app on mobile as well.
- Amend `Line 220` in [firmware code](firmware/belle.io) to add the web request URL
- Load the [firmware code](firmware/belle.io) in Arduino IDE and upload to nodeMCU
- For first time, connect to access point with SSID `Belle XXXX` and password `beautyandthebeast`
- Put in your WiFi SSID and password
- Press the 🛎. You should hear the buzzer and get a mobile notification we well.

## References

1. [Power Saving tips for the ESP8266](https://github.com/z2amiller/sensorboard/blob/master/PowerSaving.md)
- [SparkFun 3.3V Step-Up Breakout - NCP1402](https://www.sparkfun.com/products/10967)
- [Sleeping the ESP8266](http://www.esp8266.com/wiki/doku.php?id=esp8266_power_usage)
- [Reddit: How to actually run an ESP8266 for months on AA batteries](https://www.reddit.com/r/esp8266/comments/4gmkfl/how_to_actually_run_an_esp8266_for_months_on_aa/)
- [Battery Powered ESP8266 IoT – Temperature Sensor](http://homecircuits.eu/blog/battery-powered-esp8266-iot-logger/)
- [ESP8266 Sensor runs 17 days on a coin cell](https://www.youtube.com/watch?v=IYuYTfO6iOs)
- [wifiwebservers - ESP8266 with 2AA batteries](http://www.arduinesp.com/wifiwebserver)
- [Simple Authentication](https://github.com/esp8266/Arduino/blob/4897e0006b5b0123a2fa31f67b14a3fff65ce561/libraries/ESP8266WebServer/examples/SimpleAuthentification/SimpleAuthentification.ino)
## BOM

## Datasheet

1. [AP2112](https://cdn-shop.adafruit.com/product-files/2471/AP2112.pdf): CMOS process low dropout linear regulator
- ESP8266 module
- [Inverting Schmitt Trigger](http://www.ti.com/lit/ds/symlink/sn74ahc1g04.pdf)
- [CH340](https://www.olimex.com/Products/Breadboarding/BB-CH340T/resources/CH340DS1.PDF)
- [Schmitt trigger SN74AHC1G04 Single Inverter Gate](http://www.ti.com/lit/ds/symlink/sn74ahc1g04.pdf)

## Tools

- [simple calculator for estimating a (LiPo) battery's life](http://battery-life.of-things.de/battery-life-calculator.php)
- [Encode to Base64 format](https://www.base64encode.org/)
