#include <SimpleTimer.h>
#include <Adafruit_SSD1306.h>


 // ECRAN OLED ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
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

 // FREQUENCE BUZZER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 const int interruptPin = 3;
 const int buzzerPin = 2;

 volatile int frequency = 0;
 volatile unsigned long previousMillis = 0;
 volatile unsigned long intervalfreq = 100;

 // ULTRASON ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int TriggerPin = 8;
const int EchoPin = 9;
long Duration = 0;

 
void setup() 
{
  
  Serial.begin(9600);
  
  // ECRAN OLED ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(buzzer,OUTPUT);
  pinMode(potentio,INPUT);
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Default OLED address, usually
  display.setTextSize(2); 
  display.setTextColor(WHITE);
 
  timer.setInterval(interval);

 // FREQUENCE BUZZER +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, RISING);


// ULTRASON +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(TriggerPin,OUTPUT);
  pinMode(EchoPin,INPUT);
}


//=====================================================ECRAN OLED=========================================================================
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
    tone(buzzer, 2000, 50);     //changer avec potentiomètre 
 
    y=30;
    timer.reset();
 }
}
//=====================================================CAPTEUR ULTRASON=========================================================================

void capteurUltrason(){
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  Duration = pulseIn(EchoPin,HIGH);

  long Distance_cm = Distance(Duration);
/*
  Serial.print("Distance= ");
  Serial.print(Distance_cm);
  Serial.println(" cm ");
 
*/

}

long Distance(long time)
{
  long DistanceCalc;

  DistanceCalc = ((time * 0.034) / 2);

  return DistanceCalc;
}


//=====================================================FREQUENCE BUZZER=========================================================================
void freqBuzzer() {
  if (millis() - previousMillis >= intervalfreq) {
    previousMillis = millis();
    frequency = frequency * 10;


    if(Distance(Duration) > 5 && Distance(Duration) <= 10){
      frequency = frequency *1.3; 
    }

    if(Distance(Duration) > 10 && Distance(Duration) <= 15){
      frequency = frequency *1.6; 
    }

    if(Distance(Duration) > 15 && Distance(Duration) <= 20){
      frequency = frequency *1.9; 
    }
    
    if(Distance(Duration) > 20 && Distance(Duration) <= 25){
      frequency = frequency *2.2; 
    }

    if(Distance(Duration) > 25){
      frequency = frequency *2.5; 
    }



    
    Serial.println(frequency);
    tone(buzzerPin, frequency);
    frequency = 0;
  }
}

void isr() {
  frequency++;
}



//=====================================================LOOOOOOOOOOP========================================================================= 
void loop() 
{
  ecranOLED();
  capteurUltrason();
  freqBuzzer(); 
}
 
