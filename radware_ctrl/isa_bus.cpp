#include "isa_bus.h"
#include "config.h"

#if defined(__AVR_ATmega2560__)
// ---------------------------------------------------------------------
//  Arduino Mega 2560 : écriture directe des ports
// ---------------------------------------------------------------------
void IsaBus::begin() {
  PORTH |= _BV(5);                           // /IOW au repos = haut (avant de passer en sortie)
  present(0x000, 0x0000);
  outputsEnable(true);
}

void IsaBus::outputsEnable(bool en) {
  if (en) {
    DDRA = 0xFF; DDRC = 0xFF; DDRL = 0xFF;
    DDRB |= 0x03;
    DDRH |= _BV(5);
  } else {                                   // tout en entrée sans pull-up (le bus a son pull-up sur /IOW)
    DDRA = 0; DDRC = 0; DDRL = 0;
    DDRB &= ~0x03;
    DDRH &= ~_BV(5);
    PORTA = 0; PORTC = 0; PORTL = 0; PORTB &= ~0x03; PORTH &= ~_BV(5);
  }
}

void IsaBus::present(uint16_t addr, uint16_t data) {
  addr &= 0x03FF;                            // SA0..SA9
  if (dry_) return;
  PORTA = (uint8_t)(data & 0xFF);            // SD0..SD7  = D22..D29
  PORTC = (uint8_t)(data >> 8);              // SD8..SD15 = D37..D30
  PORTL = (uint8_t)(addr & 0xFF);            // SA0..SA7  = D49..D42
  PORTB = (uint8_t)((PORTB & ~0x03) | (addr >> 8));   // SA8, SA9 = D53, D52
}

void IsaBus::strobeIOW() {
  if (dry_) return;
  delayMicroseconds(1);                      // établissement adresse/données
  PORTH &= ~_BV(5);                          // /IOW = bas
  delayMicroseconds(IOW_PULSE_US);
  PORTH |= _BV(5);                           // /IOW = haut
  delayMicroseconds(1);
}

#else
// ---------------------------------------------------------------------
//  Version portable (tests sur PC) : broche par broche
// ---------------------------------------------------------------------
static const uint8_t kPinSD[16] = {22,23,24,25,26,27,28,29, 37,36,35,34,33,32,31,30};
static const uint8_t kPinSA[10] = {49,48,47,46,45,44,43,42, 53,52};

void IsaBus::begin() {
  digitalWrite(PIN_ISA_IOW, HIGH);
  pinMode(PIN_ISA_IOW, OUTPUT);
  present(0x000, 0x0000);
  outputsEnable(true);
}
void IsaBus::outputsEnable(bool en) {
  for (uint8_t i = 0; i < 16; i++) pinMode(kPinSD[i], en ? OUTPUT : INPUT);
  for (uint8_t i = 0; i < 10; i++) pinMode(kPinSA[i], en ? OUTPUT : INPUT);
}
void IsaBus::present(uint16_t addr, uint16_t data) {
  addr &= 0x03FF;
  if (dry_) return;
  for (uint8_t i = 0; i < 16; i++) digitalWrite(kPinSD[i], (data >> i) & 1);
  for (uint8_t i = 0; i < 10; i++) digitalWrite(kPinSA[i], (addr >> i) & 1);
}
void IsaBus::strobeIOW() {
  if (dry_) return;
  digitalWrite(PIN_ISA_IOW, LOW);
  delayMicroseconds(IOW_PULSE_US);
  digitalWrite(PIN_ISA_IOW, HIGH);
}
#endif

void IsaBus::write(uint16_t addr, uint16_t data) {
  present(addr, data);
  strobeIOW();
  count_++;
  if (trace_ && out_) {
    out_->print(F("  IOW 0x"));
    out_->print(addr, HEX);
    out_->print(F(" <- 0x"));
    out_->println(data, HEX);
  }
}
