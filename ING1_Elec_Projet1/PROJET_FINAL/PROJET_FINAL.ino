 
//////////////////////////////////////////
#include <Ds1302.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const unsigned char epd_bitmap_V3 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00,
  0x07, 0xe3, 0xf0, 0x00, 0x0f, 0xf7, 0xf8, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00,
  0x1f, 0x7f, 0xfe, 0x00, 0x1f, 0x7f, 0xfe, 0x00, 0x1f, 0x7f, 0xfe, 0x00, 0x1f, 0xdf, 0xfc, 0x00,
  0x08, 0x8c, 0x00, 0x00, 0x07, 0xb7, 0xfc, 0x00, 0x0f, 0xbf, 0xf8, 0x00, 0x07, 0xbf, 0xf0, 0x00,
  0x03, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7f, 0x00, 0x00,
  0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00 
}; // dessin ptit coeur
 
const unsigned char epd_bitmap_V4 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x7f, 0x80, 0x00, 0x7f, 0x80, 0x00, 0x2f, 
    0x00, 0x00, 0x3f, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // dessin gros coeur

///////////////////LED///////////////////
int LedRouge=10;
int LedVert=12;
int LedJaune=11;


////////////////////////////////////////////
int x=0;
float y=0; 
int x0=0;
float y0=0;
int Val;
float vpotent = 0.0;
const int valeurlue=A0;
float BPM, potent;
int sensorPIN = 1;
int jour,mois,annee,heure,minute,seconde;
Ds1302 rtc(8, 6, 7); //ENA, CLK, DAT

const static char* WeekDays[] =
{
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---SET UP---/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{

  // initialize the RTC
  rtc.init();
  Serial.begin(9600);

  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Default OLED address, usually
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D); // Second OLED address, via onboard jumper
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---SET---////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set() // set l'heure au branchement (a modifier car c'est un ccompteur) 
{


        Ds1302::DateTime dt;
        dt.year = 22;
        dt.month = 12;
        dt.day = 12;
        dt.dow = 1;

        dt.hour = 21;
        dt.minute =13;
        dt.second = 0;

        // set the date and time
        rtc.setDateTime(&dt);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---ECRAN_1---/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ecran_1(){
BPM = analogRead(sensorPIN)*0.12;  

vpotent=analogRead(potent)*5.0/1023.0;
display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  
  //set();
Ds1302::DateTime now;
    rtc.getDateTime(&now);

    static uint8_t last_second = 0;
    //if (last_second != now.second){
        last_second = now.second;
        display.setCursor(70,2); 
        
        
        display.print(WeekDays[now.dow - 1]); // 1-7
        display.print(' ');

        display.setCursor(40,25);
        if (now.hour < 10) display.print('0');
        display.print(now.hour);    // 00-23
        display.print(':');
        if (now.minute < 10) display.print('0');
        display.print(now.minute);  // 00-59
        display.print(':');
        if (now.second < 10) display.print('0');
        display.print(now.second);  // 00-59
        display.println();
    //}

    delay(100);
  
  display.setCursor(0,5);
  display.print("BPM :");
  if(BPM<80 && BPM>65)//on a mis 65 au lieu de 60 pour un meilleure affichage
  {
    display.println(BPM);
  }
  else 
  {
    display.print("--");

  }

  display.setCursor(0,13);
  display.print("ZOOM : ");
  display.println(vpotent);

  

  display.drawBitmap(85,16, epd_bitmap_V4, 20, 10, WHITE);
  delay(40);
  display.display();


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---ECRAN_2---////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ecran_2() {

int frq = map(analogRead(sensorPIN), 0, 800, 0, 6);
y = 26- frq*vpotent;
  
display2.drawLine(0,25,128,25,WHITE);      /// DEFINITION DU REPERE
display2.drawLine(0,0,0,64,WHITE);

display2.drawLine(30,23,30,27,WHITE);
display2.drawLine(60,23,60,27,WHITE);    /// GRADUATION SUR L'AXE DES X
display2.drawLine(90,23,90,27,WHITE);

display2.drawLine(123,22,127,25,WHITE);   /// FLECHE DU REPERE
display2.drawLine(127,25,123,28,WHITE);

display2.drawLine(0,10,3,10,WHITE);    /// GRADUATION SUR L'AXE DES y
display2.drawLine(0,20,3,20,WHITE);

display2.drawLine(0,0,4,4,WHITE);   /// FLECHE POUR L'AXE DES Y





display2.drawLine(x0,y0,x,y,WHITE); // dessine ligne de x1,y1 à x2,y2 
y0=y;
x0=x;
 x++;
 // incrémente x à chaque passage
if (x>127) {

  x=0; // RAZ x de 127=>0
  display2.clearDisplay();
  x0=0; //réinitialise aussi x0 pour retour propre
}

delay (20); // entre 2 mesures

display2.display();

 
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---s_prog LED---/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void LED() {
  // put your main code here, to run repeatedly:
 
  if ((BPM < 30) || (BPM > 120))
  {
    digitalWrite(LedRouge, HIGH);
    digitalWrite(LedJaune, LOW);
    digitalWrite(LedVert, LOW);
  }
  else if ((BPM >= 60) && (BPM <= 100))
  {
    digitalWrite(LedVert, HIGH);
    digitalWrite(LedRouge, LOW);
    digitalWrite(LedJaune, LOW);
  }
  else
  {
    digitalWrite(LedJaune, HIGH);
    digitalWrite(LedRouge, LOW);
    digitalWrite(LedVert, LOW);
  }
 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////---LOOP---/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
  
  ecran_1();
  ecran_2();
  LED();

  Serial.println(y);
}
