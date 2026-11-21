#include "Arduino.h"
#include "SPI.h"
#include "EEPROM.h"
MockSerial Serial; unsigned long g_ms=0; PinHook g_pinHook=nullptr; SPIClass SPI; EEPROMClass EEPROM;
#include "config.h"
#include "../radware_ctrl/radware_ctrl.ino"
int main(int argc,char**argv){ setup(); Serial.take();
  for(int i=1;i<argc;i++){ Serial.feed(argv[i]); Serial.feed("\n"); loop(); }
  printf("%s", Serial.take().c_str()); }
