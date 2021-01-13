// EMUCanT4 Lib Advanced Test Example

// Example to be run on Teensy 4.0 using CAN1
// Configure the EMU Black to send the CAN Stream at 500KBPS


// https://www.designer2k2.at
// Stephan Martin 28.12.2020

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

  //Setup the Callback to receive every CAN Message:
  ReturnAllFramesFunction LetMeHaveIt = specialframefunction;
  emucan.ReturnAllFrames(LetMeHaveIt);

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
      Serial.println(emucan.emu_data.RPM);
      // Check the FLAGS1 if the Engine is Idle:
      if (emucan.emu_data.flags1 & emucan.F_IDLE) {
        Serial.println("Engine Idle active");
      }
    } else {
      Serial.println("No communication from EMU");
    }
    if (emucan.over_run) {
      Serial.println("Overrun! Loosing Data!");
    }
    //Stop sending all frames after 1 second, this spams the serial
    emucan.ReturnAllFramesStop();
  }
}

// self defined function to handle all frames:
void specialframefunction(const CAN_message_t *frame) {
  //Magic things can happen here, but dont block!
  Serial.print(frame->id, HEX); // print ID
  Serial.print(" ");
  Serial.print(frame->len, HEX); // print DLC
  Serial.print(" ");

  for (int i = 0; i < frame->len; i++)  { // print the data
    Serial.print(frame->buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  //Toggle the onboard LED for show:
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
