// Broches de contrôle AD7008
const int pinWR   = 37; 
const int pinLOAD = 41; 

// Broches d'adresse de transfert TC0 à TC3
const int pinTC0  = 32;
const int pinTC1  = 33;
const int pinTC2  = 34;
const int pinTC3  = 35;

// Broche spécifique pour le Latch du pré-sélecteur (U2)
const int pinPRE_SEL_LATCH = 40; 

// Variables CAT OmniRig
String inputBuffer = "";
uint32_t currentFrequency = 7100000UL; // Fréquence par défaut au démarrage (7.1 MHz)

void setup() {
  // Configuration du PORTA (Pins 22 à 29) en sortie pour le bus de données
  DDRA = 0xFF; 
  PORTA = 0x00;

  // Configuration de toutes les broches de contrôle
  pinMode(pinWR, OUTPUT);           digitalWrite(pinWR, HIGH);
  pinMode(pinLOAD, OUTPUT);         digitalWrite(pinLOAD, LOW);
  pinMode(pinPRE_SEL_LATCH, OUTPUT);digitalWrite(pinPRE_SEL_LATCH, HIGH);
  
  pinMode(pinTC0, OUTPUT); pinMode(pinTC1, OUTPUT);
  pinMode(pinTC2, OUTPUT); pinMode(pinTC3, OUTPUT);
  setTCAddress(0x00);

  // Communication série avec OmniRig (Vitesse : 57600)
  Serial.begin(57600); 
  inputBuffer.reserve(32);

  // Initialisation matérielle
  initCommandRegister();
  updateSDRSystem(currentFrequency);
}

void loop() {
  // Traitement des données entrantes d'OmniRig
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    inputBuffer += inChar;
    
    if (inChar == ';') {
      parseCATCommand(inputBuffer);
      inputBuffer = ""; 
    }
  }
}

// Analyse des commandes Kenwood émanant d'OmniRig
void parseCATCommand(String cmd) {
  cmd.trim();
  if (cmd.length() < 3) return;

  // Commande FA : Lecture ou Écriture de la fréquence principale
  if (cmd.substring(0, 2) == "FA") {
    if (cmd.length() == 3) {
      // OmniRig demande la fréquence actuelle ("FA;")
      sendCATResponse(currentFrequency);
    } else {
      // OmniRig impose une nouvelle fréquence (Ex: "FA00007100000;")
      String freqStr = cmd.substring(2, cmd.length() - 1);
      uint32_t newFreq = freqStr.toInt();
      
      if (newFreq > 0 && newFreq != currentFrequency) {
        currentFrequency = newFreq;
        updateSDRSystem(currentFrequency); // Met à jour le DDS ET le filtre de bande
      }
    }
  }
  // Réponse obligatoire pour qu'OmniRig valide la connexion avec la "radio"
  else if (cmd.substring(0, 2) == "ID") {
    Serial.print("ID019;"); // Émulation Kenwood TS-480
  }
}

void sendCATResponse(uint32_t freq) {
  char response[16];
  sprintf(response, "FA%011lu;", freq);
  Serial.print(response);
}

// Fonction globale de mise à jour de la carte
void updateSDRSystem(uint32_t freq) {
  setSDRFrequency(freq);       // 1. Met à jour l'AD7008
  updatePreselectorFilter(freq); // 2. Aligne le filtre matériel de la carte
}

// --- Gestion Matérielle du Pré-sélecteur (U2) ---
void updatePreselectorFilter(uint32_t freq) {
  uint8_t filterMask = 0x00;

  // Détermination du filtre selon les plages de votre schéma original
  if (freq >= 18000000UL && freq <= 30000000UL)      filterMask = 0x01; // F1 : 18-30 MHz
  else if (freq >= 10800000UL && freq < 18000000UL)  filterMask = 0x02; // F2 : 10.8-18 MHz
  else if (freq >= 6400000UL  && freq < 10800000UL)  filterMask = 0x04; // F3 : 6.4-10.8 MHz
  else if (freq >= 3880000UL  && freq < 6400000UL)   filterMask = 0x08; // F4 : 3.88-6.4 MHz
  else if (freq >= 2330000UL  && freq < 3880000UL)   filterMask = 0x10; // F5 : 2.33-3.88 MHz
  else if (freq >= 1400000UL  && freq < 2330000UL)   filterMask = 0x20; // F6 : 1.4-2.33 MHz
  else if (freq >= 800000UL   && freq < 1400000UL)   filterMask = 0x40; // F7 : 800-1400 KHz
  else if (freq >= 500000UL   && freq < 800000UL)    filterMask = 0x80; // F8 : 500-800 KHz

  if (filterMask != 0x00) {
    PORTA = filterMask; // Envoi du masque sur le bus de données commun
    delayMicroseconds(1);
    digitalWrite(pinPRE_SEL_LATCH, LOW);  // Impulsion descendante pour le verrou U2
    delayMicroseconds(2);
    digitalWrite(pinPRE_SEL_LATCH, HIGH); 
    delayMicroseconds(1);
  }
}

// --- Fonctions Basses-Couches AD7008 ---

void setTCAddress(uint8_t address) {
  digitalWrite(pinTC0, (address & 0x01) ? HIGH : LOW);
  digitalWrite(pinTC1, (address & 0x02) ? HIGH : LOW);
  digitalWrite(pinTC2, (address & 0x04) ? HIGH : LOW);
  digitalWrite(pinTC3, (address & 0x08) ? HIGH : LOW);
}

void writeByteToAssemblyRegister(uint8_t data) {
  PORTA = data;
  delayMicroseconds(1);       
  digitalWrite(pinWR, LOW);   
  delayMicroseconds(1);       
  digitalWrite(pinWR, HIGH);  
  delayMicroseconds(1);       
}

void initCommandRegister() {
  setTCAddress(0x00); // 00XX -> Command Register
  writeByteToAssemblyRegister(0x00); // Mode 8-bits
  digitalWrite(pinLOAD, HIGH); delayMicroseconds(2); digitalWrite(pinLOAD, LOW);
}

void setSDRFrequency(uint32_t targetFrequency) {
  // Calcul précis basé sur votre oscillateur externe de 10 MHz
  double calc = ((double)targetFrequency * 4294967296.0) / 10000000.0;
  uint32_t tuningWord = (uint32_t)calc;

  uint8_t byte3 = (tuningWord >> 24) & 0xFF;
  uint8_t byte2 = (tuningWord >> 16) & 0xFF;
  uint8_t byte1 = (tuningWord >> 8)  & 0xFF;
  uint8_t byte0 =  tuningWord        & 0xFF;

  writeByteToAssemblyRegister(byte3);
  writeByteToAssemblyRegister(byte2);
  writeByteToAssemblyRegister(byte1);
  writeByteToAssemblyRegister(byte0);

  setTCAddress(0x08); // 1000 -> FREQ0 REG
  delayMicroseconds(1);

  digitalWrite(pinLOAD, HIGH); delayMicroseconds(2); digitalWrite(pinLOAD, LOW);
}
