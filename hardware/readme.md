# hardware continuity checklist

> Designed PCB layout with [KiCad](http://kicad-pcb.org/)

A list of connectivity checks after soldering

## Vcc (3.3V) - front

Reference: ESP8266 `Vcc` pin

- [ ] `R3`, `R6`, `R7`, `R8`
- [ ] `C2`, `C3`
- [ ] `SP1` long pin
- [ ] `P1` pin `1`
- [ ] `U1` pin `6`
- [ ] `U3`pin `5`

### Ground - back

Reference: ESP8266 `GND` pin

- [ ] `R1`, `R5`
- [ ] `C1`, `C2`, `C3`, `C4`, `C5`, `C6`, `C7`
- [ ] `P2` Bell connector
- [ ] `P3` FTDI connector
- [ ] `P4` Boot jumper

## 5V

Reference: `P3` FTDI connector `5V`

- [ ] `P2`
- [ ] `C4`, `C7`
- [ ] `U3` pins `1` and `3`
