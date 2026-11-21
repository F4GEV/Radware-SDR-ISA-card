// =====================================================================
//  selftest.cpp - MODE TEST : chaque fonction de la carte peut être
//  vérifiée séparément, avec voltmètre / oscilloscope / compteur.
//  (La carte est en écriture seule : aucune lecture de retour possible,
//   c'est donc l'opérateur qui constate l'effet. Chaque test affiche
//   quoi mesurer et où.)
// =====================================================================
#include "app.h"
#include <ctype.h>

static Stream* io;

// ---- entrées utilisateur ----
static int readKeyTimeout(unsigned long ms) {
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    if (io->available()) return io->read();
  }
  return -1;
}

// Jette les CR/LF restés dans le tampon (fin de ligne de la saisie précédente).
static void drainEol() {
  delay(20);
  while (io->available() && (io->peek() == '\r' || io->peek() == '\n')) io->read();
}

// Attend Entrée. Retourne false si l'utilisateur tape 'x' (abandon) ou après 10 min d'inactivité.
static bool waitEnter() {
  io->print(F("   [Entree = suite, x = retour menu] "));
  for (uint8_t idle = 0; idle < 10;) {
    int c = readKeyTimeout(60000UL);
    if (c < 0) { idle++; continue; }
    if (c == 'x' || c == 'X' || c == 'q') { io->println(); drainEol(); return false; }
    if (c == '\n' || c == '\r') { io->println(); drainEol(); return true; }
  }
  return false;
}

// Attend une pause de ms ; renvoie false si 'x' reçu pendant l'attente.
static bool pauseOrAbort(unsigned long ms) {
  int c = readKeyTimeout(ms);
  if (c == 'x' || c == 'X' || c == 'q') { drainEol(); return false; }
  return true;
}

static bool readLine(char* out, uint8_t max) {
  uint8_t n = 0;
  for (;;) {
    int c = readKeyTimeout(120000UL);
    if (c < 0) return false;
    if (c == '\r' || c == '\n') { if (n) { out[n] = 0; io->println(); return true; } continue; }
    if (n < max - 1) { out[n++] = (char)c; io->print((char)c); }
  }
}

static RadwareCard* askRx() {
  io->print(F("Recepteur a tester (a/b) ? "));
  for (;;) {
    int c = readKeyTimeout(120000UL);
    if (c < 0) return nullptr;
    c = tolower(c);
    if (c == 'a') { io->println(F("A")); drainEol(); return gRx[0]; }
    if (c == 'b') { io->println(F("B")); drainEol(); return gRx[1]; }
    if (c == 'x') { io->println(); drainEol(); return nullptr; }
  }
}

static bool confirm(const __FlashStringHelper* q) {
  io->print(q);
  io->print(F(" (o/n) "));
  for (;;) {
    int c = readKeyTimeout(120000UL);
    if (c < 0) return false;
    c = tolower(c);
    if (c == 'o' || c == 'y') { io->println(F("oui")); drainEol(); return true; }
    if (c == 'n') { io->println(F("non")); drainEol(); return false; }
  }
}

static void isaPinName(char* out, size_t n, bool isData, uint8_t bit) {
  if (isData) {
    if (bit < 8) snprintf(out, n, "SD%u  (ISA A%u)", bit, 9 - bit);
    else         snprintf(out, n, "SD%u (ISA C%u)", bit, 11 + (bit - 8));
  } else {
    snprintf(out, n, "SA%u  (ISA A%u)", bit, 31 - bit);
  }
}

// ----------------------------------------------------------------------
//  1. Bus : lignes une par une, /IOW reste inactif -> aucune écriture
// ----------------------------------------------------------------------
static void testBus(RadwareCard* c) {
  io->println(F("\r\n[1] BUS : une ligne a la fois a l'etat haut, /IOW reste haut."));
  io->println(F("    Sondez le connecteur ISA (logique 5 V). 'x' pour finir."));
  char nm[24];
  gBus.outputsEnable(true);
  for (uint8_t i = 0; i < 10; i++) {
    gBus.present(1u << i, 0);
    isaPinName(nm, sizeof(nm), false, i);
    io->print(F("   ")); io->println(nm);
    if (!pauseOrAbort(1200)) goto done;
  }
  for (uint8_t i = 0; i < 16; i++) {
    gBus.present(0, 1u << i);
    isaPinName(nm, sizeof(nm), true, i);
    io->print(F("   ")); io->println(nm);
    if (!pauseOrAbort(1200)) goto done;
  }
done:
  gBus.present(0, 0);
  io->println(F("    Bus remis a 0."));
  (void)c;
}

