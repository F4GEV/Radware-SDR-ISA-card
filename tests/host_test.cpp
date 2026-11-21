// Test sur PC : modèle logique de 2 cartes Radware branchées sur le bus simulé.
// Le modèle est écrit d'après les schémas (décodage SW1/XOR/NAND/74HC238, U17/U18,
// UCN5815) et la datasheet AD7008, indépendamment du code du firmware.
//   g++ -std=c++11 -I stub -I ../radware_ctrl host_test.cpp ../radware_ctrl/*.cpp -o host_test
#include "Arduino.h"
#include "EEPROM.h"
#include <vector>
#include <string>

MockSerial Serial;
unsigned long g_ms = 0;
PinHook g_pinHook = nullptr;
EEPROMClass EEPROM;

#include "config.h"
#include "../radware_ctrl/radware_ctrl.ino"      // setup() / loop() / globals

// ------------------------------------------------------------------
//  Modèle matériel d'une carte
// ------------------------------------------------------------------
struct SimCard {
  uint16_t dip;                 // adresse de base câblée par SW1 (SA0, SA3..SA9)
  uint16_t ctl = 0;             // U17 (haut) + U18 (bas)
  uint8_t  ucn = 0;             // UCN5815
  // AD7008
  uint32_t asmReg = 0, freq0 = 0, freq1 = 0; uint8_t cmd = 0; uint16_t phase = 0;
  int tcViolations = 0, loadCount = 0, resetCount = 0, wrCount = 0;
  bool loadPrev = false;

  bool match(uint16_t a) const { return ((a ^ dip) & 0x3F9) == 0; }

  void onIOW(uint16_t a, uint16_t d) {
    if (!match(a)) return;
    uint8_t code = (uint8_t)((((a >> 1) & 1) << 1) | ((a >> 2) & 1));   // {B=SA1, A=SA2}
    if (code == 0) {                       // Y0 -> /WR AD7008
      wrCount++;
      if (cmd & 1) asmReg = (asmReg << 16) | d;
      else         asmReg = (asmReg << 8) | (d & 0xFF);
    } else if (code == 2) {                // Y2 -> STROBE UCN5815
      ucn = (uint8_t)(d & 0xFF);
    } else if (code == 1) {                // Y1 -> CLK 74HC273 (front montant)
      uint16_t old = ctl;
      ctl = d;
      bool reset = ctl & (1 << 2);
      bool load  = ctl & (1 << 3);
      uint16_t tcMask = (1 << 4) | (1 << 5) | (1 << 6);
      if (reset) { cmd = 0; resetCount++; }
      if (load && !loadPrev) {             // front montant de LOAD
        if ((old & tcMask) != (ctl & tcMask)) tcViolations++;   // TC doit être stable avant LOAD
        loadCount++;
        bool tc3 = ctl & (1 << 4), tc1 = ctl & (1 << 5), tc0 = ctl & (1 << 6);
        // TC2 = 0 (masse)
        if (!tc3) cmd = asmReg & 0xF;
        else if (!tc1 && !tc0) freq0 = asmReg;
        else if (!tc1 && tc0) freq1 = asmReg;
        else if (tc1 && !tc0) phase = asmReg & 0xFFF;
      }
      if (load && loadPrev && ((old & tcMask) != (ctl & tcMask))) tcViolations++;  // maintien
      loadPrev = load;
    }
  }
  // lecture "électrique" des lignes du SP8402 d'après le câblage du schéma
  int ratio() const {
    int s0 = (ctl >> 15) & 1;  // FA2
    int s1 = (ctl >> 14) & 1;  // FA1
    int s2 = (ctl >> 8) & 1;   // FA3
    return 1 << (1 + 4 * s2 + 2 * s1 + s0);
  }
  // filtre actif (1..8) d'après OUTn <- INn <- SD(n-1) ; Fk <- OUT(9-k)
  int filter() const {
    for (int k = 1; k <= 8; k++) if (ucn & (1 << (8 - k))) return k;
    return 0;
  }
  double loHz(const RadwareCal& c) const {
    double fdds = (double)freq0 * c.frefHz / 4294967296.0;
    return fdds * c.pllN / (4.0 * ratio());
  }
};

