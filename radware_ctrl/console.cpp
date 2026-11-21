// =====================================================================
//  console.cpp - Commandes texte (moniteur série Arduino, 38400 bauds,
//  fin de ligne "Nouvelle ligne" ou "Les deux NL & CR")
// =====================================================================
#include "app.h"
#include "settings.h"
#include <ctype.h>

#define IS(x) (strcmp_P(tok, PSTR(x)) == 0)

void printHelp(Print& p) {
  p.print(F(
    "\r\n=== Radware / Arduino  v" FW_VERSION " ===\r\n"
    "Frequences : 7100000 (Hz) | 7100k | 7.1m | 7.1 (MHz)\r\n"
    "  a <f>  | b <f>            regler le LO du recepteur A / B\r\n"
    "  st                        etat des 2 recepteurs\r\n"
    "  plan <f>                  calcule les reglages sans rien ecrire\r\n"
    "  pre <a|b> <1-8|auto|off>  filtre de bande F1..F8 force / automatique / coupe\r\n"
    "  gain <a|b> <0-3>          bit0=GAIN 20, bit1=GAIN 0\r\n"
    "  ant <a|b> <rx|tx> <on|off> alimentation DC J5 (RX) / J4 (TX)\r\n"
    "  txq <a|b> <on|off>        ligne TX QUAD CONTROL\r\n"
    "  fa <a|b> <0-7>            force le diviseur SP8402 (ratio 2^(n+1))\r\n"
    "  dds <a|b> <f>             DDS a une frequence brute (test)\r\n"
    "  ftw <a|b> <hex>           mot de frequence AD7008 brut (test)\r\n"
    "  reset <a|b>               reset AD7008 + reprogrammation\r\n"
    "  cal                       affiche le calibrage\r\n"
    "  cal ref <Hz> | n <80|81> | vco <min> <max> | ddsmax <Hz>\r\n"
    "  cal base <a|b> <hex> | dds16 <0|1>\r\n"
    "  trace <on|off>            affiche chaque ecriture de bus\r\n"
    "  dry <on|off>              calcule sans ecrire sur le bus\r\n"
    "  restore <on|off>          reapplique les frequences au demarrage\r\n"
    "  save | load | defaults    EEPROM\r\n"
    "  test                      MODE TEST (menu interactif)\r\n"
    "CAT Kenwood (finissant par ;) : FA / FB / IF / ID / MD ...\r\n"));
}

void printStatusAll(Print& p) {
  gRx[0]->printStatus(p);
  gRx[1]->printStatus(p);
}

static RadwareCard* rxArg(const char* t) {
  if (!t) return nullptr;
  if (t[0] == 'a' && !t[1]) return gRx[0];
  if (t[0] == 'b' && !t[1]) return gRx[1];
  return nullptr;
}

static uint8_t rxIndex(RadwareCard* c) { return c == gRx[1] ? 1 : 0; }

static bool onOff(const char* t, bool& v) {
  if (!t) return false;
  if (!strcmp_P(t, PSTR("on")) || !strcmp_P(t, PSTR("1"))) { v = true; return true; }
  if (!strcmp_P(t, PSTR("off")) || !strcmp_P(t, PSTR("0"))) { v = false; return true; }
  return false;
}

static void printPlan(Print& p, const TunePlan& t) {
  if (!t.ok) { p.print(F("  IMPOSSIBLE : ")); p.println(t.err); return; }
  p.print(F("  bande ")); printBandName(p, t.band);
  p.print(F(" | FA=")); p.print(t.faCode);
  p.print(F(" (/")); p.print(t.ratioM);
  p.print(F(") | VCO ")); printHz(p, t.vcoHz);
  p.print(F(" | DDS ")); printHz(p, t.ddsHz);
  p.print(F(" | FTW 0x")); p.print(t.ftw, HEX);
  p.print(F(" | LO reel ")); printHz(p, t.actualLoHz);
  p.println();
}

static void doTune(Link& l, RadwareCard* c, const char* arg) {
  Print& p = *l.io;
  uint32_t hz;
  if (!arg) { c->printStatus(p); return; }
  if (!parseFreq(arg, hz)) { p.println(F("Frequence invalide (ex: 7100k, 7.1m, 7100000)")); return; }
  if (!c->tune(hz)) { p.print(F("Refuse : ")); p.println(c->plan().err); return; }
  gSet.rx[rxIndex(c)].freqHz = hz;
  p.print(F("RX ")); p.print(c->name()); p.print(F(" -> ")); printHz(p, hz); p.println();
  printPlan(p, c->plan());
}

