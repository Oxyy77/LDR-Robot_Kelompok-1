#define pin_MOTOR_DIRL 2
#define pin_MOTOR_PWML 3
#define pin_MOTOR_DIRR 4
#define pin_MOTOR_PWMR 5
 
#define Kec_Min -170
#define Kec_Max  100 // Max = 255
 
const int8_t pinSensor[2] = {A1, A2}; // array pin sensor
int16_t limitSensor[3]; // array limit sensor
int16_t correction = 100; // nilai koreksi kalibrasi sensor
 
void setup() {
  Serial.begin(9600);
  pinMode(pin_MOTOR_DIRL, OUTPUT);
  pinMode(pin_MOTOR_PWML, OUTPUT);
  pinMode(pin_MOTOR_DIRR, OUTPUT);
  pinMode(pin_MOTOR_PWMR, OUTPUT);
 
  for (byte i = 0; i < 2; i++) { // kalibrasi sensor
    pinMode(pinSensor[i], INPUT);
    limitSensor[i] = analogRead(pinSensor[i]) + correction;
    if (limitSensor[i] > 1023) limitSensor[i] = 1023;
    delay(50);
  }
}
 
void loop() {
  Serial.print("|");
  Serial.print(limitSensor[0]);
  Serial.print("|");
  Serial.print(limitSensor[1]);
  Serial.print("| |");
 
  Serial.print(analogRead(pinSensor[0]));
  Serial.print("|");
  Serial.print(analogRead(pinSensor[1]));
  Serial.print("| |");
 
  followLight();
}
 
void followLight() {
  int16_t sensor = BacaSensor();
 
  switch (sensor) {
    case 0b00 : driveMotor(0, 0); Serial.println("|  Stop"); break;        // Belok kiri
    case 0b01 : driveMotor(Kec_Max / 2, Kec_Max); Serial.println("|  Belok Kanan"); break;   
    case 0b10 : driveMotor(Kec_Max, Kec_Max/ 2); Serial.println("|  Belok Kiri"); break;
    case 0b11 : driveMotor(Kec_Max, Kec_Max); Serial.println("|  Lurus"); break;          // Lurus
    }
}
 
int16_t BacaSensor() {
  int16_t bitSensor = 0;
 
  for (byte i = 0; i < 2; i++) {
    if (analogRead(pinSensor[i]) > limitSensor[i]) {
      bitSensor = bitSensor | (0b10 >> i);
      Serial.print("1");
    } else Serial.print("0");
  }
 
  return bitSensor;
}
 
 
void driveMotor(int16_t Left, int16_t Right) {
  if (Left > 0) {
    digitalWrite(pin_MOTOR_DIRR, LOW);
  } else {
    digitalWrite(pin_MOTOR_DIRR, HIGH);
    Left = -Left;
  }
  analogWrite(pin_MOTOR_PWML, Left);
 
  if (Right > 0) {
    digitalWrite(pin_MOTOR_DIRL, LOW);
  } else {
    digitalWrite(pin_MOTOR_DIRL, HIGH);
    Right = -Right;
  }
  analogWrite(pin_MOTOR_PWMR, Right);
  if(Left == 0 && Right == 0){
    digitalWrite(pin_MOTOR_DIRL, LOW);
    digitalWrite(pin_MOTOR_DIRR, LOW);
  }
}