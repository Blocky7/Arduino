#include "TM1637.h"   // Скачайте библиотеку ниже и подключите ее
int8_t DispMSG[] = {0, 0, 0, 0};   // Настройка символов для последующего вывода на дисплей
//Определяем пины для подключения к плате Arduino
#define CLK 6
#define DIO 7
//Создаём объект класса TM1637, в качестве параметров
//передаём номера пинов подключения
TM1637 tm1637(CLK, DIO);

int LEDPin = 5;

// Pin to connect to your conductive sensor 
// (paperclip, conductive paint/fabric/thread, wire)
int capSensePin = 2;
int capSensePin2 = 3;

// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
// If this is not sensitive enough, try a resistor with more ohms.
int touchedCutoff = .5;
bool isrunning = false;
unsigned int curTime = 0;
unsigned int startTime = 0;
int prevRead = 0;
unsigned int charge = 0;
bool charging = false;
unsigned int chargeTime = 0;

void setup(){
  Serial.begin(9600);
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  tm1637.point(true);
  tm1637.display(DispMSG);
  // Set up the LED
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
  pinMode(capSensePin, INPUT);
  pinMode(capSensePin2, INPUT);
}

void loop(){
  if(isrunning)
  {
    uint32_t seconds = curTime / 1000;
    int milliseconds = curTime % 1000;
    Serial.print(seconds);
    Serial.print(".");
    if (milliseconds < 100)
      Serial.print("0");
    if (milliseconds < 10)
      Serial.print("0");
    Serial.println(milliseconds);
    DispMSG[0] = seconds / 10;
    DispMSG[1] = seconds % 10;
    DispMSG[2] = milliseconds/100;
    DispMSG[3] = (milliseconds%100)/10;
    tm1637.display(DispMSG);
  }
  //readCapacitivePin(capSensePin)
  int curRead = int(digitalRead(capSensePin)) + int(digitalRead(capSensePin2));
  if(curRead == 0 && prevRead >= 1)
  {
    if(isrunning && curTime > 1000)
    {
      Serial.println("DONE");
      isrunning = false;
    }
    else if(!isrunning && !charging)
    {
      charging = true;
      chargeTime = millis();
    }
  }
  else if(curRead >= 1 && prevRead == 0 && digitalRead(LEDPin) == HIGH)
  {
    charge = 0;
    isrunning = true;
    digitalWrite(LEDPin, LOW);
    startTime = millis();
  }
  if(isrunning)
    curTime = millis()-startTime;
  else if(charging)
    charge = millis()-chargeTime;
  if(charge > 500)
  {
    digitalWrite(LEDPin, HIGH);
    charging = false;
  }
  else 
    digitalWrite(LEDPin, LOW);
  prevRead = curRead;
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a resistor pulling
//  it up to +5v.
uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}
