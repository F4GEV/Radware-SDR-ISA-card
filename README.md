# Pilotage Arduino de la carte ISA « Radware » (SDR HF, AD7008)

Un **Arduino MEGA 2560**, branché **directement** sur le bus ISA (sans registre à décalage), joue le rôle de
**maître de bus** : il présente adresse et données sur 26 lignes et génère l'impulsion /IOW. Il pilote ainsi, sans PC à l'intérieur, une ou deux
cartes Radware (A = première adresse, B = seconde). Le PC ne sert qu'à recevoir l'audio I/Q (jacks J1/J2)
et à envoyer des commandes par USB (console série ou CAT Kenwood).

Schémas : `schema_synoptique.svg`, `schema_cablage.svg`, `schema_horloge_alim.svg`.

> **À lire d'abord – points non vérifiés.** Le schéma de la carte a été relu sur des images de
> basse résolution. Les points ci-dessous sont à contrôler sur votre exemplaire ; chacun a un réglage
> ou un test prévu dans le firmware (section 9).

## 1. Matériel

| Élément | Remarque |
|---|---|
| Arduino MEGA 2560 (5 V) | 27 broches utilisées ; 2e port CAT possible (`ENABLE_CAT_PORT2`) |
| Alimentation externe | +5 V, +12 V, −5 V sur le bus ISA (consommations estimées, à mesurer) |
| Référence 10 MHz | sortie CMOS 5 V sur B30 (voir `schema_horloge_alim.svg`) |
| −5 V | ICL7660/MAX1044 si absent de l'alimentation |

Masses communes Arduino / bus ISA obligatoires. **Ne pas relier le 5 V de l'Arduino au bus** si l'Arduino est alimenté par l'USB.
Mettre le bus sous tension **avant** de brancher/ouvrir le Mega : ses sorties ne doivent pas alimenter les cartes par leurs entrées.
Au démarrage ou avec `outputsEnable(false)`, les broches du Mega sont en haute impédance.

## 2. Câblage (direct Mega → ISA)

Un fil par signal. Accès par ports AVR entiers (fixé dans `isa_bus.cpp`) : /IOW = D8 (vers ISA B13, pull-up 10 kΩ côté bus).

| Signaux | Broches Mega | Broches ISA |
|---|---|---|
| SD0…SD7 (PORTA) | D22…D29 | A9 A8 A7 A6 A5 A4 A3 A2 |
| SD8…SD15 (PORTC) | D37, D36 … D30 | C11 C12 C13 C14 C15 C16 C17 C18 |
| SA0…SA7 (PORTL) | D49, D48 … D42 | A31 A30 A29 A28 A27 A26 A25 A24 |
| SA8, SA9 (PORTB) | D53, D52 | A23 A22 |
| /IOW | D8 | B13 |

Détail complet sur `schema_cablage.svg`. Résistances série 33 Ω optionnelles si fils > 15 cm.
Autres broches : AEN (A11) à la masse, B30 = horloge 10 MHz, +5 V = B3/B29/D16, +12 V = B9, −5 V = B5, GND = B1/B10/B31/D18.
Les cartes sont en **écriture seule** : /IOR, IRQ, DMA inutilisés.

## 3. Adresse de base (DIP SW1)

Le décodeur compare SA0 et SA3…SA9 ; SA1, SA2 et /IOW sont décodés par le 74HC238.
Interrupteur fermé = bit d'adresse à 1. Défauts du firmware : **A = 0x300, B = 0x310** (`cal base a|b <hex>`).
La correspondance numéro d'interrupteur ↔ bit n'est pas certaine : utilisez le menu test « bus » pour vérifier
la continuité/XOR et ajustez avec `cal base`.

## 4. Registres (offsets depuis la base)

| Offset | Fonction |
|---|---|
| +0 | AD7008 : strobe /WR, données D0–D15 |
| +2 | Verrou préselecteur UCN5815 (SD0–SD7) : F1 = bit 7 … F8 = bit 0 |
| +4 | Verrous de commande U18 (SD0–7) et U17 (SD8–15), mot 16 bits |
| +6 | Sans effet (utilisé par le test /IOW) |

Mot de commande (+4) : GAIN20 = b0, GAIN0 = b1, RESET = b2, LOAD = b3, TC3 = b4, TC1 = b5, TC0 = b6, FSEL = b7,
FA3 = b8, TX QUAD = b11, RX POWER = b12, TX POWER = b13, FA1 = b14, FA2 = b15.
Diviseur SP8402 : k = 4·S2 + 2·S1 + S0 (S0 = FA2, S1 = FA1, S2 = FA3), rapport M = 2^(k+1).

Bandes (limite basse) : F1 18 MHz, F2 10,8, F3 6,4, F4 3,88, F5 2,33, F6 1,4, F7 0,8, F8 0,5 MHz (< 0,8 MHz → F8).