// ----------------------------------------------------------------------
//  2. /IOW : impulsions vers base+6 (Y3 non câblé = aucun effet)
// ----------------------------------------------------------------------
static void testStrobe(RadwareCard* c) {
  io->print(F("\r\n[2] /IOW : ecritures repetees a l'adresse 0x"));
  io->print(c->base() + rw::REG_NUL, HEX);
  io->println(F(" (sortie Y3 du 74HC238, non cablee)."));
  io->println(F("    Oscillo sur ISA B13 : impulsions basses de ~3 us. Sur la carte :"));
  io->println(F("    U22 pin 12 (Y3) ne doit pas bouger jusqu'a ce que /IOW passe a 0 : rien d'autre."));
  io->println(F("    Pendant 10 s (ou 'x')..."));
  unsigned long t0 = millis();
  uint16_t d = 0xAAAA;
  while (millis() - t0 < 10000UL) {
    c->dummyWrite(d);
    d = ~d;
    if (io->available()) { int k = io->read(); if (k == 'x') break; }
    delay(1);
  }
  io->println(F("    Fin."));
}

// ----------------------------------------------------------------------
//  3. Préselecteurs
// ----------------------------------------------------------------------
static void testPresel(RadwareCard* c) {
  io->println(F("\r\n[3] PRESELECTEURS : un filtre a la fois (UCN5815 U2)."));
  io->println(F("    Mesurez la sortie OUTn de U2 (pin 25 = OUT1 ... pin 18 = OUT8)"));
  io->println(F("    ou le courant de polarisation des PIN D1/D2 du filtre."));
  c->setPreselAuto(false);
  for (uint8_t b = 0; b < 8; b++) {
    c->setPreselBand(b);
    io->print(F("   ")); printBandName(*io, b);
    io->print(F("  -> sortie OUT")); io->print(8 - b);
    io->print(F(" (U2 pin ")); io->print(b == 0 ? 18 : 26 - (8 - b)); io->print(F(")  masque=0x"));
    io->println(rw::bandMask(b), HEX);
    if (!pauseOrAbort(2500)) break;
  }
  c->setPreselMask(0);
  c->setPreselAuto(true);
  io->println(F("    Filtres coupes, mode automatique retabli."));
}

// ----------------------------------------------------------------------
//  4. Diviseur SP8402 (FA1..FA3) : DDS fixé à 2 MHz -> VCO 160 MHz
// ----------------------------------------------------------------------
static void testDivider(RadwareCard* c) {
  const uint32_t f = 2000000UL;
  io->println(F("\r\n[4] DIVISEUR SP8402 (U8) : DDS = 2.000000 MHz -> VCO attendu N x 2 MHz."));
  io->println(F("    Compteur sur J6 (moniteur VCO) : doit lire N x 2 MHz (160 MHz si N=80)."));
  io->println(F("    La sortie du SP8402 (U8 pin 21 ou 20) vaut VCO / ratio."));
  c->ddsRawHz(f);
  const uint32_t vco = f * gSet.cal.pllN;
  for (uint8_t k = 0; k < 8; k++) {
    c->setFA(k);
    uint32_t M = 2UL << k;
    io->print(F("   FA=")); io->print(k);
    io->print(F("  S2S1S0=")); io->print((k >> 2) & 1); io->print((k >> 1) & 1); io->print(k & 1);
    io->print(F("  ratio /")); io->print(M);
    io->print(F("  sortie U8 attendue = ")); printHz(*io, vco / M);
    io->print(F("  LO (/4) = ")); printHz(*io, vco / M / 4);
    io->println();
    if (!pauseOrAbort(3000)) break;
  }
  c->setFA(0);
  c->retune();
}

// ----------------------------------------------------------------------
//  5. DDS AD7008
// ----------------------------------------------------------------------
static void testDds(RadwareCard* c) {
  io->println(F("\r\n[5] DDS AD7008 (U16) : frequences fixes."));
  io->println(F("    Oscillo sur le noeud 'DDS' (cote C122 / entree CIN du 4046) : sinus filtre."));
  io->println(F("    Si aucun signal : essayez 'cal dds16 0' (bus 8 bits) puis relancez ce test."));
  static const uint32_t fl[] = { 500000UL, 1000000UL, 1500000UL, 2000000UL, 2500000UL, 3000000UL };
  for (uint8_t i = 0; i < sizeof(fl) / sizeof(fl[0]); i++) {
    c->ddsRawHz(fl[i]);
    io->print(F("   DDS = ")); printHz(*io, fl[i]);
    io->print(F("   VCO attendu (PLL verrouillee) = ")); printHz(*io, fl[i] * gSet.cal.pllN);
    io->println();
    if (!pauseOrAbort(4000)) break;
  }
  c->retune();
}

