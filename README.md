# EMUcanT4

🔥 **This Library is obsolete!** 🔥

The EMUcan Library is now able to run also on a Teensy, no specific version is needed anymore. 🤩

Please migrate to the EMUcan Library, there will be no future update/support here.

**https://github.com/designer2k2/EMUcan**

## old:

ECUMaster EMU CAN Stream Reader Arduino Teensy 4 and 3 Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcanT4.svg?)](https://www.ardu-badge.com/EMUcanT4) [![CI badge](https://github.com/designer2k2/EMUcanT4/actions/workflows/main.yml/badge.svg)](https://github.com/designer2k2/EMUcanT4/actions) [![CI badge](https://github.com/designer2k2/EMUcanT4/actions/workflows/compile_sketches.yml/badge.svg)](https://github.com/designer2k2/EMUcanT4/actions)

**Content:**

* [Installation](#installation)
* [Setup](#setup)
* [Hardware](#hardware)
* [Software usage](#software-usage)
   * [Initialization](#initialization)
   * [Check on CAN Bus updates](#check-on-can-bus-updates)
   * [Status](#status)
   * [Reading the Values](#reading-the-values)
   * [Reading Flags](#reading-flags)
   * [Sending Data](#sending-data)
   * [Custom Frame Receive](#custom-frame-receive)
   * [Mailbox Status](#mailbox-status)
   * [CAN Bus Warnings and Errors](#can-bus-warnings-and-errors)
* [Others](#others)
   * [Different Versions](#different-versions)
   * [Support](#support)

## Installation

To install EMUcanT4 into your Arduino IDE you can use the Library Manager (available from Arduino IDE version 1.6.2). Open the IDE and click to the `Sketch` menu and then `Include Library` → `Manage Libraries`. There search for "EMUcanT4" and click `Install`.

Or alternatively install it as ZIP file:

[Download](https://github.com/designer2k2/EMUcanT4/archive/main.zip) the zip file from our [repository](https://github.com/designer2k2/EMUcanT4/) and select in Arduino IDE `Sketch` → `Include library` → `Add .ZIP library...`.

Or clone it into your `Documents/Arduino/libraries` folder:

```sh
git clone https://github.com/designer2k2/EMUcanT4.git
```

When installed you will also see a few examples in `File` → `Examples` → `EMUcanT4` menu.

This Library uses the CAN Library: 
https://github.com/tonton81/FlexCAN_T4 it is automatically installed with the Teensyduino add-on.

## Setup

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus"

The EMU Stream base ID can be changed, the begin function takes this as parameter.

CAN-Bus speed can be set too, look in the examples.

## Hardware

The Teensy 4.0 has a CAN Bus inside, CAN1 on pin 22 and 23 is used. For Teensy 3 CAN0 is used.

Teensy 3.5 and 3.6 are also supported.

Tested CAN Bus Transceiver can be found here: https://github.com/PaulStoffregen/FlexCAN

# Software usage

## Initialization

To create connection with the EMU Can Base (600 by default).

```C++
EMUcan emucan(0x600);
```

Then in the begin define the CAN Bus baudrate

```C++
emucan.begin(500000);
```

## Check on CAN Bus updates

Call this as often as possible in the loop function:

```C++
emucan.checkEMUcan();
```

## Status

The EMUcan library provides its status:

```C++
enum EMUcan_STATUS {
  EMUcan_FRESH,
  EMUcan_RECEIVED_WITHIN_LAST_SECOND,
  EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND,
};
```

## Reading the Values

Example on how to read a value:
```C++
Serial.println(emucan.emu_data.RPM);
```

All the values:

see https://github.com/designer2k2/EMUcanT4/blob/main/src/EMUcanT4.h

```C++
// Available data
struct emu_data_t {
  uint16_t RPM;  //RPM
  uint16_t MAP;  //kPa
  uint8_t TPS;  //%
  int8_t IAT;  //C
  float Batt;  //V
  float IgnAngle;  //deg
  float pulseWidth;  //ms
  uint16_t Egt1;  //C
  uint16_t Egt2;  //C
  float dwellTime;  //ms
  int8_t gear;  //
  uint8_t Baro;  //kPa
  float analogIn1;  //V
  float analogIn2;  //V
  float analogIn3;  //V
  float analogIn4;  //V
  float analogIn5;  //V
  float analogIn6;  //V
  int8_t emuTemp;  //C
  float oilPressure;  //Bar
  uint8_t oilTemperature;  //C
  float fuelPressure;  //Bar
  int16_t CLT;  //C
  float flexFuelEthanolContent;  //%
  float wboLambda;  //λ
  uint16_t vssSpeed;  //km/h
  float lambdaTarget;  //λ
  uint16_t cel;  //
  float LambdaCorrection; //%
  uint8_t flags1; //Flags 1
  uint8_t outflags1; //Outflags 1
  uint8_t outflags2; //Outflags 2
  uint8_t outflags3; //Outflags 3
  uint8_t outflags4; //Outflags 4
  uint8_t pwm1; //%
  uint16_t boostTarget; //kPa
  uint8_t pwm2; //%
  float fuel_used; //L
  uint8_t DSGmode; //DSG mode
  float DBWpos; //%
  float DBWtarget; //%
  uint16_t TCdrpmRaw; //
  uint16_t TCdrpm; //
  uint8_t TCtorqueReduction; //%
  uint8_t PitLimitTorqueReduction; //%
};
```

## Reading Flags

Example on how to check if the Engine is currently in idle:

```C++
if (emucan.emu_data.flags1 & emucan.F_IDLE) {
  Serial.println("Engine Idle active");
}
```

The flags1 would contain following states:
```C++
enum FLAGS1 : uint8_t {
  F_GEARCUT = (1 << 0),
  F_ALS = (1 << 1),
  F_LC = (1 << 2),
  F_IDLE = (1 << 3),
  F_TABLE_SET = (1 << 4),
  F_TC_INTERVENTION = (1 << 5),
  F_PIT_LIMITER = (1 << 6),
  F_BRAKE_SWITCH = (1 << 7)
};
```

There are many more registers, take a look at https://github.com/designer2k2/EMUcanT4/blob/main/src/EMUcanT4.h 

The information if the CEL is on can be checked by a dedicated function:

```C++
if (emucan.decodeCel()){
  Serial.println("WARNING Engine CEL active");
}
```

Details on why the CEL is on is contained in the cel flag:

```C++
enum ERRORFLAG : uint16_t {
  ERR_CLT = (1 << 0),
  ERR_IAT = (1 << 1),
  ERR_MAP = (1 << 2),
  ERR_WBO = (1 << 3),
  ERR_EGT1 = (1 << 4),
  ERR_EGT2 = (1 << 5),
  EGT_ALARM = (1 << 6),
  KNOCKING = (1 << 7),
  FFSENSOR = (1 << 8),
  ERR_DBW = (1 << 9),
  ERR_FPR = (1 << 10)
};
```

Example to check CEL against the ERR_CLT:

```C++
if (emucan.emu_data.cel & emucan.ERR_CLT) {
  Serial.println("WARNING Engine CEL active due to CLT");
}
```

## Sending Data

This can be used to transmit data to the EMU Black, or any other Device on the CAN Bus.

Example: https://github.com/designer2k2/EMUcanT4/blob/main/examples/EMUcanSendTest/EMUcanSendTest.ino

Define a CAN Message:
```C++
struct can_frame canMsg1;
```

Fill the CAN Message with data:
```C++
canMsg1.can_id  = 0x0F6;
canMsg1.can_dlc = 2;
canMsg1.data[0] = 0xFF;
canMsg1.data[1] = 0x00;
```

Send the CAN Message:
```C++
emucan.sendFrame(&canMsg1);
```

In the EMU Black Software use this to define the receive:
https://github.com/designer2k2/EMUcanT4/blob/main/examples/EMUcanSendTest/EMUBlackCANStreamExample.canstr


## Custom Frame Receive

Listening to more than just the EMU Black CAN Stream is possible. Every CAN frame can be used.

The advanced example shows it: https://github.com/designer2k2/EMUcanT4/blob/main/examples/EMUcanAdvancedTest/EMUcanAdvancedTest.ino

in Setup:
```C++
ReturnAllFramesFunction LetMeHaveIt = specialframefunction;
emucan.ReturnAllFrames(LetMeHaveIt);
```

Stop the function:
```C++
emucan.ReturnAllFramesStop();
```

Example receive function:
```C++
void specialframefunction(const struct can_frame *frame) {
  //Magic things can happen here, but dont block!
  Serial.print(frame->can_id, HEX); // print ID
  Serial.print(" ");
  Serial.print(frame->can_dlc, HEX); // print DLC
  Serial.print(" ");

  for (int i = 0; i < frame->can_dlc; i++)  { // print the data
    Serial.print(frame->data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  //Toggle the onboard LED for show:
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
```

## Mailbox Status:

The Teensy 4.0 CAN offers this for debug reasons, it prints into the Serial port:
```C++
emucan.mailboxStatus();
```

## CAN Bus Warnings and Errors:

The Teensy 4.0 CAN offers diagnose informations for debug reasons with the Flex_CAN_T4 library.
```C++
    if (emucan.can_error_flag) {
      //CAN Error gets true on any warning / error from the CAN controller
      //The can_error_data contains detailed information.
      Serial.println("CAN Error!");
      Serial.print("FlexCAN State: "); Serial.print((char*)emucan.can_error_data.state);
      if ( emucan.can_error_data.BIT1_ERR ) Serial.print(", BIT1_ERR");
      if ( emucan.can_error_data.BIT0_ERR ) Serial.print(", BIT0_ERR");
      if ( emucan.can_error_data.ACK_ERR ) Serial.print(", ACK_ERR");
      if ( emucan.can_error_data.CRC_ERR ) Serial.print(", CRC_ERR");
      if ( emucan.can_error_data.FRM_ERR ) Serial.print(", FRM_ERR");
      if ( emucan.can_error_data.STF_ERR ) Serial.print(", STF_ERR");
      if ( emucan.can_error_data.RX_WRN ) Serial.printf(", RX_WRN: %d", emucan.can_error_data.RX_ERR_COUNTER);
      if ( emucan.can_error_data.TX_WRN ) Serial.printf(", TX_WRN: %d", emucan.can_error_data.TX_ERR_COUNTER);
      Serial.printf(", FLT_CONF: %s\n", (char*)emucan.can_error_data.FLT_CONF);
    }
```

See the EMUcanDiagnoseTest.ino example for details: https://github.com/designer2k2/EMUcanT4/blob/main/examples/EMUcanDiagnoseTest/EMUcanDiagnoseTest.ino

# Others

This Library is tested with a Teesy 4.0 and SN65HVD232 transceiver.

It also is tested on a Teensy 3.5

The EMU Black was running Software Version 2.154.

Arduino IDE Version 1.8.19 and Teensyduino 1.57 was used.

## Different Versions

For Arduino (Nano) with MCP2515 use the EMUcan: https://github.com/designer2k2/EMUcan

For using the ECUMaster serial stream instead of the CAN Bus use this library: https://github.com/GTO2013/EMUSerial

## Support

Please feel free to use/extend/report bugs/request features!

