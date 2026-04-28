#ifndef ATM90E26_CONTROL_H
#define ATM90E26_CONTROL_H

#include <SPI.h>
#include "Pins.h"

// --- 1. STATUS AND SPECIAL REGISTERS ---
#define SoftReset 0x00
#define SysStatus 0x01
#define FuncEn    0x02

// --- 2. METERING CALIBRATION REGISTERS ---
#define CalStart  0x20
#define PLconstH  0x21
#define PLconstL  0x22
#define MMode     0x2B // Metering Mode Configuration
#define CS1       0x2C // Checksum 1

// --- 3. MEASUREMENT CALIBRATION REGISTERS ---
#define AdjStart  0x30
#define Ugain     0x31 // Voltage rms gain
#define IgainL    0x32 // L Line current rms gain
#define IgainN    0x33 // N Line current rms gain
#define CS2       0x3B // Checksum 2

// --- 4. MEASUREMENT REGISTERS (Ocitavanje trenutnih vrednosti) ---
#define Irms      0x48 // L Line Current rms
#define Urms      0x49 // Voltage rms
#define Pmean     0x4A // L Line Mean Active Power
#define Freq      0x4C // Voltage Frequency
#define PowerF    0x4D // L Line Power Factor (Faktor snage - korisno imati)

// --- 5. ENERGY REGISTERS (Ocitavanje utrosene energije - kWh)
#define APenergy  0x40 // Forward Active Energy
#define ATenergy  0x42 // Absolute Active Energy

// --- 6. Konstante za kalibraciju
#define POWER_MULTIPLIER 1 //TODO
#define ENERGY_MULTIPLIER 1 //0.0001?

// --- SPI
SPIClass SPI_ATM(HSPI);

// Deklaracije funkcija
void ATM_HardwareReset();
void ATM_WriteRegister(unsigned short address, unsigned short data);
unsigned short ATM_ReadRegister(unsigned short address);
void ATM_Init();
float ATM_GetVoltage();
float ATM_GetCurrent();

// --- DEFINICIJE FUNKCIJA ---

void ATM_HardwareReset() {
  pinMode(ATM_RESET, OUTPUT);
  digitalWrite(ATM_RESET, LOW);
  delay(50);
  digitalWrite(ATM_RESET, HIGH);
  delay(150); // Cekanje da se cip podigne
}

/*
-- Objasnjenje zasto SPI_MODE3

SCK (Serial Clock) Idle State: Na dijagramu se jasno vidi da linija za "Clock" stoji visoko na 3.3V (Logic 1) u trenucima pre nego što CS (Chip Select) padne na nulu. Ovo u terminologiji mikrokontrolera znači CPOL = 1 (Clock Polarity je 1).

Data Sampling (Uzorkovanje podataka): Dijagram pokazuje da se podaci sa SDI (Serial Data In) žice očitavaju (citiraju) na uzlaznoj ivici (rising edge) clock signala, a postavljaju se na silaznoj (falling edge). Ovo u SPI terminologiji znači CPHA = 1 (Clock Phase je 1).

Kada spojiš CPOL=1 i CPHA=1, dobijaš definiciju onoga što se u Arduino i ESP32 svetu zove SPI_MODE3.


*/

void ATM_WriteRegister(unsigned short address, unsigned short data) {
  // Za write, MSB bit mora biti 0 (zato se ne radi OR sa 0x8000 kao kod citanja)
  SPI_ATM.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(ATM_CS, LOW);
  
  SPI_ATM.transfer16(address); // Saljemo adresu (Write bit = 0)
  SPI_ATM.transfer16(data);    // Saljemo 16-bitni podatak
  
  digitalWrite(ATM_CS, HIGH);
  SPI_ATM.endTransaction();
  delay(1); // Kratka pauza za sigurnost
}

unsigned short ATM_ReadRegister(unsigned short address) {
  unsigned short data = 0;
  unsigned short command = (address | 0x8000); // Za read, MSB mora biti 1
  
  SPI_ATM.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(ATM_CS, LOW);
  
  SPI_ATM.transfer16(command);
  data = SPI_ATM.transfer16(0x0000); // Citamo
  
  digitalWrite(ATM_CS, HIGH);
  SPI_ATM.endTransaction();
  
  return data;
}

void ATM_Init() {
  Serial.println("Inicijalizacija ATM90E26...");
  
  // 1. Hardverski i softverski reset
  ATM_HardwareReset();
  ATM_WriteRegister(SoftReset, 0x789A); // Kljuc iz datasheet-a za reset
  delay(100);

  // Otkljucavanje registara za kalibraciju
  ATM_WriteRegister(AdjStart, 0x5678); // 5678 kljuc za kalibraciju
  
  //moramo izracunati IGAINL kad nam stigne plocica
  //Formula za racunanje prema datasheet Atmel-46102A-SE-M90E26-ApplicationNote.book

  //IgainL = int(31251 * poznataStruja / izmerenaStruja )

  //mozda puta 3? jer smo realan napon smanjili za 1/3 preko onih otpornika
  //TODO

  //ATM_WriteRegister(IgainL, 0x0ED1); 


  //Takodje ista stvar za napon
  // UgainL = int(26400 * poznataVoltaza / izmerenaVoltaza)
  //mozda *21?

  //ATM_WriteRegister(UgainL, ...)


  
  // Zakljucavanje
  ATM_WriteRegister(AdjStart, 0x8765);
  


  // Konfiguracija MMode registra (adresa 0x2B)
  // Upisujemo 0x9422: Lgain=1x, Ngain=1x, HPF=Ukljucen, L-line selektovana
  ATM_WriteRegister(MMode, 0x9422);
  
  // Provera da li je cip ziv
  unsigned short status = ATM_ReadRegister(SysStatus);
  if(status != 0xFFFF && status != 0x0000) {
    Serial.println("ATM90E26 Uspesno inicijalizovan!");
  } else {
    Serial.println("Greska: ATM90E26 ne odgovara.");
  }


}

//nisam siguran za ove dve sledece zasto se deli ovo... mozda ne treba

// Funkcije za dobijanje realnih vrednosti
float ATM_GetVoltage() {
  unsigned short rawVoltage = ATM_ReadRegister(Urms);
  // Ovu formulu cemo prilagoditi tvom hardveru kasnije.
  // Prema default vrednostima: rawVrms / 100.0
  return rawVoltage / 100.0; 
}

float ATM_GetCurrent() {
  unsigned short rawCurrent = ATM_ReadRegister(Irms);
  // Ovu formulu cemo prilagoditi tvom hardveru kasnije
  return rawCurrent / 1000.0;
}

float ATM_GetFrequency() {
  unsigned short rawFreq = ATM_ReadRegister(Freq); // Adresa 0x4C
  return rawFreq / 100.0; // Npr. 5000 postane 50.00 Hz
}

float ATM_GetActivePower() {
  
  short rawPower = (short)ATM_ReadRegister(Pmean); // Adresa 0x4A
  
  return rawPower * POWER_MULTIPLIER; 
}

float ATM_GetActiveEnergy() {
  // Citamo akumuliranu energiju
  unsigned short rawEnergy = ATM_ReadRegister(APenergy); // Adresa 0x40
  
  // Pretvaramo u kWh
  return rawEnergy * ENERGY_MULTIPLIER;
}


#endif