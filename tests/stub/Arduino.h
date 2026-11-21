// Mini-stub Arduino pour compiler/tester le firmware sur PC (g++).
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <deque>

#define PROGMEM
#define PSTR(x) (x)
#define pgm_read_dword(p) (*(p))
#define pgm_read_byte(p) (*(p))
#define strcmp_P strcmp
class __FlashStringHelper;
#define F(x) (reinterpret_cast<const __FlashStringHelper*>(x))

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0
#define LED_BUILTIN 13
#define DEC 10
#define HEX 16
#define MSBFIRST 1

typedef bool boolean;
typedef uint8_t byte;

class Print {
 public:
  virtual ~Print() {}
  virtual size_t write(uint8_t c) = 0;
  size_t print(const __FlashStringHelper* f) { return print((const char*)f); }
  size_t print(const char* s) { size_t n = 0; while (*s) { write((uint8_t)*s++); n++; } return n; }
  size_t print(char c) { return write((uint8_t)c); }
  size_t print(int v, int b = 10) { return print((long)v, b); }
  size_t print(unsigned int v, int b = 10) { return print((unsigned long)v, b); }
  size_t print(long v, int b = 10) {
    if (v < 0 && b == 10) { write('-'); return 1 + print((unsigned long)(-v), b); }
    return print((unsigned long)v, b);
  }
  size_t print(unsigned long v, int b = 10) {
    char tmp[40]; int i = 0;
    if (v == 0) tmp[i++] = '0';
    while (v) { int d = v % b; tmp[i++] = d < 10 ? '0' + d : 'A' + d - 10; v /= b; }
    size_t n = 0; while (i) { write(tmp[--i]); n++; } return n;
  }
  size_t println() { return print("\r\n"); }
  size_t println(const __FlashStringHelper* f) { size_t n = print(f); return n + println(); }
  size_t println(const char* s) { size_t n = print(s); return n + println(); }
  size_t println(char c) { size_t n = print(c); return n + println(); }
  size_t println(int v, int b = 10) { size_t n = print(v, b); return n + println(); }
  size_t println(unsigned int v, int b = 10) { size_t n = print(v, b); return n + println(); }
  size_t println(long v, int b = 10) { size_t n = print(v, b); return n + println(); }
  size_t println(unsigned long v, int b = 10) { size_t n = print(v, b); return n + println(); }
};

class Stream : public Print {
 public:
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
};

class MockSerial : public Stream {
 public:
  std::string out;
  std::deque<char> in;
  void begin(unsigned long) {}
  size_t write(uint8_t c) override { out.push_back((char)c); return 1; }
  int available() override { return (int)in.size(); }
  int read() override { if (in.empty()) return -1; char c = in.front(); in.pop_front(); return (unsigned char)c; }
  int peek() override { return in.empty() ? -1 : (unsigned char)in.front(); }
  void feed(const char* s) { while (*s) in.push_back(*s++); }
  std::string take() { std::string s = out; out.clear(); return s; }
};
extern MockSerial Serial;

// ---- horloge virtuelle et broches (le test branche un modèle de carte) ----
extern unsigned long g_ms;
inline unsigned long millis() { return ++g_ms; }
inline void delay(unsigned long ms) { g_ms += ms; }
inline void delayMicroseconds(unsigned int) {}
typedef void (*PinHook)(uint8_t pin, uint8_t val);
extern PinHook g_pinHook;
inline void pinMode(uint8_t, uint8_t) {}
inline void digitalWrite(uint8_t pin, uint8_t val) { if (g_pinHook) g_pinHook(pin, val); }
