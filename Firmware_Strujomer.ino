#include "Pins.h"
#include "ATM90E26_Control.h"
#include "Display_Control.h"
#include "src/GUI/LCD_Driver.h"
#include "src/GUI/DEV_Config.h" //nzm da l treba
#include <Preferences.h>
//#include "GUI_Paint.h"
//#include "image.h"

SPIClass SPI_ATM(HSPI);
SPIClass SPI_LCD(FSPI);

//Za cuvanje podataka u slucaju nestanka struje (konkretno energije)
Preferences preferences;

// Promenljive za pracenje energije
float saved_energy = 0.0;
unsigned long last_save_time = 0;
const unsigned long SAVE_INTERVAL = 600000; // Cuvanje svakih 10 minuta (600.000 ms)


void setup() {
  Serial.begin(115200);
  
  // Otvaramo memorijski prostor pod imenom "strujomer" u Read/Write modu (false)
  preferences.begin("strujomer", false);
  
  // Ucitavamo prethodno sacuvanu energiju. Ako ne postoji, vraca 0.0
  saved_energy = preferences.getFloat("total_e", 0.0);
  Serial.print("Ucitana sacuvana energija: ");
  Serial.println(saved_energy, 4);

  //display settings:
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100); //max osvetljenost

  Display_DrawStaticUI();

  ATM_Init();
  Serial.println("V,I,F,P,E");
}

void loop() {
  float v = ATM_GetVoltage();
  float i = ATM_GetCurrent();
  float f = ATM_GetFrequency();
  float p = ATM_GetActivePower();

  // Energija od trenutka kada je cip upaljen
  float session_e = ATM_GetActiveEnergy(); 
  
  // Ukupna energija = ono sto je bilo u memoriji + ono sto je potroseno od paljenja
  float total_e = saved_energy + session_e;

  // CSV format: V,I,F,P,E
  Serial.print(v, 2); Serial.print(","); // ovo drugo je broj decimala...
  Serial.print(i, 3); Serial.print(",");
  Serial.print(f, 2); Serial.print(",");
  Serial.print(p, 2); Serial.print(",");
  Serial.print(total_e, 6);
  Serial.println(); // kraj linije

  Update_Display(v, i, f, p, total_e);

   // Zapisivanje u flash memoriju svakih 10 minuta
  if (millis() - last_save_time >= SAVE_INTERVAL) {
    preferences.putFloat("total_e", total_e);
    last_save_time = millis();
    Serial.println(" --- Stanje energije sacuvano u memoriju! --- ");
  }

  delay(200); // npr. 5Hz


}