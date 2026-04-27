//NEBITAN FAJL NESTO SAM ISPROBAVAO ...

#include <Adafruit_GFX.h>    // Core graficka biblioteka
#include <Adafruit_ST7789.h> // Hardware-specific biblioteka za ST7789
#include <SPI.h>

// Definisanje pinova za ekran prema tvojoj semi (ESP32-S3 GPIO)
#define D_BL   4  // Backlight (Mora biti HIGH da bi ekran svetleo)
#define D_CS   5  // Chip select
#define D_SDI  6  // Podaci ka ekranu (MOSI)
#define D_SCLK 7  // Clock
#define D_RST  8  // Reset
#define D_A0   9  // Data/Command (DC pin)

// Inicijalizacija ekrana sa specifičnim pinovima
Adafruit_ST7789 display = Adafruit_ST7789(D_CS, D_A0, D_SDI, D_SCLK, D_RST);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Testiranje SEEED 1.69 IPS ekrana...");

  // 1. Ukljucivanje pozadinskog osvetljenja (Backlight)
  pinMode(D_BL, OUTPUT);
  digitalWrite(D_BL, HIGH); // Upali svetlo na ekranu

  // 2. Inicijalizacija ekrana (SEEED 1.69" ima rezoluciju 240x280)
  display.init(240, 280); 
  
  // 3. Rotacija ekrana (0, 1, 2 ili 3 zavisi kako zelis da stoji na kucistu)
  display.setRotation(0); 
  
  // 4. Oboji ceo ekran u plavo da vidimo da radi
  display.fillScreen(ST77XX_BLUE);

  // 5. Ispisivanje teksta
  display.setCursor(20, 100);             // Pozicija teksta (X, Y)
  display.setTextColor(ST77XX_WHITE);     // Bela boja slova
  display.setTextSize(3);                 // Velicina fonta
  display.println("Sistem");
  
  display.setCursor(20, 140);
  display.setTextColor(ST77XX_GREEN);     // Zelena boja
  display.println("Pokrenut!");
  
  Serial.println("Slika bi trebala biti na ekranu!");
}

void loop() {
  // Animacija - menjamo boju teksta svake sekunde
  display.setCursor(20, 180);
  display.setTextColor(ST77XX_RED);
  display.setTextSize(2);
  display.println("Test uspesan!");
  delay(1000);
  
  // Prebojimo ga pozadinskom bojom (plavom) da "nestane"
  display.setCursor(20, 180);
  display.setTextColor(ST77XX_BLUE); 
  display.println("Test uspesan!");
  delay(500);
}