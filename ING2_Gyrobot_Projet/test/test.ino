#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1_bc.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
double setpoint = 0;
double input, output;
double Kp = 500.0, Ki = 5.0, Kd = 5.0;
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
Servo monServo1, monServo2;
int vitesse = 90;
int vitesse1, vitesse2;

void setup() {
  Serial.begin(9600);
  monServo1.attach(10);
  monServo2.attach(11);
  Wire.begin();
  mpu.initialize();
  /*if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  else{Serial.println("MPU6050 connection successful");}*/
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-20000, 20000);
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  double angle = (atan2(ay, az) * 180 / PI) - 90;


  if(angle > 0) {
    monServo1.write(vitesse+angle);
    monServo2.write(vitesse-angle);
  }
  if(angle <= 0) {
    monServo1.write(vitesse-angle);
    monServo2.write(vitesse+angle);
  }
  Serial.println("angle");
  Serial.println(angle);
  Serial.println("output");
  Serial.println(output);
}
