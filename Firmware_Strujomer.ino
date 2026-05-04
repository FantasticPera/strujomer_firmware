#include "Pins.h"
#include "ATM90E26_Control.h"
#include "Display_Control.h"
#include "src/GUI/LCD_Driver.h"
#include "src/GUI/DEV_Config.h" //nzm da l treba
//#include "GUI_Paint.h"
//#include "image.h"

SPIClass SPI_ATM(HSPI);
SPIClass SPI_LCD(FSPI);



void setup() {
  Serial.begin(115200);
  //SPI_ATM.begin(ATM_SCLK, ATM_SDO, ATM_SDI, ATM_CS); // valjda mora i ovo
  //ATM_SPI_Init(); poziva se u ATM_Init();

  //display settings:
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100); //max osvetljenost

  ATM_Init();
  Serial.println("V,I,F,P,E");
}

void loop() {
  float v = ATM_GetVoltage();
  float i = ATM_GetCurrent();
  float f = ATM_GetFrequency();
  float p = ATM_GetActivePower();
  float e = ATM_GetActiveEnergy();

  // CSV format: V,I,F,P,E
  Serial.print(v, 2); Serial.print(","); // ovo drugo je broj decimala...
  Serial.print(i, 3); Serial.print(",");
  Serial.print(f, 2); Serial.print(",");
  Serial.print(p, 2); Serial.print(",");
  Serial.print(e, 6);
  Serial.println(); // kraj linije

  Update_Display(v, i, f, p, e);

  delay(200); // npr. 5Hz


}