// =====================================================================
//  isa_bus.h - Maître de bus ISA minimal (écritures E/S 16 bits seules)
//
//  Arduino Mega 2560 branché DIRECTEMENT sur le bus ISA (aucun registre
//  à décalage). Accès par ports AVR entiers :
//     SD0..SD7   : PORTA  (D22..D29)
//     SD8..SD15  : PORTC  (D37..D30, SD8 = D37 ... SD15 = D30)
//     SA0..SA7   : PORTL  (D49..D42, SA0 = D49 ... SA7 = D42)
//     SA8, SA9   : PORTB bits 0,1 (D53, D52)
//     /IOW       : PORTH bit 5 (D8)
//  Les adresses/données ne changent que lorsque /IOW est à l'état haut,
//  donc aucune impulsion parasite n'est vue par le décodeur 74HC238.
// =====================================================================
#pragma once
#include <Arduino.h>

class IsaBus {
 public:
  void begin();
  void outputsEnable(bool en);                 // true : broches en sortie ; false : haute impédance
  void present(uint16_t addr, uint16_t data);  // place adresse+données SANS impulsion /IOW
  void strobeIOW();                            // impulsion /IOW
  void write(uint16_t addr, uint16_t data);    // present() + strobeIOW()

  void setTrace(bool on, Print* out) { trace_ = on; out_ = out; }
  void setDry(bool on) { dry_ = on; }
  bool trace() const { return trace_; }
  bool dry() const { return dry_; }
  uint32_t writeCount() const { return count_; }

 private:
  bool trace_ = false;
  bool dry_ = false;
  Print* out_ = nullptr;
  uint32_t count_ = 0;
};
