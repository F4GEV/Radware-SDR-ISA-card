// =====================================================================
//  radware.h - Pilote d'une carte Radware (cartographie déduite des
//  schémas SWOPC / SWOPCADD / DSP1DDS / SWOPCSYN)
//
//  Décodage d'adresse (SW1 + 74LS86 + 74LS30 + 74HC238) :
//     la carte répond si SA0, SA3..SA9 = réglage de SW1 (ON = 1) ; SA1/SA2
//     et /IOW sélectionnent le registre (écriture seule) :
//        base+0  (Y0) : strobe /WR de l'AD7008       (D0..D15 = bus)
//        base+2  (Y2) : STROBE du UCN5815 (U2)       (SD0..SD7 -> filtres)
//        base+4  (Y1) : horloge de U18 (SD0..SD7) et U17 (SD8..SD15)
//        base+6  (Y3) : non câblé -> écriture sans effet (test de /IOW)
// =====================================================================
#pragma once
#include <Arduino.h>
#include "config.h"
#include "isa_bus.h"

namespace rw {

// ---- offsets de registres ----
const uint16_t REG_DDS = 0x00;
const uint16_t REG_PRE = 0x02;
const uint16_t REG_CTL = 0x04;
const uint16_t REG_NUL = 0x06;

// ---- registre de contrôle (écrit à base+4) ----
// Octet bas = U18 (74HCT273) -> AD7008 + gains des mélangeurs
const uint16_t CTL_GAIN20 = 1u << 0;   // U18.Q1 -> ligne "GAIN 20"
const uint16_t CTL_GAIN0  = 1u << 1;   // U18.Q2 -> ligne "GAIN 0"
const uint16_t CTL_RESET  = 1u << 2;   // U18.Q3 -> AD7008 RESET (pin 38)
const uint16_t CTL_LOAD   = 1u << 3;   // U18.Q4 -> AD7008 LOAD  (pin 36)
const uint16_t CTL_TC3    = 1u << 4;   // U18.Q5 -> AD7008 TC3   (pin 35)
const uint16_t CTL_TC1    = 1u << 5;   // U18.Q6 -> AD7008 TC1   (pin 33)
const uint16_t CTL_TC0    = 1u << 6;   // U18.Q7 -> AD7008 TC0   (pin 32)
const uint16_t CTL_FSEL   = 1u << 7;   // U18.Q8 -> AD7008 FSEL  (pin 31)
// (TC2, SLEEP et /CS de l'AD7008 sont câblés à la masse sur la carte)

// Octet haut = U17 (74HC273). ATTENTION : câblage croisé sur D1..D8 (C15..C18, C11..C14)
const uint16_t CTL_FA3    = 1u << 8;   // SD8  -> U17.Q5 -> SP8402 S2 (poids fort)
const uint16_t CTL_TXQUAD = 1u << 11;  // SD11 -> U17.Q8 -> "TX QUAD CONTROL"
const uint16_t CTL_RXPWR  = 1u << 12;  // SD12 -> U17.Q1 -> "RX POWER" (+12 V sur J5)
const uint16_t CTL_TXPWR  = 1u << 13;  // SD13 -> U17.Q2 -> "TX POWER" (+12 V sur J4)
const uint16_t CTL_FA1    = 1u << 14;  // SD14 -> U17.Q3 -> SP8402 S1
const uint16_t CTL_FA2    = 1u << 15;  // SD15 -> U17.Q4 -> SP8402 S0 (poids faible)
const uint16_t CTL_FA_MASK = CTL_FA1 | CTL_FA2 | CTL_FA3;
const uint16_t CTL_TC_MASK = CTL_TC3 | CTL_TC1 | CTL_TC0;

// ---- filtres : F1..F8 <- OUT8..OUT1 du UCN5815 <- SD7..SD0 ----
const uint8_t  NUM_BANDS = 8;
inline uint8_t bandMask(uint8_t band) { return (uint8_t)(1u << (7 - band)); }  // band 0..7 = F1..F8

}  // namespace rw

// ---------------------------------------------------------------------
//  Paramètres de calibrage (EEPROM)
// ---------------------------------------------------------------------
struct RadwareCal {
  uint32_t frefHz;     // horloge AD7008 (B30)
  uint32_t vcoMinHz;   // plage de verrouillage du VCO
  uint32_t vcoMaxHz;
  uint32_t ddsMaxHz;   // limite haute de la fréquence DDS
  uint16_t baseA;      // adresse E/S carte 1
  uint16_t baseB;      // adresse E/S carte 2
  uint8_t  pllN;       // rapport du SP8792 (80 ou 81)
  uint8_t  dds16;      // 1 = bus 16 bits (CR0=1)
};
void calDefaults(RadwareCal& c);

