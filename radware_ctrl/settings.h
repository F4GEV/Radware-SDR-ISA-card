// =====================================================================
//  settings.h - Réglages persistants (EEPROM)
// =====================================================================
#pragma once
#include <Arduino.h>
#include "radware.h"

struct RxSettings {
  uint32_t freqHz;       // dernière fréquence LO
  uint8_t  gain;         // bit0 GAIN20, bit1 GAIN0
  uint8_t  flags;        // bit0 RX power, bit1 TX power, bit2 TX quad
  uint8_t  preselManual; // 0 = automatique, 1..8 = filtre F1..F8 forcé
};

struct Settings {
  uint16_t   magic;
  uint8_t    version;
  RadwareCal cal;
  RxSettings rx[2];
  uint8_t    restoreAtBoot;   // 1 = réappliquer les fréquences au démarrage
  uint8_t    checksum;
};

void settingsDefaults(Settings& s);
bool settingsLoad(Settings& s);     // false si EEPROM vide / invalide (s reçoit les défauts)
void settingsSave(Settings& s);
