// =====================================================================
//  app.h - Objets globaux et liaison série (console + CAT Kenwood)
// =====================================================================
#pragma once
#include <Arduino.h>
#include "config.h"
#include "radware.h"
#include "settings.h"
#include "isa_bus.h"

extern IsaBus        gBus;
extern Settings      gSet;
extern RadwareCard*  gRx[2];          // [0] = récepteur A, [1] = récepteur B

// Une liaison série. Les commandes se terminant par ';' sont du CAT Kenwood
// (FA, FB, IF, ID, MD...), celles se terminant par un retour à la ligne sont
// des commandes de console texte (voir "aide").
struct Link {
  Stream* io;
  char    buf[LINE_MAX];
  uint8_t n;
  uint8_t mapA;      // récepteur piloté par le VFO A du CAT  (0 = A, 1 = B)
  uint8_t mapB;      // récepteur piloté par le VFO B du CAT
  uint8_t active;    // récepteur "actif" (réponse à IF)
  char    mode;      // mode Kenwood mémorisé ('1'=LSB '2'=USB '3'=CW '4'=FM '5'=AM)
  bool    inTest;
};

void linkInit(Link& l, Stream& io, uint8_t mapA, uint8_t mapB);
void linkPoll(Link& l);
void catHandle(Link& l, char* cmd);        // commande CAT sans le ';'
void consoleHandle(Link& l, char* line);   // ligne de console sans le CR/LF
void printHelp(Print& p);
void printStatusAll(Print& p);
void applySettingsToCards();               // réapplique gSet -> cartes
void runTestMode(Link& l);                 // mode test interactif (selftest.cpp)

// utilitaires de saisie
bool parseFreq(const char* s, uint32_t& hz);      // 7100000 | 7100k | 7.1m | 7,1
bool parseUInt(const char* s, uint32_t& v, uint8_t base = 10);