static SimCard simA, simB;
static uint16_t busAddr = 0, busData = 0;

// Câblage Mega 2560 (table indépendante du firmware) : broche -> bit de bus
static void hook(uint8_t pin, uint8_t val) {
  static const uint8_t sd[16] = {22,23,24,25,26,27,28,29, 37,36,35,34,33,32,31,30};
  static const uint8_t sa[10] = {49,48,47,46,45,44,43,42, 53,52};
  for (int i = 0; i < 16; i++) if (pin == sd[i]) { busData = (busData & ~(1u << i)) | ((uint16_t)val << i); return; }
  for (int i = 0; i < 10; i++) if (pin == sa[i]) { busAddr = (busAddr & ~(1u << i)) | ((uint16_t)val << i); return; }
  if (pin == PIN_ISA_IOW && val == LOW) {
    simA.onIOW(busAddr, busData);
    simB.onIOW(busAddr, busData);
  }
}

// ------------------------------------------------------------------
static int fails = 0, checks = 0;
#define CHECK(c) do { checks++; if (!(c)) { fails++; printf("  ECHEC ligne %d : %s\n", __LINE__, #c); } } while (0)

static std::string run(const char* in) {
  Serial.feed(in);
  loop();
  return Serial.take();
}

static int expectedFilter(uint32_t f) {
  if (f >= 18000000) return 1; if (f >= 10800000) return 2; if (f >= 6400000) return 3;
  if (f >= 3880000) return 4;  if (f >= 2330000) return 5;  if (f >= 1400000) return 6;
  if (f >= 800000) return 7;   return 8;
}

static void checkTune(SimCard& s, RadwareCard& c, uint32_t f, const char* tag) {
  bool ok = c.tune(f);
  CHECK(ok);
  double lo = s.loHz(gSet.cal);
  double err = lo - (double)f;
  if (err < 0) err = -err;
  if (err > 0.5 || !ok) printf("  %s f=%lu lo_modele=%.4f err=%.4f ratio=%d ftw=%08lX\n", tag, (unsigned long)f, lo, err, s.ratio(), (unsigned long)s.freq0);
  CHECK(err < 0.5);                              // < 0,5 Hz
  CHECK(s.filter() == expectedFilter(f));
  CHECK(s.freq0 == c.plan().ftw);
  CHECK(s.ratio() == c.plan().ratioM);
}

