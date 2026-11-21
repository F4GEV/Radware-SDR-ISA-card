// =====================================================================
//  config.h - Réglages du pilote Arduino pour cartes ISA "Radware"
//  (Comer Communications - "Short Wave on a PC", SDR à mélangeurs en
//  quadrature, DDS AD7008 + PLL SP1648/SP8792 + 8 préselecteurs).
//
//  Carte supportée : Arduino MEGA 2560 (5 V), branchée directement sur
//  le bus ISA (27 broches : 16 données + 10 adresses + /IOW).
// =====================================================================
#pragma once
#include <Arduino.h>

#define FW_VERSION      "1.0"

// ---------------------------------------------------------------------
//  Broches Arduino Mega 2560 (câblage direct, voir docs/schema_cablage.svg)
//    SD0..SD7  = D22..D29     SD8..SD15 = D37..D30
//    SA0..SA7  = D49..D42     SA8, SA9  = D53, D52
//  Ces affectations sont figées par l'accès direct aux ports (isa_bus.cpp).
// ---------------------------------------------------------------------
#define PIN_ISA_IOW      8   // vers ISA B13 (/IOW) actif bas  (pull-up 10k côté bus)

#define IOW_PULSE_US    2           // largeur de l'impulsion /IOW en µs (>= 1)

// ---------------------------------------------------------------------
//  Liaison série (console + CAT Kenwood sur le même port USB)
// ---------------------------------------------------------------------
#define SERIAL_BAUD     38400UL
#define LINE_MAX        64

// 2e port CAT (Mega 2560 : Serial1 + adaptateur USB-série) : un 2e "rig"
// OmniRig qui pilote uniquement le récepteur B. Mettre à 1 pour l'activer.
#define ENABLE_CAT_PORT2  0
#define CAT2_BAUD         38400UL

// ---------------------------------------------------------------------
//  Valeurs par défaut (modifiables ensuite par la commande "cal" et
//  sauvegardées en EEPROM avec "save")
// ---------------------------------------------------------------------
#define DEF_BASE_A      0x300     // adresse E/S de la carte 1 (récepteur A)
#define DEF_BASE_B      0x310     // adresse E/S de la carte 2 (récepteur B)
#define DEF_FREF_HZ     10000000UL  // horloge de l'AD7008 (broche ISA B30 "OSC")
#define DEF_PLL_N       80          // SP8792 : ÷80 (CONT=1) ou ÷81 (CONT=0)
#define DEF_VCO_MIN_HZ  120000000UL // plage du VCO SP1648 (à calibrer, cf. doc)
#define DEF_VCO_MAX_HZ  240000000UL
#define DEF_DDS_MAX_HZ  3500000UL   // limite de fréquence de sortie du DDS
#define DEF_DDS_16BIT   1           // AD7008 en bus 16 bits (CR0=1) ; 0 = 8 bits

// Polarité des sorties du UCN5815 (U2) vers les filtres : 0 = niveau haut
// = filtre sélectionné (hypothèse par défaut), 1 = inversé.
#define PRESEL_INVERT   0
