#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define adresseI2CecranOLED 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

const int sensorPin = A0;
const int analogInPin = A0;
int sensorValue = 0;
int a=1;


void setup() {
pinMode(sensorPin, INPUT);

Serial.begin(115200);
display.begin(SSD1306_SWITCHCAPVCC,adresseI2CecranOLED);
display.setTextSize(a);
display.setTextColor(WHITE);

pinMode(analogInPin, INPUT);
Serial.begin(9600);


}

void loop() {
  sensorValue = analogRead(analogInPin);
  display.clearDisplay();
  display.setCursor(0,10);

  if (sensorValue < 511){
  display.setCursor(2,6);
  display.println(" >>> Valeurs");
  display.setCursor(2,22);
  display.println(" Sauvegardes ");

  display.drawRect(0, 0, 128, 15, WHITE);
  display.drawRect(0, 16, 128, 16, WHITE);

  display.display();

  delay(20);

}

if (sensorValue >= 512){
  display.setCursor(2,6);
  display.println(" Valeurs ");
  display.setCursor(2,22);
  display.println(" >>> Sauvegardes");

  display.drawRect(0, 0, 128, 15, WHITE);
  display.drawRect(0, 16, 128, 16, WHITE);

  display.display();

  delay(20);

  }
}
