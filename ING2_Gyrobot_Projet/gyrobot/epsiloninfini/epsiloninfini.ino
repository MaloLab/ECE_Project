#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1_bc.h>
#include <SimpleKalmanFilter.h>

// Déclaration des objets Servo et MPU6050
Servo monServo1, monServo2;
MPU6050 mpu;

// Déclaration des variables pour les mesures du capteur
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Paramètres du filtre de Kalman pour l'angle
float mesure_noise = 3.0;  // Bruit de mesure
float estime_error = 3.0;  // Erreur estimée
float process_noise = 0.03;  // Bruit du processus
SimpleKalmanFilter kalmanAngle(mesure_noise, estime_error, process_noise);

// Déclaration des variables pour le PID
double setpoint = -15;  // Angle désiré (0 pour maintenir l'équilibre)
double input, output;  // Variables pour l'entrée et la sortie du PID
double Kp = 600.0, Ki = 5.0, Kd = 5.0;  // PID coefficients
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);  // Initialisation du PID

// Vitesse de base pour les servos
int vitesse = 90;

// Variables pour la moyenne des angles
double sumAngle = 0.0;
int countReadings = 0;
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialisation de la communication I2C
  mpu.initialize();  // Initialisation du capteur MPU6050
  myPID.SetMode(AUTOMATIC);  // Mode automatique pour le PID
  myPID.SetOutputLimits(-20000, 20000);  // Limites de sortie du PID
  monServo1.attach(10);
  monServo2.attach(11);
  startTime = 1000;  // Enregistrer le temps de début
}

void loop() {
  // Lecture des valeurs du capteur MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calcul de l'angle d'inclinaison à partir des données de l'accéléromètre
  double angle_raw = (atan2(ay, az) * 180 / PI) - 90;  // Conversion des radians en degrés
  double angle = kalmanAngle.updateEstimate(angle_raw);  // Filtrer l'angle avec le filtre de Kalman
  input = angle;  // Définition de l'entrée du PID comme étant l'angle filtré

  // Accumuler les angles pour la moyenne
  sumAngle += angle;
  countReadings++;

  // Calcul du PID
  myPID.Compute();

  // Contrôle des servos en fonction de l'angle et du PID
  if (angle > 2 + setpoint || angle < -2 + setpoint) {
    monServo1.write(vitesse - 2 * angle);
    monServo2.write(vitesse + 2 * angle);
  } else {
    monServo1.write(vitesse);
    monServo2.write(vitesse);
  }

  // Affichage des données pour le débogage
  Serial.print("angle: ");
  Serial.println(angle);

  // Vérifier si 20 secondes se sont écoulées
  if (millis() - startTime >= 10000) {
    double averageAngle = sumAngle / countReadings;  // Calculer la moyenne
    Serial.print("Moyenne: ");
    Serial.println(averageAngle);
    // Réinitialiser les valeurs pour une nouvelle période de mesure
    sumAngle = 0.0;
    countReadings = 0;
    startTime = millis();
  }

  // Petit délai pour stabiliser les lectures
  delay(5);
}
