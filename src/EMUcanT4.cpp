/* Copyright (C) designer2k2 Stephan M.
  # This file is part of EMUcanT4 <https://github.com/designer2k2/EMUcanT4>.
  #
  # EMUcanT4 is free software: you can redistribute it and/or modify
  # it under the terms of the GNU General Public License as published by
  # the Free Software Foundation, either version 3 of the License, or
  # (at your option) any later version.
  #
  # EMUcanT4 is distributed in the hope that it will be useful,
  # but WITHOUT ANY WARRANTY; without even the implied warranty of
  # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  # GNU General Public License for more details.
  #
  # You should have received a copy of the GNU General Public License
  # along with EMUcanT4.  If not, see <http://www.gnu.org/licenses/>.
*/

// Look up the Emu Can Stream documentation in your local installed ECUMASTER Emu Black Software:
// file:///C:/Program%20Files%20(x86)/Ecumaster/EMU%20Black/Help/EN/emuCANStream.html

#include "EMUcanT4.h"

// CAN Things, CAN1 on pin22 pin23 is used at Teensy 4, and CAN0 on Teensy 3:
#include <FlexCAN_T4.h>
#if (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)) || (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41))
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
#else
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can1;
#endif

CAN_message_t canMsg;

EMUcan::EMUcan(const uint32_t EMUbase) {
  //Getting the base number, as set in the EMU Software
  _EMUbase = EMUbase;
}

void EMUcan::begin(const uint32_t canSpeed) {
  can1.begin();
  can1.setBaudRate(canSpeed);
#if (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)) || (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41))
  //on Teensy 4 64 works:
  can1.setMaxMB(64);
#else
  can1.setMaxMB(16);
#endif
  can1.enableFIFO();
  can1.mailboxStatus();
}

bool EMUcan::checkEMUcan() {
  if (can1.read(canMsg)) {
    //Check if Message is within Range of 0-7 from base:
    if ( canMsg.id >= _EMUbase && canMsg.id <= _EMUbase + 7) {
      //So messages here should be decoded!
      decodeEmuFrame(&canMsg);
      //Store the event:
      emucanstatusEngine(EMU_MESSAGE_RECEIVED_VALID);
    }
    if (_returnexists == true) {
      _returnfunction(&canMsg);
    }
  } else {
    emucanstatusEngine(EMU_RECEIVED_NOTHING);
  }
  //Check the can error status:
  if ( can1.error(can_error_data, false) ) {
    can_error_flag = true;
  } else {
    can_error_flag = false;
  }
  return true;
}

void EMUcan::emucanstatusEngine(const EMU_STATUS_UPDATES action) {
  //check the current time versus the last to define the status.
  unsigned long currentMillis = millis();
  switch (action) {
    case EMU_RECEIVED_NOTHING:
      if (currentMillis - _previousMillis >= 1000) {
        EMUcan_Status = EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND;
      }
      break;
    case EMU_MESSAGE_RECEIVED_VALID:
      _previousMillis = currentMillis;
      EMUcan_Status = EMUcan_RECEIVED_WITHIN_LAST_SECOND;
      break;
    default:
      break;
  }
}

bool EMUcan::sendFrame(const CAN_message_t sendframe) {
  if (can1.write(sendframe) != 1) {
    return false;
  }
  return true;
}