## 5. Calcul de fréquence

f_LO = f_VCO / (4·M) ; f_VCO = N · f_DDS (N = 80 par défaut) ;
FTW = round(f_VCO · 2³² / (N · fref)), fref = 10 MHz. Le firmware choisit le plus petit M tel que
VCOmin ≤ 4·M·f_LO ≤ VCOmax (défaut 120–240 MHz) et f_DDS ≤ 3,5 MHz. Plage utile ≈ 117 kHz – 30 MHz.
`plan <f>` affiche M, N, FTW et la bande sans rien écrire.

## 6. Utilisation

Série 38400 bauds 8N1. Fin de ligne **« ; » = CAT**, **CR/LF = console**.

Console : `a 7.1m`, `b 14200k`, `st`, `plan <f>`, `pre <a|b> <1-8|auto|off>`, `gain <a|b> <0-3>`,
`ant <a|b> <rx|tx> <on|off>`, `txq`, `fa`, `dds`, `ftw`, `reset`, `cal …`, `trace`, `dry`, `restore`,
`save | load | defaults`, `test`, `aide`.
Formats : `7100000`, `7100k`, `7.1m`, `7.1` (MHz).

CAT Kenwood (sous-ensemble TS-2000) : `FA` → récepteur A, `FB` → récepteur B, `FR/FT`, `ID` (019), `IF`, `MD`,
`PS`, `AI`, `RX/TX/TQ/RS`, `SM`.

OmniRig / HDSDR : choisir un rig Kenwood TS-2000 (ou TS-480), 38400 bauds, 8N1, port COM de l'Arduino.
Sur le Mega l'ouverture du port **réinitialise** la carte (DTR) : attendez ~2 s, ou coupez la ligne DTR,
ou activez `restore on` + `save` pour que les fréquences soient réappliquées au démarrage.
Un second logiciel pour B : utiliser `FB` (ou le 2e port série du Mega avec `ENABLE_CAT_PORT2`).

## 7. Mode test (`test`)

Menu interactif, chaque fonction est indépendante : 1 bus (une ligne à 1 à la fois, avec noms des broches ISA),
2 /IOW (écritures factices à base+6), 3 préselecteurs F1…F8, 4 diviseur SP8402, 5 DDS à fréquences fixes,
6 lignes de commande (avertissement avant RX/TX POWER), 7 balayage du LO, 8 table de calcul (sans écriture),
9 aide au calibrage du VCO, A auto-test guidé. `dry on` permet de tout essayer sans toucher au bus ; `trace on` affiche chaque écriture.

## 8. Calibrage

1. Mesurer le LO sur J6 avec un fréquencemètre, `a 10m` par exemple.
2. Écart de fréquence : `cal ref <Hz>` (valeur réelle de l'oscillateur de référence).
3. VCO hors plage ou LO absent : `cal vco <min> <max>` (menu test 9), `cal n 81` si N = 81.
4. Pas de signal DDS ou mots corrompus : `cal dds16 0` (mode 8 bits, 4 écritures d'octets).
5. `save` pour mémoriser.

## 9. Incertitudes à vérifier sur votre carte

- Broche CONT du SP8792 flottante : division 80 ou 81 (`cal n`).
- Plage réelle du VCO : le SP8792 est donné pour 225 MHz max ; la plage 120–240 MHz est une hypothèse (`cal vco`).
- /CLR de U17/U18 dessinés à la masse sur le schéma relu.
- Polarité de l'UCN5815 (`PRESEL_INVERT` dans `config.h`).
- Signification exacte des bits GAIN20 / GAIN0 et polarité de TX QUAD.
- Correspondance interrupteurs SW1 ↔ bits d'adresse.
- AD7008 en mode 16 bits ou 8 bits (`cal dds16`).
- Filtre passe-bas du DDS (~2,1 MHz) alors que le DDS peut monter à ~3 MHz.
- Consommations d'alimentation (+5 V ≥ 1 A, +12 V ~300 mA, −5 V ~10 mA) : estimations.
- Conditionnement de l'horloge 10 MHz : un sinus faible ne suffit pas, voir `schema_horloge_alim.svg`.

## 10. Compilation et tests

Dossier `radware_ctrl/` : ouvrir `radware_ctrl.ino` dans l'IDE Arduino (carte « Arduino Mega or Mega 2560 »), aucune bibliothèque externe.
Réglages matériels dans `config.h`.
Tests sur PC (modèle logique de deux cartes, indépendant du firmware) :

```
cd tests
g++ -std=c++11 -I stub -I ../radware_ctrl host_test.cpp ../radware_ctrl/*.cpp -o host_test && ./host_test
```

Ces tests valident la logique et la séquence des écritures, **pas** le comportement du matériel réel.
