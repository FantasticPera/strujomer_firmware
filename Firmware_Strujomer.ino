#include "Pins.h"
#include "ATM90E26_Control.h"

void setup() {
  Serial.begin(115200);
  SPI.begin(); // valjda mora i ovo
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

  delay(200); // npr. 5Hz
}