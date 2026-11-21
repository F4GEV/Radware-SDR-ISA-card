#include "radware.h"

// ---------------------------------------------------------------------
//  Bandes des préselecteurs (bornes basses en Hz), F1 .. F8
// ---------------------------------------------------------------------
static const uint32_t kBandLo[8] PROGMEM = {
  18000000UL, 10800000UL, 6400000UL, 3880000UL,
   2330000UL,  1400000UL,  800000UL,  500000UL };
static const uint32_t kBandHi[8] PROGMEM = {
  30000000UL, 18000000UL, 10800000UL, 6400000UL,
   3880000UL,  2330000UL,  1400000UL,  800000UL };

uint32_t bandLowHz(uint8_t b)  { return pgm_read_dword(&kBandLo[b & 7]); }
uint32_t bandHighHz(uint8_t b) { return pgm_read_dword(&kBandHi[b & 7]); }

uint8_t bandForFreq(uint32_t hz) {
  for (uint8_t b = 0; b < 7; b++)
    if (hz >= bandLowHz(b)) return b;
  return 7;                               // < 800 kHz -> F8 (même hors bande)
}

void printBandName(Print& p, uint8_t b) {
  p.print('F'); p.print((int)(b + 1)); p.print(' ');
  uint32_t lo = bandLowHz(b), hi = bandHighHz(b);
  if (lo >= 1000000UL) { p.print(lo / 1000000UL); if (lo % 1000000UL) { p.print('.'); p.print((lo % 1000000UL) / 10000UL); } }
  else { p.print(lo / 1000UL); }
  p.print('-');
  if (hi >= 1000000UL) { p.print(hi / 1000000UL); if (hi % 1000000UL) { p.print('.'); p.print((hi % 1000000UL) / 10000UL); } p.print(F(" MHz")); }
  else { p.print(hi / 1000UL); p.print(F(" kHz")); }
}

void printHz(Print& p, uint32_t hz) {
  p.print(hz / 1000000UL);
  p.print('.');
  uint32_t r = hz % 1000000UL;
  if (r < 100000UL) p.print('0');
  if (r < 10000UL)  p.print('0');
  if (r < 1000UL)   p.print('0');
  if (r < 100UL)    p.print('0');
  if (r < 10UL)     p.print('0');
  p.print(r);
  p.print(F(" MHz"));
}

void calDefaults(RadwareCal& c) {
  c.frefHz   = DEF_FREF_HZ;
  c.vcoMinHz = DEF_VCO_MIN_HZ;
  c.vcoMaxHz = DEF_VCO_MAX_HZ;
  c.ddsMaxHz = DEF_DDS_MAX_HZ;
  c.baseA    = DEF_BASE_A;
  c.baseB    = DEF_BASE_B;
  c.pllN     = DEF_PLL_N;
  c.dds16    = DEF_DDS_16BIT;
}

uint16_t faToCtl(uint8_t k) {
  uint16_t v = 0;
  if (k & 1) v |= rw::CTL_FA2;            // S0 = FA2
  if (k & 2) v |= rw::CTL_FA1;            // S1 = FA1
  if (k & 4) v |= rw::CTL_FA3;            // S2 = FA3
  return v;
}

