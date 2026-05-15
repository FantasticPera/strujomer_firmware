// #include "LCD_Driver.h"
#include "src/GUI/GUI_Paint.h"

void Display_DrawStaticUI();
void Update_Display(float V_Value, float I_Value, float F_Value, float P_Value, float E_Value);


// --- DEFINICIJE FUNKCIJA ---
void Display_DrawStaticUI(){
    Paint_Clear(WHITE); // Oboji ceo ekran u belo
    
    // Crtanje ikonice na sredini gore (Ekran je 240px sirok, ikonica 70px -> (240-70)/2 = 85)
    //Paint_DrawImage(gImage_70X70, 85, 10, 70, 70); 

    // Crtanje statickih labela (tekst koji se ne menja)
    // Format: X, Y, tekst, font, boja_pozadine_slova, boja_slova
    Paint_DrawString_EN(10,  100, "Napon:", &Font16, WHITE, BLUE);
    Paint_DrawString_EN(130, 100, "Struja:", &Font16, WHITE, BLUE);
    
    Paint_DrawString_EN(10,  170, "Snaga:", &Font16, WHITE, BLUE);
    Paint_DrawString_EN(130, 170, "Frekv:", &Font16, WHITE, BLUE);

    // Linija separatora i labela za energiju na dnu
    Paint_DrawString_EN(40, 230, "--- Ukupno kWh ---", &Font16, WHITE, BLACK);
}


void Update_Display(float V, float I, float F, float P, float E) {
    char buffer[20];
    
    // --- LOGIKA ZA BOJE ---
    uint16_t color_V = GREEN;
    if (V < 200.0 || V > 250.0) color_V = RED; // Upozorenje: Prenizak ili previsok napon

    uint16_t color_I = BLACK;
    if (I > 16.0) color_I = RED; // Upozorenje: Prevelika struja (npr. preko 16 Ampera)

    uint16_t color_P = BLACK;
    if (P > 3500.0) color_P = RED; // Upozorenje: Prevelika snaga (preko 3.5 kW)

    // --- ISPISIVANJE VREDNOSTI ---
    // Koristimo "%6.1f" (fiksnu sirinu od 6 karaktera) da bi nova pozadina slova (WHITE) 
    // u potpunosti prebrisala stari broj, sprecavajuci "duhove" na ekranu.

    // Napon (X=10, Y=125)
    sprintf(buffer, "%5.1f V ", V); 
    Paint_DrawString_EN(10, 125, buffer, &Font20, WHITE, color_V);

    // Struja (X=130, Y=125)
    sprintf(buffer, "%5.2f A ", I);
    Paint_DrawString_EN(130, 125, buffer, &Font20, WHITE, color_I);

    // Snaga (X=10, Y=195)
    sprintf(buffer, "%6.1f W ", P);
    Paint_DrawString_EN(10, 195, buffer, &Font20, WHITE, color_P);

    // Frekvencija (X=130, Y=195)
    sprintf(buffer, "%5.1f Hz", F);
    Paint_DrawString_EN(130, 195, buffer, &Font20, WHITE, BLACK);

    // Energija (X=50, Y=255) - Centrirano sa najvecim fontom
    sprintf(buffer, "%8.3f ", E);
    Paint_DrawString_EN(50, 255, buffer, &Font24, WHITE, BLUE); 
}
