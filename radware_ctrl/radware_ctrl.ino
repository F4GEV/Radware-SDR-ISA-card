// =====================================================================
//  radware_ctrl.ino
//  Pilotage par Arduino de 1 ou 2 cartes ISA 16 bits "Radware" (SDR HF)
//
//   - 1 Arduino MEGA 2560 branché directement sur le bus ISA = maître de bus
//   - carte 1 = récepteur A, carte 2 = récepteur B (réglées par SW1)
//   - AD7008 cadencé par une référence 10 MHz appliquée sur ISA B30
//   - console texte + CAT Kenwood (OmniRig / HDSDR) sur le port USB
//   - mode test interactif : tapez  test
//
//  Fichiers : config.h (réglages), isa_bus, radware (matériel), settings,
//             cat, console, selftest, link.
// =====================================================================
#include "app.h"

IsaBus       gBus;
Settings     gSet;
RadwareCard  cardA(gBus, 'A', gSet.cal);
RadwareCard  cardB(gBus, 'B', gSet.cal);
RadwareCard* gRx[2] = { &cardA, &cardB };

static Link usb;
#if ENABLE_CAT_PORT2
static Link port2;
#endif

void applySettingsToCards() {
  cardA.setBase(gSet.cal.baseA);
  cardB.setBase(gSet.cal.baseB);
  for (uint8_t i = 0; i < 2; i++) {
    RadwareCard* c = gRx[i];
    c->begin();
    c->setGain(gSet.rx[i].gain & 3);
    c->setRxPower(gSet.rx[i].flags & 1);
    c->setTxPower(gSet.rx[i].flags & 2);
    c->setTxQuad(gSet.rx[i].flags & 4);
    if (gSet.rx[i].preselManual) {
      c->setPreselAuto(false);
      c->setPreselBand(gSet.rx[i].preselManual - 1);
    } else {
      c->setPreselAuto(true);
    }
    if (gSet.restoreAtBoot && gSet.rx[i].freqHz) c->tune(gSet.rx[i].freqHz);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  gBus.begin();
  bool ok = settingsLoad(gSet);
  applySettingsToCards();

  linkInit(usb, Serial, 0, 1);              // FA -> récepteur A, FB -> récepteur B
#if ENABLE_CAT_PORT2
  Serial1.begin(CAT2_BAUD);
  linkInit(port2, Serial1, 1, 1);           // 2e rig OmniRig : tout sur le récepteur B
#endif

  Serial.print(F("\r\nRadware/Arduino v" FW_VERSION "  (EEPROM "));
  Serial.print(ok ? F("ok") : F("vide : valeurs par defaut"));
  Serial.println(F(")  tapez 'aide'"));
}

void loop() {
  linkPoll(usb);
#if ENABLE_CAT_PORT2
  linkPoll(port2);
#endif
}