// ---------------------------------------------------------------------
//  Calcul d'accord
// ---------------------------------------------------------------------
TunePlan planTune(uint32_t lo, const RadwareCal& cal) {
  TunePlan p;
  memset(&p, 0, sizeof(p));
  p.band = bandForFreq(lo);
  if (lo == 0) { p.err = F("frequence nulle"); return p; }
  if (cal.frefHz == 0 || cal.pllN == 0) { p.err = F("calibrage invalide"); return p; }

  bool tooLow = false, found = false;
  uint8_t k;
  uint32_t M = 0;
  uint64_t vco = 0;
  for (k = 0; k < 8; k++) {
    M = 2UL << k;                                   // 2, 4, ... 256
    vco = (uint64_t)lo * 4ULL * M;
    if (vco > cal.vcoMaxHz) break;                  // M plus grand -> VCO encore plus haut
    if (vco < cal.vcoMinHz) { tooLow = true; continue; }
    found = true;
    break;
  }
  if (!found) {
    if (k == 0)          p.err = F("trop haute (> VCOmax/8)");
    else if (tooLow && k >= 8) p.err = F("trop basse (< VCOmin/1024)");
    else                 p.err = F("trou dans la plage VCO (voir cal vco)");
    return p;
  }

  const uint64_t den = (uint64_t)cal.pllN * cal.frefHz;          // N * Fref
  uint64_t ftw = ((vco << 32) + den / 2) / den;                  // arrondi
  if (ftw >= 0x80000000ULL) { p.err = F("DDS > Fref/2"); return p; }
  uint64_t dds = ((ftw * cal.frefHz) + 0x80000000ULL) >> 32;
  if (dds > cal.ddsMaxHz) { p.err = F("DDS > limite (cal ddsmax)"); return p; }

  p.ok = true;
  p.err = nullptr;
  p.faCode = k;
  p.ratioM = (uint16_t)M;
  p.vcoHz = (uint32_t)vco;
  p.ddsHz = (uint32_t)dds;
  p.ftw = (uint32_t)ftw;
  uint64_t num = ftw * cal.frefHz * cal.pllN;                    // < 2^64 (ftw < 2^31)
  uint64_t d2 = (4ULL * M) << 32;
  p.actualLoHz = (uint32_t)((num + d2 / 2) / d2);
  return p;
}

// ---------------------------------------------------------------------
//  RadwareCard
// ---------------------------------------------------------------------
void RadwareCard::ctlWrite() {
  bus_.write(base_ + rw::REG_CTL, ctl_);
}

void RadwareCard::setBit(uint16_t mask, bool on) {
  if (on) ctl_ |= mask; else ctl_ &= ~mask;
  ctlWrite();
}

void RadwareCard::pulseLoad() {
  ctl_ |= rw::CTL_LOAD;  ctlWrite();       // TC3..TC0 déjà stables avant LOAD
  ctl_ &= ~rw::CTL_LOAD; ctlWrite();
}

// Impulsion RESET puis écriture du registre de commande de l'AD7008.
void RadwareCard::ddsReset() {
  ctl_ &= ~(rw::CTL_TC_MASK | rw::CTL_LOAD | rw::CTL_FSEL);
  ctl_ |= rw::CTL_RESET;  ctlWrite();
  delay(1);
  ctl_ &= ~rw::CTL_RESET; ctlWrite();
  // TC3..TC0 = 00xx : destination = registre de commande (source parallèle)
  // CR0 = 1 : bus de données 16 bits ; CR2 = 0 : pas de modulation AM ;
  // CR3 = 0 : synchroniseur actif.
  ddsWriteWord(cal_.dds16 ? 0x0001 : 0x0000);
  pulseLoad();
}

// Charge FREQ0 : TC3..TC0 = 1000, 2 mots de 16 bits (ou 4 octets), puis LOAD.
void RadwareCard::ddsLoadFtw(uint32_t ftw) {
  ctl_ &= ~(rw::CTL_TC_MASK | rw::CTL_LOAD | rw::CTL_FSEL);
  ctl_ |= rw::CTL_TC3;                      // destination FREQ0, FSEL=0
  ctlWrite();
  if (cal_.dds16) {
    ddsWriteWord((uint16_t)(ftw >> 16));    // mot de poids fort d'abord
    ddsWriteWord((uint16_t)(ftw & 0xFFFF));
  } else {
    ddsWriteWord((uint16_t)((ftw >> 24) & 0xFF));   // octets, MSB d'abord (D8..D15 = 0)
    ddsWriteWord((uint16_t)((ftw >> 16) & 0xFF));
    ddsWriteWord((uint16_t)((ftw >> 8) & 0xFF));
    ddsWriteWord((uint16_t)(ftw & 0xFF));
  }
  pulseLoad();
}