// ---------------------------------------------------------------------
//  Calcul d'accord (pur, sans accès matériel) : fréquence LO -> réglages
//     f_LO = f_VCO / (4 * M)       M = 2^(k+1) (SP8402), k = faCode 0..7
//     f_VCO = N * f_DDS            N = 80 (SP8792)
//     FTW  = f_DDS * 2^32 / f_REF  (AD7008)
// ---------------------------------------------------------------------
struct TunePlan {
  bool     ok;
  const __FlashStringHelper* err;
  uint8_t  band;        // 0..7 = F1..F8
  uint8_t  faCode;      // 0..7
  uint16_t ratioM;      // 2..256
  uint32_t vcoHz;
  uint32_t ddsHz;       // fréquence DDS réellement obtenue
  uint32_t ftw;
  uint32_t actualLoHz;  // LO réellement obtenu (arrondi du FTW)
};
TunePlan planTune(uint32_t loHz, const RadwareCal& cal);
uint8_t  bandForFreq(uint32_t hz);           // 0..7
uint32_t bandLowHz(uint8_t band);
uint32_t bandHighHz(uint8_t band);
uint16_t faToCtl(uint8_t faCode);            // codes 0..7 -> bits CTL_FA1/2/3
void     printBandName(Print& p, uint8_t band);
void     printHz(Print& p, uint32_t hz);     // 7.100000 MHz

// ---------------------------------------------------------------------
//  Une carte
// ---------------------------------------------------------------------
class RadwareCard {
 public:
  RadwareCard(IsaBus& bus, char name, const RadwareCal& cal)
      : bus_(bus), cal_(cal), name_(name) {}

  void setBase(uint16_t b) { base_ = b; }
  uint16_t base() const { return base_; }
  char name() const { return name_; }

  void begin();                          // reset AD7008, commande, filtres coupés
  bool tune(uint32_t loHz);              // calcule + programme FA, DDS, filtre
  void retune() { if (loHz_) tune(loHz_); }

  // --- accès bas niveau (tests / diagnostic) ---
  void ctlWrite();                                   // écrit le registre de contrôle (ombre)
  void ddsReset();                                   // impulsion RESET + registre de commande
  void ddsLoadFtw(uint32_t ftw);                     // charge FREQ0
  void ddsRawHz(uint32_t hz);                        // DDS à une fréquence brute
  void setFA(uint8_t faCode);
  void setPreselMask(uint8_t mask);                  // bit0 = F8 ... bit7 = F1
  void setPreselBand(uint8_t band);                  // 0..7 = F1..F8
  void setPreselAuto(bool a) { preselAuto_ = a; }
  void setGain(uint8_t g);                           // bit0 = GAIN 20, bit1 = GAIN 0
  void setRxPower(bool on) { setBit(rw::CTL_RXPWR, on); }
  void setTxPower(bool on) { setBit(rw::CTL_TXPWR, on); }
  void setTxQuad(bool on)  { setBit(rw::CTL_TXQUAD, on); }
  void setFsel(bool on)    { setBit(rw::CTL_FSEL, on); }
  void setBit(uint16_t mask, bool on);               // modifie + écrit le registre de contrôle
  void forceCtl(uint16_t v) { ctl_ = v; ctlWrite(); }
  void dummyWrite(uint16_t data) { bus_.write(base_ + rw::REG_NUL, data); }

  // --- état ---
  uint32_t loHz() const { return loHz_; }
  const TunePlan& plan() const { return plan_; }
  uint8_t preselMask() const { return presel_; }
  bool preselAuto() const { return preselAuto_; }
  uint16_t ctl() const { return ctl_; }
  uint8_t gain() const { return (uint8_t)(ctl_ & (rw::CTL_GAIN20 | rw::CTL_GAIN0)); }
  bool rxPower() const { return ctl_ & rw::CTL_RXPWR; }
  bool txPower() const { return ctl_ & rw::CTL_TXPWR; }
  bool txQuad() const { return ctl_ & rw::CTL_TXQUAD; }
  void printStatus(Print& p) const;

 private:
  void ddsWriteWord(uint16_t w) { bus_.write(base_ + rw::REG_DDS, w); }
  void pulseLoad();

  IsaBus& bus_;
  const RadwareCal& cal_;
  char name_;
  uint16_t base_ = 0x300;
  uint16_t ctl_ = 0;
  uint8_t presel_ = 0;
  bool preselAuto_ = true;
  uint32_t loHz_ = 0;
  TunePlan plan_ = {};
};
