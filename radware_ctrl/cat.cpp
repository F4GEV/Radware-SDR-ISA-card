// =====================================================================
//  cat.cpp - Émulation CAT Kenwood (jeu TS-2000 / TS-480 réduit)
//
//  FA = VFO A  -> récepteur A (carte 1)       FB = VFO B -> récepteur B (carte 2)
//  La fréquence transmise est la fréquence du LO = centre du spectre I/Q.
//  Compatible OmniRig (rig "TS-2000" ou "TS-480"), HDSDR, Ham Radio Deluxe...
// =====================================================================
#include "app.h"
#include <ctype.h>

static void reply(Link& l, const char* s) {
  l.io->print(s);
  l.io->print(';');
}

static void replyFreq(Link& l, char vfo, uint32_t hz) {
  char b[24];
  snprintf(b, sizeof(b), "F%c%011lu", vfo, (unsigned long)hz);
  reply(l, b);
}

static void replyErr(Link& l) { l.io->print(F("?;")); }

// n chiffres décimaux exactement -> valeur
static bool digits(const char* s, uint8_t maxLen, uint32_t& v) {
  size_t n = strlen(s);
  if (n == 0 || n > maxLen) return false;
  return parseUInt(s, v, 10);
}

void catHandle(Link& l, char* s) {
  while (*s == ' ') s++;
  for (char* q = s; *q; ++q) *q = (char)toupper(*q);
  if (strlen(s) < 2) { replyErr(l); return; }

  const char c0 = s[0], c1 = s[1];
  const char* arg = s + 2;
  char b[48];

  // ---- FA / FB : fréquence des VFO = LO des récepteurs ----
  if (c0 == 'F' && (c1 == 'A' || c1 == 'B')) {
    uint8_t rx = (c1 == 'A') ? l.mapA : l.mapB;
    if (*arg == 0) { replyFreq(l, c1, gRx[rx]->loHz()); return; }
    uint32_t hz;
    if (!digits(arg, 11, hz)) { replyErr(l); return; }
    if (!gRx[rx]->tune(hz)) { replyErr(l); return; }
    gSet.rx[rx].freqHz = hz;
    return;
  }

  // ---- FR / FT : sélection du récepteur actif (0 = VFO A, 1 = VFO B) ----
  if (c0 == 'F' && (c1 == 'R' || c1 == 'T')) {
    if (*arg == 0) {
      snprintf(b, sizeof(b), "F%c%d", c1, (l.active == l.mapB && l.mapA != l.mapB) ? 1 : 0);
      reply(l, b);
      return;
    }
    if (*arg == '0') { if (c1 == 'R') l.active = l.mapA; return; }
    if (*arg == '1') { if (c1 == 'R') l.active = l.mapB; return; }
    if (*arg == '2') return;                      // mémoire : ignoré
    replyErr(l); return;
  }

  // ---- ID : identité (019 = TS-2000) ----
  if (c0 == 'I' && c1 == 'D') { reply(l, "ID019"); return; }

  // ---- IF : état du récepteur actif (37 caractères) ----
  if (c0 == 'I' && c1 == 'F') {
    snprintf(b, sizeof(b), "IF%011lu0000+00000000000%c%c000000",
             (unsigned long)gRx[l.active]->loHz(), l.mode,
             (l.active == l.mapB && l.mapA != l.mapB) ? '1' : '0');
    reply(l, b);
    return;
  }

  // ---- MD : mode (mémorisé seulement : le SDR démodule côté PC) ----
  if (c0 == 'M' && c1 == 'D') {
    if (*arg == 0) { snprintf(b, sizeof(b), "MD%c", l.mode); reply(l, b); return; }
    if (*arg >= '1' && *arg <= '9' && arg[1] == 0) { l.mode = *arg; return; }
    replyErr(l); return;
  }

  // ---- PS : alimentation (toujours allumé) ----
  if (c0 == 'P' && c1 == 'S') { if (*arg == 0) reply(l, "PS1"); return; }

  // ---- AI : auto-information (non gérée -> toujours 0) ----
  if (c0 == 'A' && c1 == 'I') { if (*arg == 0) reply(l, "AI0"); return; }

  // ---- RX / TX / TQ / RS ----
  if (c0 == 'R' && c1 == 'X') { reply(l, "RX0"); return; }
  if (c0 == 'T' && c1 == 'X') { return; }                       // réception seule : ignoré
  if (c0 == 'T' && c1 == 'Q') { reply(l, "TQ0"); return; }
  if (c0 == 'R' && c1 == 'S') { reply(l, "RS0"); return; }

  // ---- SM : S-mètre (pas de mesure : 0) ----
  if (c0 == 'S' && c1 == 'M') { reply(l, "SM00000"); return; }

  // ---- commandes de réglage sans effet matériel : acceptées en silence ----
  if ((c0 == 'A' && c1 == 'G') || (c0 == 'R' && c1 == 'G') || (c0 == 'S' && c1 == 'Q') ||
      (c0 == 'N' && (c1 == 'B' || c1 == 'R')) || (c0 == 'P' && c1 == 'A') ||
      (c0 == 'R' && (c1 == 'T' || c1 == 'U' || c1 == 'D' || c1 == 'C')) ||
      (c0 == 'X' && c1 == 'T') || (c0 == 'S' && c1 == 'C') || (c0 == 'M' && c1 == 'F')) {
    if (*arg == 0) replyErr(l);
    return;
  }

  replyErr(l);
}
