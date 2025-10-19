const int interruptPin = 3;
const int buzzerPin = 9;

volatile int frequency = 0;
volatile unsigned long previousMillis = 0;
volatile unsigned long interval = 100;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, RISING);
}

void freqBuzzer() {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    frequency = frequency * 10;
    Serial.print("Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");
    tone(buzzerPin, frequency);
    frequency = 0;
  }
}

void isr() {
  frequency++;
}


void loop(){
  freqBuzzer();
}
