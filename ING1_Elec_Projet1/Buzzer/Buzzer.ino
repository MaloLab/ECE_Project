//int BPM=30;
float buzzer;
int son=5;
int timbre;
const int Bouton=2;
bool state = true;
 
float SensorPIN=1;
float BPM=0;
 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(son, OUTPUT);
pinMode(Bouton, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(Bouton), changer, FALLING);
}
 
void loop() {
  // put your main code here, to run repeatedly:
BPM=analogRead(SensorPIN)*0.12;
Serial.println(BPM);
if (state == true)
{
buzzer=60000/BPM;
//Serial.println(buzzer);
if ((BPM < 50))
{
  timbre=850;
}
  else if ((BPM >= 50) && (BPM <= 100))
  {
    timbre=500;
  }
 
  else if (BPM>100)
  {
    timbre=150;
  }
 
  
tone(son,timbre);
delay(5);
noTone(son);
delay(buzzer);
}
else if (state == false)
{
  noTone(son);
}
}
 
void changer()
{
  state=!state;
  delay(100);
}
