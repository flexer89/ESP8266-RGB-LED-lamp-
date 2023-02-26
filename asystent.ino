#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// Define the pin to which the IR receiver output is connected
const uint16_t RECEIVER_PIN = 0;

// Create an IR receiver object
IRrecv irrecv(RECEIVER_PIN);

const int RED_PIN = 13;
const int GREEN_PIN = 12;
const int BLUE_PIN = 14;

// Fade Colors
const uint32_t colors[] = {
  0xFF0000, // Red
  0xFFFF00, // Yellow
  0x00FF00, // Green
  0x00FFFF, // Cyan
  0x0000FF, // Blue
  0xFF00FF, // Magenta
  0xFF0000 // Red
};

void changeColor() 
{
  for (int i = 0; i < sizeof(colors) / sizeof(uint32_t) - 1; i++) 
  {
    uint32_t color1 = colors[i];
    uint32_t color2 = colors[i + 1];
    for (float j = 0; j <= 1; j += 0.01) 
    {
      uint8_t r1 = (color1 >> 16) & 0xFF;
      uint8_t g1 = (color1 >> 8) & 0xFF;
      uint8_t b1 = color1 & 0xFF;
      uint8_t r2 = (color2 >> 16) & 0xFF;
      uint8_t g2 = (color2 >> 8) & 0xFF;
      uint8_t b2 = color2 & 0xFF;
      uint8_t r = (r1 * (1 - j)) + (r2 * j);
      uint8_t g = (g1 * (1 - j)) + (g2 * j);
      uint8_t b = (b1 * (1 - j)) + (b2 * j);
      analogWrite(RED_PIN, r);
      analogWrite(GREEN_PIN, g);
      analogWrite(BLUE_PIN, b);
      delay(10);
    }
  }
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  decode_results diode;

  if (irrecv.decode(&diode)) 
  {
    Serial.print("Received IR code: 0x");
    Serial.println(diode.value, HEX);
    
    delay(200);

    if (diode.value == 0xFF6897) // 0 button OFF
    {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }

    if (diode.value == 0xFF30CF) // 1 button
    {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
    if (diode.value == 0xFF18E7) // 2 button
    {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
    }
    if (diode.value == 0xFF7A85) // 3 button
    {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
    }
    if (diode.value == 0xFF10EF) // 4 button
    {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        changeColor();
    }
    if (diode.value == 0xFF38C7) // 5 button
    {
      analogWrite(RED_PIN, 255);
      analogWrite(GREEN_PIN, 255);
      analogWrite(BLUE_PIN, 255);
      delay(500);
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      delay(500);
    }
    if (diode.value == 0xFF5AA5) // 6 button
    {
        for (int i = 0; i < 50; i++) 
        {
          analogWrite(RED_PIN, random(0, 255));
          analogWrite(GREEN_PIN, random(0, 255));
          analogWrite(BLUE_PIN, random(0, 255));
          delay(100);
        }
    }
    if (diode.value == 0xFF42BD) // 7 button
    {
         for (int i = 0; i < 10; i++) 
         {
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(BLUE_PIN, LOW);
            delay(100);
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, HIGH);
            digitalWrite(BLUE_PIN, LOW);
            delay(100);
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(BLUE_PIN, HIGH);
            delay(100);
        }
    }
    irrecv.resume(); // Receive the next value
  }

}