int main() {
  g_pinHook = hook;
  simA.dip = DEF_BASE_A;
  simB.dip = DEF_BASE_B;
  setup();
  printf("Demarrage : %s", Serial.take().c_str());

  // --- 1. tune sur plusieurs fréquences, mode 16 bits ---
  printf("[1] accord 16 bits\n");
  static const uint32_t fl[] = { 117200, 150000, 500000, 799999, 800000, 1000000, 1400000, 2330000, 3500000,
                                 3880000, 5000000, 6400000, 7100000, 10100000, 10800000, 14200000,
                                 17999999, 18000000, 21200000, 28500000, 30000000 };
  for (uint32_t f : fl) { checkTune(simA, cardA, f, "A16"); checkTune(simB, cardB, f / 2 + 123457, "B16"); }
  CHECK(simA.tcViolations == 0 && simB.tcViolations == 0);

  // --- 2. mode 8 bits ---
  printf("[2] accord 8 bits\n");
  gSet.cal.dds16 = 0;
  cardA.ddsReset(); cardB.ddsReset();
  CHECK((simA.cmd & 1) == 0);
  for (uint32_t f : fl) checkTune(simA, cardA, f, "A8");
  gSet.cal.dds16 = 1;
  cardA.ddsReset();
  CHECK((simA.cmd & 1) == 1);
  CHECK(simA.tcViolations == 0);

  // --- 3. hors plage ---
  printf("[3] hors plage\n");
  CHECK(!cardA.tune(40000000));
  CHECK(!cardA.tune(100000));
  CHECK(!cardA.tune(0));

  // --- 4. isolation des cartes ---
  printf("[4] isolation A/B\n");
  cardA.tune(7100000); cardB.tune(14200000);
  uint32_t fa = simA.freq0, fb = simB.freq0;
  cardA.tune(3700000);
  CHECK(simB.freq0 == fb);
  CHECK(simA.freq0 != fa);

  // --- 5. contrôle : mapping des bits ---
  printf("[5] lignes de controle\n");
  cardA.setRxPower(true);  CHECK(simA.ctl & (1 << 12));
  cardA.setRxPower(false); CHECK(!(simA.ctl & (1 << 12)));
  cardA.setTxPower(true);  CHECK(simA.ctl & (1 << 13));
  cardA.setTxPower(false);
  cardA.setTxQuad(true);   CHECK(simA.ctl & (1 << 11));
  cardA.setTxQuad(false);
  cardA.setGain(1);        CHECK((simA.ctl & 3) == 1);
  cardA.setGain(2);        CHECK((simA.ctl & 3) == 2);
  cardA.setGain(0);
  for (int k = 0; k < 8; k++) { cardA.setFA(k); CHECK(simA.ratio() == (2 << k)); }
  for (int b = 0; b < 8; b++) { cardA.setPreselBand(b); CHECK(simA.filter() == b + 1); }
  cardA.setPreselMask(0); CHECK(simA.filter() == 0);

  // --- 6. CAT Kenwood ---
  printf("[6] CAT\n");
  Serial.take();
  std::string r;
  r = run("FA00007100000;");              CHECK(r == "");
  CHECK(cardA.loHz() == 7100000);
  r = run("FB00014200000;");              CHECK(cardB.loHz() == 14200000);
  r = run("FA;");                          CHECK(r == "FA00007100000;");
  r = run("FB;");                          CHECK(r == "FB00014200000;");
  r = run("ID;");                          CHECK(r == "ID019;");
  r = run("IF;");                          CHECK(r.size() == 38 && r.substr(0, 13) == "IF00007100000");
  if (r.size() != 38) printf("  IF = '%s' (%zu)\n", r.c_str(), r.size());
  r = run("FR1;");                         CHECK(r == "");
  r = run("IF;");                          CHECK(r.substr(0, 13) == "IF00014200000" && r[r.size() - 8] == '1');
  r = run("FR0;");
  r = run("MD2;MD;");                      CHECK(r == "MD2;");
  r = run("FA99999999999;");               CHECK(r == "?;");
  r = run("FA00040000000;");               CHECK(r == "?;");        // 40 MHz : hors plage
  r = run("XYZ;");                         CHECK(r == "?;");
  r = run("fa00003500000;");               CHECK(cardA.loHz() == 3500000);   // minuscules acceptées
  r = run("PS;");                          CHECK(r == "PS1;");
  r = run("AI;");                          CHECK(r == "AI0;");

  // --- 7. console ---
  printf("[7] console\n");
  r = run("a 7.1m\n");                     CHECK(cardA.loHz() == 7100000);
  r = run("b 14200k\r\n");                 CHECK(cardB.loHz() == 14200000);
  r = run("a 3.5\n");                      CHECK(cardA.loHz() == 3500000);
  r = run("a 7100000\n");                  CHECK(cardA.loHz() == 7100000);
  r = run("a 7100\n");                     CHECK(r.find("Refuse") != std::string::npos);   // 7,1 kHz
  r = run("plan 14.2m\n");                 CHECK(r.find("FTW") != std::string::npos);
  printf("  plan: %s", r.c_str());
  r = run("pre a 3\n");                    CHECK(simA.filter() == 3 && !cardA.preselAuto());
  r = run("pre a auto\n");                 CHECK(cardA.preselAuto() && simA.filter() == expectedFilter(7100000));
  r = run("ant a rx on\n");                CHECK(simA.ctl & (1 << 12));
  r = run("ant a rx off\n");               CHECK(!(simA.ctl & (1 << 12)));
  r = run("gain b 3\n");                   CHECK((simB.ctl & 3) == 3);
  r = run("fa a 5\n");                     CHECK(simA.ratio() == 64);
  r = run("dds a 2m\n");                   CHECK(simA.freq0 == (uint32_t)((2000000ULL << 32) / 10000000));
  r = run("st\n");                         CHECK(r.find("RX A") != std::string::npos && r.find("RX B") != std::string::npos);
  r = run("cal n 81\n");                   CHECK(gSet.cal.pllN == 81);
  r = run("cal n 80\n");
  r = run("cal ref 10000000\n");
  r = run("cal vco 100m 250m\n");          CHECK(gSet.cal.vcoMinHz == 100000000UL && gSet.cal.vcoMaxHz == 250000000UL);
  r = run("cal vco 120m 240m\n");
  r = run("aide\n");                       CHECK(r.find("MODE TEST") != std::string::npos);
  r = run("bidon\n");                      CHECK(r.find("inconnue") != std::string::npos);

  // --- 8. EEPROM ---
  printf("[8] EEPROM\n");
  gSet.restoreAtBoot = 1;
  r = run("a 7.1m\n"); r = run("b 14.2m\n");
  r = run("save\n");
  Settings t; CHECK(settingsLoad(t) && t.rx[0].freqHz == 7100000 && t.rx[1].freqHz == 14200000);
  cardA.begin(); cardB.begin();
  r = run("load\n");
  r = run("a\n");
  CHECK(cardA.loHz() == 7100000 && cardB.loHz() == 14200000);
  CHECK(simA.freq0 == cardA.plan().ftw);

  // --- 9. dry / trace ---
  printf("[9] dry/trace\n");
  uint32_t w0 = simA.wrCount;
  r = run("dry on\n");
  r = run("a 10.1m\n");
  CHECK(simA.wrCount == w0);
  r = run("dry off\n");
  r = run("trace on\n");
  r = run("a 10.1m\n");                    CHECK(r.find("IOW 0x300") != std::string::npos);
  r = run("trace off\n");

  // --- 10. mode test ---
  printf("[10] mode test\n");
  r = run("test\n8\n0\n");
  CHECK(r.find("TABLE DE CALCUL") != std::string::npos);
  CHECK(r.find("Retour au mode normal") != std::string::npos);
  r = run("test\n3ax0\n");                 CHECK(r.find("PRESELECTEURS") != std::string::npos);
  r = run("test\n4ax0\n");                 CHECK(r.find("DIVISEUR") != std::string::npos);
  r = run("test\n5ax0\n");                 CHECK(r.find("DDS AD7008") != std::string::npos);
  r = run("test\n1ax0\n");                 CHECK(r.find("BUS") != std::string::npos);
  r = run("test\n2ax0\n");                 CHECK(r.find("/IOW") != std::string::npos);
  r = run("test\n6an\nx\nx\nx\n0\n");
  r = run("test\n9ax0\n");                 CHECK(r.find("CALIBRAGE VCO") != std::string::npos);
  r = run("test\n7a7000k\n7100k\n50k\nx\n0\n");  CHECK(r.find("MODE TEST") != std::string::npos);
  printf("  (retour du mode test) cardA lo=%lu\n", (unsigned long)cardA.loHz());
  CHECK(simA.tcViolations == 0 && simB.tcViolations == 0);

  printf("\n%d verifications, %d echec(s)\n", checks, fails);
  return fails ? 1 : 0;
}