void EMUcan::decodeEmuFrame(struct CAN_message_t *msg) {
  //This decodes the frames and fills them into the data:

  //first, check if overrun happened:
  if (msg->flags.overrun) {
    //oh oh!
    over_run = true;
  } else {
    over_run = false;
  }

  //Base:
  if (msg->id == _EMUbase) {
    //0-1 RPM in 16Bit unsigned
    emu_data.RPM = (msg->buf[1] << 8) + msg->buf[0];
    //2 TPS in /2 %
    emu_data.TPS = msg->buf[2] * 0.5;
    //3 IAT 8bit signed -40-127°C
    emu_data.IAT = int8_t(msg->buf[3]);
    //4-5 MAP 16Bit 0-600kpa
    emu_data.MAP = (msg->buf[5] << 8) + msg->buf[4];
    //6-7 INJPW 0-50 0.016129ms
    emu_data.pulseWidth = ((msg->buf[7] << 8) + msg->buf[6]) * 0.016129;
  }
  //Base +1:
  if (msg->id == _EMUbase + 1) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn1 = ((msg->buf[1] << 8) + msg->buf[0]) * 0.0048828125;
    emu_data.analogIn2 = ((msg->buf[3] << 8) + msg->buf[2]) * 0.0048828125;
    emu_data.analogIn3 = ((msg->buf[5] << 8) + msg->buf[4]) * 0.0048828125;
    emu_data.analogIn4 = ((msg->buf[7] << 8) + msg->buf[6]) * 0.0048828125;
  }
  //Base +2:
  if (msg->id == _EMUbase + 2) {
    //0-1 VSPD in 16Bit unsigned 1 kmh/h / bit
    emu_data.vssSpeed = (msg->buf[1] << 8) + msg->buf[0];
    //2 BARO 50-130 kPa
    emu_data.Baro = msg->buf[2];
    //3 OILT 0-160°C
    emu_data.oilTemperature = msg->buf[3];
    //4 OILP BAR 0.0625 bar/bit
    emu_data.oilPressure = msg->buf[4] * 0.0625;
    //5 FUELP BAR 0.0625 bar/bit
    emu_data.fuelPressure = msg->buf[5] * 0.0625;
    //6-7 CLT 16bit Signed -40-250 1 C/bit
    emu_data.CLT = int16_t(((msg->buf[7] << 8) + msg->buf[6]));
  }
  //Base +3:
  if (msg->id == _EMUbase + 3) {
    //0 IGNANG in 8Bit signed    -60 60  0.5deg/bit
    emu_data.IgnAngle = int8_t(msg->buf[0]) * 0.5;
    //1 DWELL 0-10ms 0.05ms/bit
    emu_data.dwellTime = msg->buf[1] * 0.05;
    //2 LAMBDA 8bit 0-2 0.0078125 L/bit
    emu_data.wboLambda = msg->buf[2] * 0.0078125;
    //3 LAMBDACORR 75-125 0.5%
    emu_data.LambdaCorrection = msg->buf[3] * 0.5;
    //4-5 EGT1 16bit °C
    emu_data.Egt1 = ((msg->buf[5] << 8) + msg->buf[4]);
    //6-7 EGT2 16bit °C
    emu_data.Egt2 = ((msg->buf[7] << 8) + msg->buf[6]);
  }
  //Base +4:
  if (msg->id == _EMUbase + 4) {
    //0 GEAR
    emu_data.gear = msg->buf[0];
    //1 ECUTEMP °C
    emu_data.emuTemp = msg->buf[1];
    //2-3 BATT 16bit  0.027 V/bit
    emu_data.Batt = ((msg->buf[3] << 8) + msg->buf[2]) * 0.027;
    //4-5 ERRFLAG 16bit
    emu_data.cel = ((msg->buf[5] << 8) + msg->buf[4]);
    //6 FLAGS1 8bit
    emu_data.flags1 = msg->buf[6];
    //7 ETHANOL %
    emu_data.flexFuelEthanolContent = msg->buf[7];
  }
  //Base +5:
  if (msg->id == _EMUbase + 5) {
    //0 DBW Pos 0.5%
    emu_data.DBWpos = msg->buf[0] * 0.5;
    //1 DBW Target 0.5%
    emu_data.DBWtarget = msg->buf[1] * 0.5;
    //2-3 TC DRPM RAW 16bit  1/bit
    emu_data.TCdrpmRaw = ((msg->buf[3] << 8) + msg->buf[2]);
    //4-5 TC DRPM 16bit  1/bit
    emu_data.TCdrpm = ((msg->buf[5] << 8) + msg->buf[4]);
    //6 TC Torque reduction %
    emu_data.TCtorqueReduction = msg->buf[6];
    //7 Pit Limit Torque reduction %
    emu_data.PitLimitTorqueReduction = msg->buf[7];
  }
  //Base +6:
  if (msg->id == _EMUbase + 6) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn5 = ((msg->buf[1] << 8) + msg->buf[0]) * 0.0048828125;
    emu_data.analogIn6 = ((msg->buf[3] << 8) + msg->buf[2]) * 0.0048828125;
    emu_data.outflags1 = msg->buf[4];
    emu_data.outflags2 = msg->buf[5];
    emu_data.outflags3 = msg->buf[6];
    emu_data.outflags4 = msg->buf[7];
  }
  //Base +7:
  if (msg->id == _EMUbase + 7) {
    //0-1 Boost target 16bit 0-600 kPa
    emu_data.boostTarget = ((msg->buf[1] << 8) + msg->buf[0]);
    //2 PWM#1 DC 1%/bit
    emu_data.pwm1 = msg->buf[2];
    //3 DSG mode 2=P 3=R 4=N 5=D 6=S 7=M 15=fault
    emu_data.DSGmode = msg->buf[3];
    //since version 143 this contains more data, check lenght:
    if (msg->len == 8) {
      //4 Lambda target 8bit 0.01%/bit
      emu_data.lambdaTarget = msg->buf[4] * 0.01;
      //5 PWM#2 DC 1%/bit
      emu_data.pwm2 = msg->buf[5];
      //6-7 Fuel used 16bit 0.01L/bit
      emu_data.fuel_used = ((msg->buf[7] << 8) + msg->buf[6]) * 0.01;
    }
  }
}

bool EMUcan::decodeCel() {
  //Returns true if an CEL error is on:
  if ( emu_data.cel & EFLG_ERRORMASK ) {
    return true;
  } else {
    return false;
  }
}

void EMUcan::ReturnAllFrames (ReturnAllFramesFunction response) {
  _returnfunction = response;
  _returnexists = true;
}

void EMUcan::ReturnAllFramesStop() {
  _returnexists = false;
}

void EMUcan::mailboxStatus() {
  can1.mailboxStatus();
}

void EMUcan::setClock(FLEXCAN_CLOCK clock) {
#if (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)) || (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41))
  // This only works on Teensy 4
  can1.setClock(clock);
#endif
}