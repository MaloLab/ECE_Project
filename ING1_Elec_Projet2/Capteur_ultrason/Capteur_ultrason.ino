#include <SimpleTimer.h>
#include <Adafruit_SSD1306.h>
 

const int TriggerPin = 8;
const int EchoPin = 9;
long Duration = 0;

void setup() {

 pinMode(TriggerPin,OUTPUT);
 pinMode(EchoPin,INPUT);
 Serial.begin(9600);
}


void capteurUltrason(){
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  Duration = pulseIn(EchoPin,HIGH);

  long Distance_cm = Distance(Duration);

  Serial.print("Distance= ");
  Serial.print(Distance_cm);
  Serial.println(" cm");

}
long Distance(long time)
{
  long DistanceCalc;

  DistanceCalc = ((time * 0.034) / 2);

  return DistanceCalc;
}

void loop() {

capteurUltrason();
  
}
