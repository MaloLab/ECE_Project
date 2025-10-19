#include <SimpleTimer.h>
#include <Adafruit_SSD1306.h>
 
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 
int interval = 2000; // intervalle de temps en millisecondes
 
 
const int potentio = A6;
int bpm;
int buzzer=2;
 
int x=0, y=0; 
int x0=0,y0=0;
 
SimpleTimer timer;
 
 
void setup() 
{
  Serial.begin(9600);
 
  pinMode(buzzer,OUTPUT);
  pinMode(potentio,INPUT);
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Default OLED address, usually
  display.setTextSize(2); 
  display.setTextColor(WHITE);
 
  timer.setInterval(interval);
  
  Serial.begin(9600); // Initialiser la communication série pour l'affichage du résultat
}



void ecranOLED(){
  display.setCursor(20,0);
  display.drawLine(x0,y0,x,y,WHITE); // dessine ligne de x1,y1 à x2,y2 
 
  display.fillRect(40, 0, 120,20, BLACK);
  display.print("BPM: ");
  display.print(bpm);
 
  bpm = map(analogRead(potentio), 0, 1023, 0, 200);
  interval = 60000 / bpm;
  timer.setInterval(interval);
 
 
  y=40;
  y0=y;
  x0=x;
  x++;
// incrémente x à chaque passage
if (x>127) {
  x=0; // RAZ x de 127=>0
  x0=0; //réinitialise aussi x0 pour retour propre
  display.clearDisplay();
}
 
  display.display();
 
  if(timer.isReady())
  { 
    Serial.print("  /  ");
 
    Serial.print("interval: ");
    Serial.println(interval);
 
    tone(buzzer, 2000, 50);     //changer avec potentiomètre 
 
    y=30;
    timer.reset();
 }
}

 
void loop() 
{
  ecranOLED();
}
 
