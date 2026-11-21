#include "app.h"
#include <ctype.h>

void linkInit(Link& l, Stream& io, uint8_t mapA, uint8_t mapB) {
  l.io = &io;
  l.n = 0;
  l.mapA = mapA;
  l.mapB = mapB;
  l.active = mapA;
  l.mode = '2';
  l.inTest = false;
}

// Aiguillage : ';' = commande CAT Kenwood, CR/LF = commande console.
void linkPoll(Link& l) {
  while (l.io->available()) {
    int c = l.io->read();
    if (c < 0) break;
    if (c == '\r' || c == '\n') {
      if (l.n) { l.buf[l.n] = 0; l.n = 0; consoleHandle(l, l.buf); }
      continue;
    }
    if (c == ';') {
      if (l.n) { l.buf[l.n] = 0; l.n = 0; catHandle(l, l.buf); }
      continue;
    }
    if (l.n < LINE_MAX - 1) l.buf[l.n++] = (char)c;
    else l.n = 0;                               // débordement : on repart de zéro
  }
}

// ---- utilitaires de saisie ----
bool parseUInt(const char* s, uint32_t& v, uint8_t base) {
  if (!s || !*s) return false;
  uint64_t acc = 0;
  for (; *s; s++) {
    int d;
    char c = *s;
    if (c >= '0' && c <= '9') d = c - '0';
    else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
    else return false;
    if (d >= base) return false;
    acc = acc * base + d;
    if (acc > 0xFFFFFFFFULL) return false;
  }
  v = (uint32_t)acc;
  return true;
}

// 7100000 (Hz) | 7100k | 7100.5k | 7.1m | 7,1m | 14.2 (point/virgule sans suffixe = MHz)
bool parseFreq(const char* s, uint32_t& hz) {
  if (!s || !*s) return false;
  uint64_t ip = 0, fp = 0;
  uint32_t fdiv = 1;
  bool dot = false, any = false;
  const char* p = s;
  for (; *p; p++) {
    char c = *p;
    if (c >= '0' && c <= '9') {
      any = true;
      if (!dot) { ip = ip * 10 + (c - '0'); if (ip > 4000000000ULL) return false; }
      else if (fdiv < 1000000UL) { fp = fp * 10 + (c - '0'); fdiv *= 10; }
    } else if ((c == '.' || c == ',') && !dot) {
      dot = true;
    } else break;
  }
  if (!any) return false;
  uint64_t scale = dot ? 1000000ULL : 1ULL;       // pas de suffixe : Hz, ou MHz si décimal
  if (*p) {
    char u = (char)tolower(*p);
    if (u == 'k') scale = 1000ULL;
    else if (u == 'm') scale = 1000000ULL;
    else if (u == 'h') scale = 1ULL;
    else return false;
    p++;
    if (u == 'h' && (*p == 'z' || *p == 'Z')) p++;
    if (*p) return false;
  }
  uint64_t v = ip * scale + (fp * scale) / fdiv;
  if (v > 0xFFFFFFFFULL) return false;
  hz = (uint32_t)v;
  return true;
}