static void doCal(Link& l, char* t1, char* t2, char* t3) {
  Print& p = *l.io;
  RadwareCal& c = gSet.cal;
  uint32_t v, w;
  if (!t1) {
    p.print(F("fref=")); p.print(c.frefHz);
    p.print(F(" Hz  N=")); p.print(c.pllN);
    p.print(F("  VCO=")); p.print(c.vcoMinHz); p.print('-'); p.print(c.vcoMaxHz);
    p.print(F(" Hz  DDSmax=")); p.print(c.ddsMaxHz);
    p.print(F(" Hz  dds16=")); p.println(c.dds16);
    p.print(F("baseA=0x")); p.print(c.baseA, HEX);
    p.print(F("  baseB=0x")); p.println(c.baseB, HEX);
    return;
  }
  if (!strcmp_P(t1, PSTR("ref")) && t2 && parseUInt(t2, v) && v >= 1000000UL && v <= 60000000UL) {
    c.frefHz = v; gRx[0]->retune(); gRx[1]->retune(); p.println(F("OK")); return;
  }
  if (!strcmp_P(t1, PSTR("n")) && t2 && parseUInt(t2, v) && (v == 80 || v == 81)) {
    c.pllN = (uint8_t)v; gRx[0]->retune(); gRx[1]->retune(); p.println(F("OK")); return;
  }
  if (!strcmp_P(t1, PSTR("vco")) && t2 && t3 && parseFreq(t2, v) && parseFreq(t3, w) && v < w) {
    c.vcoMinHz = v; c.vcoMaxHz = w; p.println(F("OK")); return;
  }
  if (!strcmp_P(t1, PSTR("ddsmax")) && t2 && parseFreq(t2, v)) {
    c.ddsMaxHz = v; p.println(F("OK")); return;
  }
  if (!strcmp_P(t1, PSTR("dds16")) && t2 && parseUInt(t2, v) && v <= 1) {
    c.dds16 = (uint8_t)v;
    gRx[0]->ddsReset(); gRx[1]->ddsReset(); gRx[0]->retune(); gRx[1]->retune();
    p.println(F("OK (AD7008 reprogramme)")); return;
  }
  if (!strcmp_P(t1, PSTR("base")) && t2 && t3 && parseUInt(t3, v, 16) && v <= 0x3FF) {
    RadwareCard* r = rxArg(t2);
    if (r) {
      if (r == gRx[0]) c.baseA = (uint16_t)v; else c.baseB = (uint16_t)v;
      r->setBase((uint16_t)v); r->begin();
      p.println(F("OK")); return;
    }
  }
  p.println(F("cal : argument invalide (tapez aide)"));
}

