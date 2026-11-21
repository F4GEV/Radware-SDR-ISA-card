#include "settings.h"
#include <EEPROM.h>
#include <stddef.h>

static const uint16_t kMagic = 0x52D7;
static const uint8_t  kVersion = 1;

static uint8_t sum8(const Settings& s) {
  const uint8_t* p = (const uint8_t*)&s;
  uint8_t c = 0x5A;
  for (size_t i = 0; i < offsetof(Settings, checksum); i++) c = (uint8_t)((c << 1 | c >> 7) ^ p[i]);
  return c;
}

void settingsDefaults(Settings& s) {
  memset(&s, 0, sizeof(s));
  s.magic = kMagic;
  s.version = kVersion;
  calDefaults(s.cal);
  s.restoreAtBoot = 0;
}

bool settingsLoad(Settings& s) {
  Settings t;
  EEPROM.get(0, t);
  if (t.magic != kMagic || t.version != kVersion || t.checksum != sum8(t)) {
    settingsDefaults(s);
    return false;
  }
  s = t;
  return true;
}

void settingsSave(Settings& s) {
  s.magic = kMagic;
  s.version = kVersion;
  s.checksum = sum8(s);
  EEPROM.put(0, s);
}
