#include "FastLED.h"

// Numar LEDuri
#define NUM_LEDS 30
// PIN DIGITAL
#define DATA_PIN 4
// Viteza Transfer Date
#define serialRate 38400

// Prefix codificare
uint8_t prefix[] = {'r', 'i', 'n', 'g'}, bRingAlarm = 0;
// Declarare Vector de LEDuri
CRGB leds[NUM_LEDS];

void setup() {
  //adaugare LEDuri in structura FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  // Test LEDuri
  LEDS.showColor(CRGB(10, 0, 0));
  delay(200);
  LEDS.showColor(CRGB(0, 10, 0));
  delay(200);
  LEDS.showColor(CRGB(0, 0, 10));
  delay(200);
  LEDS.showColor(CRGB(0, 0, 0));
  
  // Comunicatii USB
  Serial.begin(serialRate);
}

void loop() { 
  if(bRingAlarm != '1')
  {
    // Verificare biti de comunicare
    for(int i = 0; i < sizeof prefix; ++i) {
      waitLoop: while (!Serial.available());
      if(prefix[i] == Serial.read()) continue;
      i = 0;
      goto waitLoop;
    }
    
    while (!Serial.available()) ;
    bRingAlarm=Serial.read();
    
    // Alocare memorie
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
    // Citeste datele transmise si seteaza culori LEDuri
  }
  else if(bRingAlarm == '1')
  {
    byte brightness = 0;
    while(bRingAlarm == '1')
    {
      if(brightness>=200)
      {
        bRingAlarm = 0;
        LEDS.showColor(CRGB(0, 0, 0));
        break;
      }
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(brightness,brightness,brightness);
      }
      brightness += 1;
      delay(300);
      FastLED.show();
    }
  }
}
