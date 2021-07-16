// EMUCanT4 Lib Diagnose Test Example

// Example to be run on Teensy 4.0 using CAN1
// Configure the EMU Black to send the CAN Stream at 500KBPS

// https://www.designer2k2.at
// Stephan Martin 16.07.2021

#include <EMUcanT4.h>

//Define the EMU Can Base at 600):
EMUcan emucan(0x600);

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200); //set Teensy USB Type to "Serial"

  //Call this in the setup to init the lib:
  emucan.begin(500000);
  //Check mailbox status:
  emucan.mailboxStatus();
}

void loop() {
  // This function should be called as often as possible:
  emucan.checkEMUcan();

  // only check every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (emucan.EMUcan_Status == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.println(emucan.emu_data.RPM);
    } else {
      Serial.println("No communication from EMU");
    }
    
    if (emucan.over_run) {
      //Overrun indicates that the checkEMUcan function is not called often enough.
      Serial.println("Overrun! Loosing Data!");
    }

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
  }
}
