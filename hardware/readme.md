# hardware continuity checklist

> Designed PCB layout with [KiCad](http://kicad-pcb.org/)

A list of connectivity checks after soldering

## Vcc (3.3V) - front

Reference: ESP8266 `Vcc` pin

- [x] `R3`, `R6`, `R7`, `R8`
- [x] `C2`, `C3`
- [x] `SP1` long pin
- [x] `P1` pin `1`
- [x] `U1` pin `6`
- [x] `U3`pin `5`

### Ground - back

Reference: ESP8266 `GND` pin

- [x] `R1`, `R5`
- [x] `C1`, `C2`, `C3`, `C4`, `C5`, `C6`, `C7`
- [x] `P2` Bell connector
- [x] `P3` FTDI connector
- [x] `P4` Boot jumper

## 5V

Reference: `P3` FTDI connector `5V`

- [x] `P2`
- [x] `C4`, `C7`
- [x] `U3` pins `1` and `3`

## Shorted pins?

- [x] `U1`, `U2` ESP8266, `U3`
- [x] `SP1`
- [x] `P1`, `P2`, `P3`, `P4`
