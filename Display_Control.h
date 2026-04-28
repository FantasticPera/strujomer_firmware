// #include "LCD_Driver.h"
#include "src/GUI/GUI_Paint.h"

void Update_Display(float V_Value, float I_Value, float F_Value, float P_Value, float E_Value);


// --- DEFINICIJE FUNKCIJA ---


void Update_Display(float V_Value, float I_Value, float F_Value, float P_Value, float E_Value) {

  // Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE); //nzm sta je ovo
  Paint_Clear(WHITE);

  Paint_DrawString_EN(10, 10, "Voltage:", &Font16, WHITE, BLACK);
  Paint_DrawNum(150, 10, V_Value, &Font16, WHITE, BLACK); // 2 decimale
  
  Paint_DrawString_EN(10, 30, "Current:", &Font16, WHITE, BLACK);
  Paint_DrawNum(150, 30, I_Value, &Font16, WHITE, BLACK); // 2 decimale

  Paint_DrawString_EN(10, 50, "Frequency:", &Font16, WHITE, BLACK);
  Paint_DrawNum(150, 50, F_Value, &Font16, WHITE, BLACK); // 2 decimale

  Paint_DrawString_EN(10, 70, "Power:", &Font16, WHITE, BLACK);
  Paint_DrawNum(150, 70, P_Value, &Font16, WHITE, BLACK); // 2 decimale
  
  Paint_DrawString_EN(10, 90, "Energy:", &Font16, WHITE, BLACK);
  Paint_DrawNum(150, 90, E_Value, &Font16, WHITE, BLACK); // 2 decimale

}
