#ifndef MT_DEVICE_DEFINES_H
#define MT_DEVICE_DEFINES_H

#define commandSoftwareCmdHex 0x21 // 0x21 ->ASCII !
  #define commandStartTimeHex 0x81
    #define responseStartTimeHex 0x91
  #define commandEndTimeHex 0x82
    #define responseEndTimeHex 0x92
  #define commandSplitTimeHex 0x83
    #define responseSplitTimeHex 0x93
  #define commandResetSoftwareHex 0x31
  #define commandSkipTimeHex 0x32
  #define commandGetTimesHex 0x33
  #define commandConnectDevicesHex 0x34

#define commandSystemCmd 0x20
  #define commandSystemStatusQueryHex 0x15
    #define responseSystemStatusQueryHex 0x16
  #define commandSystemBatteryQueryHex 0x17
    #define responseSystemBatteryQueryHex 0x18
  #define commandSystemPingHex 0x13
    #define responseSystemPingHex 0x14
  #define commandSystemSetStateConnected 0x19

#define commandMasterOnPc 0xF0
#define commandMasterOnWifi 0xF1
#define commandMasterOnLocal 0xF2

#define confirmCmd 0xFF

#endif
 // MT_DEVICE_DEFINES_H

