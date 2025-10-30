#include "pinmap.h"

void setup()
{
  Serial.begin(9600);
  nexSerial.begin(115200);
  Serial1.begin(115200);
  
  InitializePins();
  //InitializeRTC();
}

void loop()
{
  PinToDisplay();
  //TestPins();
  //ProcessPins();
  EStop_Effect();
  //RTCHandler();
  //nexLoop(nex_listen_list);
}