// ----------------------------------------------------------------------
//  6. Lignes de contrôle
// ----------------------------------------------------------------------
static void testControls(RadwareCard* c) {
  io->println(F("\r\n[6] LIGNES DE CONTROLE (U17/U18)."));
  if (confirm(F("    Tester RX POWER / TX POWER ? Cela met +12 V sur J5/J4 : antennes DEBRANCHEES ?"))) {
    c->setRxPower(true);
    io->println(F("   RX POWER ON  : +12 V (via L45) sur le centre de J5. Mesurez."));
    if (waitEnter()) {
      c->setRxPower(false);
      c->setTxPower(true);
      io->println(F("   TX POWER ON  : +12 V (via L4) sur le centre de J4. Mesurez."));
      waitEnter();
    }
    c->setRxPower(false); c->setTxPower(false);
  }
  c->setTxQuad(true);
  io->println(F("   TX QUAD CONTROL = 1 (U17 pin 19). Mesurez ; la polarite RX/TX est a constater."));
  waitEnter();
  c->setTxQuad(false);
  c->setGain(1);
  io->println(F("   GAIN 20 seul (U18 pin 2). Observez l'amplitude I/Q sur la sortie audio."));
  if (!waitEnter()) { c->setGain(0); return; }
  c->setGain(2);
  io->println(F("   GAIN 0 seul (U18 pin 5)."));
  if (!waitEnter()) { c->setGain(0); return; }
  c->setGain(3);
  io->println(F("   GAIN 20 + GAIN 0."));
  if (!waitEnter()) { c->setGain(0); return; }
  c->setGain(0);
  c->setFsel(true);
  io->println(F("   FSEL = 1 (AD7008 pin 31) : choisit FREQ1 (non programme -> sortie differente)."));
  waitEnter();
  c->setFsel(false);
  io->println(F("   Impulsion RESET AD7008 + reprogrammation..."));
  c->ddsReset();
  c->retune();
}

// ----------------------------------------------------------------------
//  7. LO complet + balayage
// ----------------------------------------------------------------------
static void printPlanLine(const TunePlan& t) {
  io->print(F("   ")); printBandName(*io, t.band);
  io->print(F(" FA=")); io->print(t.faCode);
  io->print(F(" /")); io->print(t.ratioM);
  io->print(F(" VCO=")); printHz(*io, t.vcoHz);
  io->print(F(" DDS=")); printHz(*io, t.ddsHz);
  io->print(F(" LO=")); printHz(*io, t.actualLoHz);
  io->println();
}

static void testLo(RadwareCard* c) {
  io->println(F("\r\n[7] LO COMPLET : balayage entre deux frequences."));
  char b[20]; uint32_t f1, f2, st;
  io->print(F("   Frequence de depart ? (ex 7000k) "));
  if (!readLine(b, sizeof(b)) || !parseFreq(b, f1)) { io->println(F("annule")); return; }
  io->print(F("   Frequence de fin ? "));
  if (!readLine(b, sizeof(b)) || !parseFreq(b, f2)) { io->println(F("annule")); return; }
  io->print(F("   Pas ? (ex 100k) "));
  if (!readLine(b, sizeof(b)) || !parseFreq(b, st) || st == 0) { io->println(F("annule")); return; }
  io->println(F("   500 ms par pas ; 'x' pour arreter. Surveillez le spectre I/Q et J6."));
  for (uint32_t f = f1; f <= f2; f += st) {
    if (!c->tune(f)) { io->print(F("   ")); printHz(*io, f); io->print(F(" refuse : ")); io->println(c->plan().err); }
    else { printPlanLine(c->plan()); }
    if (!pauseOrAbort(500)) break;
  }
}

