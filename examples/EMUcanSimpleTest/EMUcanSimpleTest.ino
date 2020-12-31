// EMUCanT4 Lib Simple Test Example

// Example to be run on Teensy 4.0 using CAN1
// Configure the EMU Black to send the CAN Stream at 500KBPS


// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcanT4.h>

EMUcan emucan;

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  Serial.begin(115200); //set Teensy USB Type to "Serial"

  //Call this in the setup to init the lib:
  emucan.begin(500000);

  Serial.println("------- CAN Read ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.print(emucan.emu_data.RPM);
      Serial.print(";");
      Serial.print(emucan.emu_data.TPS);
      Serial.print(";");
      Serial.print(emucan.emu_data.IAT);
      Serial.print(";");
      Serial.print(emucan.emu_data.MAP);
      Serial.print(";");
      Serial.println(emucan.emu_data.pulseWidth);
    } else {
      Serial.println("No communication from EMU");
    }
  }
}