void consoleHandle(Link& l, char* line) {
  Print& p = *l.io;
  // minuscules + découpage en jetons (max 5)
  for (char* q = line; *q; ++q) *q = (char)tolower(*q);
  char* tk[5] = {0, 0, 0, 0, 0};
  uint8_t nt = 0;
  char* s = line;
  while (*s && nt < 5) {
    while (*s == ' ' || *s == '\t') s++;
    if (!*s) break;
    tk[nt++] = s;
    while (*s && *s != ' ' && *s != '\t') s++;
    if (*s) *s++ = 0;
  }
  if (!nt) return;
  char* tok = tk[0];
  char* a1 = tk[1];
  char* a2 = tk[2];
  char* a3 = tk[3];

  if (IS("aide") || IS("help") || IS("?") || IS("h")) { printHelp(p); return; }
  if (IS("st") || IS("status")) { printStatusAll(p); return; }
  if (IS("a")) { doTune(l, gRx[0], a1); return; }
  if (IS("b")) { doTune(l, gRx[1], a1); return; }

  if (IS("plan")) {
    uint32_t hz;
    if (a1 && parseFreq(a1, hz)) printPlan(p, planTune(hz, gSet.cal));
    else p.println(F("plan <frequence>"));
    return;
  }

  if (IS("test")) { l.inTest = true; runTestMode(l); l.inTest = false; return; }

  if (IS("trace")) {
    bool v; if (onOff(a1, v)) { gBus.setTrace(v, &p); p.println(F("OK")); } else p.println(F("trace on|off"));
    return;
  }
  if (IS("dry")) {
    bool v; if (onOff(a1, v)) { gBus.setDry(v); p.println(v ? F("OK : aucune ecriture sur le bus") : F("OK : ecritures reelles")); }
    else p.println(F("dry on|off"));
    return;
  }
  if (IS("restore")) {
    bool v; if (onOff(a1, v)) { gSet.restoreAtBoot = v; p.println(F("OK (pensez a save)")); } else p.println(F("restore on|off"));
    return;
  }
  if (IS("cal")) { doCal(l, a1, a2, a3); return; }

  if (IS("save")) {
    for (uint8_t i = 0; i < 2; i++) {
      gSet.rx[i].freqHz = gRx[i]->loHz() ? gRx[i]->loHz() : gSet.rx[i].freqHz;
      gSet.rx[i].gain = gRx[i]->gain();
      gSet.rx[i].flags = (gRx[i]->rxPower() ? 1 : 0) | (gRx[i]->txPower() ? 2 : 0) | (gRx[i]->txQuad() ? 4 : 0);
    }
    settingsSave(gSet); p.println(F("Sauvegarde en EEPROM")); return;
  }
  if (IS("load")) {
    if (settingsLoad(gSet)) { applySettingsToCards(); p.println(F("EEPROM chargee")); }
    else p.println(F("EEPROM vide/invalide : valeurs par defaut"));
    return;
  }
  if (IS("defaults")) {
    settingsDefaults(gSet); applySettingsToCards(); p.println(F("Valeurs par defaut (non sauvegardees)")); return;
  }

  // ---- commandes à argument récepteur ----
  RadwareCard* c = rxArg(a1);
  if (c) {
    uint32_t v;
    if (IS("pre")) {
      if (!a2) { p.println(F("pre <a|b> <1-8|auto|off>")); return; }
      uint8_t i = rxIndex(c);
      if (!strcmp_P(a2, PSTR("auto"))) { c->setPreselAuto(true); gSet.rx[i].preselManual = 0; if (c->loHz()) c->tune(c->loHz()); p.println(F("Filtre automatique")); return; }
      if (!strcmp_P(a2, PSTR("off")))  { c->setPreselAuto(false); c->setPreselMask(0); p.println(F("Tous les filtres coupes (manuel)")); return; }
      if (parseUInt(a2, v) && v >= 1 && v <= 8) {
        c->setPreselAuto(false); c->setPreselBand((uint8_t)(v - 1)); gSet.rx[i].preselManual = (uint8_t)v;
        p.print(F("Filtre force : ")); printBandName(p, (uint8_t)(v - 1)); p.println(); return;
      }
      p.println(F("pre <a|b> <1-8|auto|off>")); return;
    }
    if (IS("gain")) {
      if (a2 && parseUInt(a2, v) && v <= 3) { c->setGain((uint8_t)v); p.println(F("OK")); } else p.println(F("gain <a|b> <0-3>"));
      return;
    }
    if (IS("ant")) {
      bool on;
      if (a2 && a3 && onOff(a3, on)) {
        if (!strcmp_P(a2, PSTR("rx"))) { c->setRxPower(on); p.println(F("OK (+12 V sur J5 si on)")); return; }
        if (!strcmp_P(a2, PSTR("tx"))) { c->setTxPower(on); p.println(F("OK (+12 V sur J4 si on)")); return; }
      }
      p.println(F("ant <a|b> <rx|tx> <on|off>")); return;
    }
    if (IS("txq")) {
      bool on; if (a2 && onOff(a2, on)) { c->setTxQuad(on); p.println(F("OK")); } else p.println(F("txq <a|b> <on|off>"));
      return;
    }
    if (IS("fa")) {
      if (a2 && parseUInt(a2, v) && v <= 7) { c->setFA((uint8_t)v); p.print(F("FA=")); p.print(v); p.print(F(" -> diviseur /")); p.println(2UL << v); }
      else p.println(F("fa <a|b> <0-7>"));
      return;
    }
    if (IS("dds")) {
      if (a2 && parseFreq(a2, v)) { c->ddsRawHz(v); p.print(F("DDS = ")); printHz(p, v); p.println(); }
      else p.println(F("dds <a|b> <frequence>"));
      return;
    }
    if (IS("ftw")) {
      if (a2 && parseUInt(a2, v, 16)) { c->ddsLoadFtw(v); p.print(F("FTW = 0x")); p.println(v, HEX); }
      else p.println(F("ftw <a|b> <hex>"));
      return;
    }
    if (IS("reset")) { c->ddsReset(); c->retune(); p.println(F("AD7008 reinitialise")); return; }
  }

  p.println(F("Commande inconnue (tapez aide)"));
}