void RadwareCard::ddsRawHz(uint32_t hz) {
  uint64_t ftw = (((uint64_t)hz << 32) + cal_.frefHz / 2) / cal_.frefHz;
  if (ftw >= 0x80000000ULL) ftw = 0x7FFFFFFFULL;
  ddsLoadFtw((uint32_t)ftw);
}

void RadwareCard::setFA(uint8_t k) {
  ctl_ = (ctl_ & ~rw::CTL_FA_MASK) | faToCtl(k & 7);
  ctlWrite();
}

void RadwareCard::setPreselMask(uint8_t mask) {
  presel_ = mask;
  uint8_t out = PRESEL_INVERT ? (uint8_t)~mask : mask;
  bus_.write(base_ + rw::REG_PRE, (uint16_t)out | (ctl_ & 0xFF00));
}

void RadwareCard::setPreselBand(uint8_t band) {
  setPreselMask(rw::bandMask(band & 7));
}

void RadwareCard::setGain(uint8_t g) {
  ctl_ &= ~(rw::CTL_GAIN20 | rw::CTL_GAIN0);
  if (g & 1) ctl_ |= rw::CTL_GAIN20;
  if (g & 2) ctl_ |= rw::CTL_GAIN0;
  ctlWrite();
}

void RadwareCard::begin() {
  ctl_ = 0;
  ctlWrite();                               // 1re écriture : état connu partout
  setPreselMask(0);                         // tous les filtres coupés
  ddsReset();
  loHz_ = 0;
}

bool RadwareCard::tune(uint32_t loHz) {
  plan_ = planTune(loHz, cal_);
  if (!plan_.ok) return false;
  // 1) filtre de bande (si automatique) : seulement s'il change
  if (preselAuto_) {
    uint8_t m = rw::bandMask(plan_.band);
    if (m != presel_) setPreselMask(m);
  }
  // 2) diviseur SP8402 + DDS (FA et TC dans la même écriture de contrôle)
  ctl_ = (ctl_ & ~rw::CTL_FA_MASK) | faToCtl(plan_.faCode);
  ddsLoadFtw(plan_.ftw);
  loHz_ = loHz;
  return true;
}

void RadwareCard::printStatus(Print& p) const {
  p.print(F("RX ")); p.print(name_);
  p.print(F("  base=0x")); p.print(base_, HEX);
  p.print(F("  LO="));
  if (loHz_) printHz(p, loHz_); else p.print(F("(non regle)"));
  if (loHz_ && plan_.ok) {
    p.print(F("\r\n   bande=")); printBandName(p, plan_.band);
  }
  p.print(F("\r\n   presel=0x")); p.print(presel_, HEX);
  p.print(preselAuto_ ? F(" (auto)") : F(" (manuel)"));
  if (loHz_ && plan_.ok) {
    p.print(F("\r\n   FA=")); p.print(plan_.faCode);
    p.print(F(" (/")); p.print(plan_.ratioM);
    p.print(F(")  VCO=")); printHz(p, plan_.vcoHz);
    p.print(F("  DDS=")); printHz(p, plan_.ddsHz);
    p.print(F("\r\n   FTW=0x")); p.print(plan_.ftw, HEX);
    p.print(F("  LO reel=")); printHz(p, plan_.actualLoHz);
  }
  p.print(F("\r\n   gain=")); p.print(gain());
  p.print(F("  RXpwr=")); p.print(rxPower() ? 1 : 0);
  p.print(F("  TXpwr=")); p.print(txPower() ? 1 : 0);
  p.print(F("  TXquad=")); p.print(txQuad() ? 1 : 0);
  p.print(F("  ctl=0x")); p.println(ctl_, HEX);
}