// ----------------------------------------------------------------------
//  8. Table de calcul (sans toucher au bus)
// ----------------------------------------------------------------------
static void testTable() {
  io->println(F("\r\n[8] TABLE DE CALCUL (aucune ecriture) :"));
  static const uint32_t fl[] = { 150000UL, 500000UL, 1000000UL, 1500000UL, 3500000UL, 5000000UL,
                                 7100000UL, 10100000UL, 14200000UL, 18100000UL, 21200000UL,
                                 28500000UL, 30000000UL };
  for (uint8_t i = 0; i < sizeof(fl) / sizeof(fl[0]); i++) {
    TunePlan t = planTune(fl[i], gSet.cal);
    io->print(F("   ")); printHz(*io, fl[i]); io->print(F(" : "));
    if (!t.ok) { io->println(t.err); continue; }
    io->print(F("F")); io->print(t.band + 1);
    io->print(F(" FA=")); io->print(t.faCode);
    io->print(F(" /")); io->print(t.ratioM);
    io->print(F(" VCO=")); printHz(*io, t.vcoHz);
    io->print(F(" DDS=")); printHz(*io, t.ddsHz);
    io->print(F(" FTW=0x")); io->println(t.ftw, HEX);
  }
}

// ----------------------------------------------------------------------
//  9. Aide au calibrage de la plage du VCO
// ----------------------------------------------------------------------
static void testVcoRange(RadwareCard* c) {
  io->println(F("\r\n[9] CALIBRAGE VCO : le DDS balaye 1.0 -> 3.5 MHz par pas de 50 kHz."));
  io->println(F("    Lisez J6 au compteur. Tant que la PLL est verrouillee, J6 = N x DDS."));
  io->println(F("    Tapez 'x' des que J6 ne suit plus : la derniere valeur affichee = limite."));
  io->println(F("    (balayage montant ; refaites-le a l'envers avec 'cal vco' pour l'autre limite)"));
  uint32_t last = 0;
  for (uint32_t f = 1000000UL; f <= 3500000UL; f += 50000UL) {
    c->ddsRawHz(f);
    io->print(F("   DDS ")); printHz(*io, f);
    io->print(F("  -> VCO attendu ")); printHz(*io, f * gSet.cal.pllN); io->println();
    if (!pauseOrAbort(1500)) { io->print(F("   Arret. Derniere valeur verrouillee probable : VCO = ")); printHz(*io, last * gSet.cal.pllN); io->println(); break; }
    last = f;
  }
  c->retune();
}

// ----------------------------------------------------------------------
//  Menu
// ----------------------------------------------------------------------
static void menu() {
  io->println(F("\r\n===== MODE TEST RADWARE ====="));
  io->println(F(" 1 Bus ISA : lignes une a une (sans /IOW)"));
  io->println(F(" 2 /IOW : impulsions vers un registre vide"));
  io->println(F(" 3 Preselecteurs F1..F8"));
  io->println(F(" 4 Diviseur SP8402 (FA1..FA3)"));
  io->println(F(" 5 DDS AD7008 : frequences fixes"));
  io->println(F(" 6 Lignes de controle : alim ant., gain, TX quad, FSEL, reset"));
  io->println(F(" 7 LO complet : balayage"));
  io->println(F(" 8 Table de calcul (sans ecriture bus)"));
  io->println(F(" 9 Calibrage de la plage du VCO"));
  io->println(F(" A Auto-test guide : enchaine 1,3,5,4,6,7"));
  io->println(F(" 0 Quitter"));
  io->print(F("> "));
}

void runTestMode(Link& l) {
  io = l.io;
  io->println(F("\r\nMode test : le CAT est suspendu. Chaque test se termine par 'x'."));
  for (;;) {
    menu();
    int k;
    do { k = readKeyTimeout(600000UL); } while (k == '\r' || k == '\n');
    if (k < 0) { io->println(F("\r\nInactif : sortie du mode test")); return; }
    k = tolower(k);
    io->println((char)k);
    if (k == '0' || k == 'q' || k == 'x') { io->println(F("Retour au mode normal.")); gRx[0]->retune(); gRx[1]->retune(); return; }
    if (k == '8') { testTable(); continue; }
    RadwareCard* c = askRx();
    if (!c) continue;
    switch (k) {
      case '1': testBus(c); break;
      case '2': testStrobe(c); break;
      case '3': testPresel(c); break;
      case '4': testDivider(c); break;
      case '5': testDds(c); break;
      case '6': testControls(c); break;
      case '7': testLo(c); break;
      case '9': testVcoRange(c); break;
      case 'a':
        testBus(c);       if (!waitEnter()) break;
        testPresel(c);    if (!waitEnter()) break;
        testDds(c);       if (!waitEnter()) break;
        testDivider(c);   if (!waitEnter()) break;
        testControls(c);  if (!waitEnter()) break;
        testLo(c);
        io->println(F("\r\nAuto-test termine."));
        break;
      default: io->println(F("Choix inconnu")); break;
    }
  }
}
