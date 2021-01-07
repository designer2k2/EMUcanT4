# EMUcanT4
ECUMaster EMU CAN Stream Reader Arduino Teensy 4 Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/MyLibrary.svg?)](https://www.ardu-badge.com/EMUcanT4)

## Installation

The library is not listed in the Arduino Library manager yet, but you can download and install it manually.

[Download](https://github.com/designer2k2/EMUcanT4/archive/main.zip) the zip file from our [repository](https://github.com/designer2k2/EMUcanT4/) and select in Arduino IDE `Sketch` → `Include library` → `Add .ZIP library...`.

Or clone it into your `Documents/Arduino/libraries` folder:

```sh
git clone https://github.com/designer2k2/EMUcanT4.git
```

When installed you will also see a few examples in `File` → `Examples` → `EMUcanT4` menu.

This Library uses the CAN Library: (installed with the Teensyduino add-on)
https://github.com/tonton81/FlexCAN_T4

## Setup

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus"

The EMU Stream base ID can be changed, the begin function takes this as parameter.

CAN-Bus speed can be set too, look in the examples.

This Library is tested on Teensy 4.0 with CAN1 on pin 22 and 23.

The EMU Black is running Software Version 2.127.

## Other Versions

For Arduino Nano use the EMUcan: https://github.com/designer2k2/EMUcan

## Support

Please feel free to use/extend/report bugs/request features!

