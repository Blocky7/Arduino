#include <Servo.h>

#define MOTOR1_IN 2
#define MOTOR1_PWM 3

Servo rotator;

void setup() {
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  rotator.attach(11);
  rotator.write(90);
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  
}

void serialEvent() {
  String data = Serial.readString();
  if(data == "U")
  {
    rotator.write(90);
    motorControl(1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "D")
  {
    rotator.write(90);
    motorControl(-1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "R")
  {
    rotator.write(65);
    motorControl(1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "L")
  {
    rotator.write(125);
    motorControl(1, MOTOR1_IN, MOTOR1_PWM);
  }
  else if(data == "S")
  {
    rotator.write(90);
    motorControl(0, MOTOR1_IN, MOTOR1_PWM);
  }
}

// принимает знач. 0-1023, пин IN и PWM
void motorControl(int val, byte pinIN, byte pinPWM) {
  val = 127*val;

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
