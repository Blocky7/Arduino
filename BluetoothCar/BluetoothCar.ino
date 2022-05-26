#include <Servo.h>

#define MOTOR_IN 2
#define MOTOR_PWM 3
#define MOTOR1_IN 4
#define MOTOR1_PWM 5

Servo ser;

void setup() {
  pinMode(MOTOR_IN, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  ser.attach(11);
  ser.write(90);
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  
}

void serialEvent() {
  String data = Serial.readString();
  if(data == "U")
  {
    motorControl(1, MOTOR_IN, MOTOR_PWM);
    motorControl(1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "D")
  {
    motorControl(-1, MOTOR_IN, MOTOR_PWM);
    motorControl(-1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "R")
  {
    motorControl(1, MOTOR_IN, MOTOR_PWM);
    motorControl(-1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "L")
  {
    motorControl(-1, MOTOR_IN, MOTOR_PWM);
    motorControl(1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "S")
  {
    motorControl(0, MOTOR_IN, MOTOR_PWM);
    motorControl(0, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "F")
  {
    shoot();
  }
}

void motorControl(int val, byte pinIN, byte pinPWM) {
  val = val * 255;
  
  if (val > 0) {  // вперёд
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 0);
  } else if (val < 0) {  // назад
    analogWrite(pinPWM, 255 + val);
    digitalWrite(pinIN, 1);
  } else {  // стоп
    digitalWrite(pinIN, 0);
    digitalWrite(pinPWM, 0);
  }
}

void shoot() {
  ser.write(180);
  delay(500);
  ser.write(90);
  delay(500);
}
