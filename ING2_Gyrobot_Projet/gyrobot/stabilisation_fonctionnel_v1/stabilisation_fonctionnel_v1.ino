#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1_bc.h>

// Déclaration des objets Servo et MPU6050
Servo monServo1, monServo2;
MPU6050 mpu;

// Déclaration des variables pour les mesures du capteur
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Déclaration des variables pour le PID
double setpoint = 0;  // Angle désiré (0 pour maintenir l'équilibre)
double input, output;  // Variables pour l'entrée et la sortie du PID
double Kp = 600.0, Ki = 5.0, Kd = 5.0;  // PID coefficients
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);  // Initialisation du PID

// Vitesse de base pour les servos
int vitesse = 90;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialisation de la communication I2C
  mpu.initialize();  // Initialisation du capteur MPU6050


  // Configuration du PID
  myPID.SetMode(AUTOMATIC);  // Mode automatique pour le PID
  myPID.SetOutputLimits(-20000, 20000);  // Limites de sortie du PID

  // Attachement des servos aux broches correspondantes
  monServo1.attach(10);
  monServo2.attach(11);
}

void loop() {
  // Lecture des valeurs du capteur MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calcul de l'angle d'inclinaison à partir des données de l'accéléromètre
  double angle = (atan2(ay, az) * 180 / PI) - 90;  // Conversion des radians en degrés et ajustement par rapport à la verticale
  input = angle;  // Définition de l'entrée du PID comme étant l'angle mesuré

  // Calcul du PID
  myPID.Compute();  // Exécution du calcul PID

 
  
  if(angle > 1 || angle < -1) {
    monServo1.write(vitesse-1.8*angle);
    monServo2.write(vitesse+1.8*angle);
  }
  else {
    monServo1.write(vitesse);
    monServo2.write(vitesse);    
  }
  
  
  
  // Affichage des données pour le débogage
  Serial.print("Angle: ");
  Serial.println(angle);
  Serial.print(" | PID Output: ");
  Serial.println(output);


  // Petit délai pour stabiliser les lectures
  delay(5);
}
