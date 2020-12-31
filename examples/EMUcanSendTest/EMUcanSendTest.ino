// EMUCanT4 Lib Simple Send Example

// Example to be run on Teensy 4.0 using CAN1
// Configure the EMU Black to send the CAN Stream at 500KBPS

// Configure custom CAN messages in the EMU Black:
// Use this file: EMUBlackCANStreamExample.canstr
// It will receive the values as CAN Analog 1 and 2.

// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcanT4.h>

EMUcan emucan;

CAN_message_t canMsg1;

unsigned long previousMillis = 0;
const long interval = 100;

byte countUp = 0;

void setup() {
  Serial.begin(115200); //set Teensy USB Type to "Serial"

  //Call this in the setup to init the lib:
  emucan.begin(500000);

  // Frame to be send:
  canMsg1.id  = 0x0F6;
  canMsg1.len = 2;
  canMsg1.buf[0] = 0xFF;
  canMsg1.buf[1] = 0x00;

  Serial.println("------- CAN Send ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    countUp++;  //Byte, so overflow at 255
    canMsg1.buf[0] = 255 - countUp;
    canMsg1.buf[1] = countUp;

    //Sends the frame;
    emucan.sendFrame(canMsg1);

    Serial.print("Message sent: ");
    Serial.println(countUp);

  }
}