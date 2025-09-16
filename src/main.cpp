#include <Arduino.h>

uint8_t leftMotorDir = 2;
uint8_t leftMotorStep = 3;
uint8_t rightMotorDir = 4;
uint8_t rightMotorStep = 5;

uint8_t gearShift = A0;
uint8_t steeringWheel = A1;
uint8_t throttlePedal = A2;

enum Gear
{
  DRIVE = 2,
  NEUTRAL = 1,
  REVERSE = 0
};
Gear currentGear = NEUTRAL;

int prevGearValue = -1;

const int UP_THRESHOLD = 700;
const int DOWN_THRESHOLD = 300;

void setup()
{
  Serial.begin(9600);
  pinMode(gearShift, INPUT);
  pinMode(steeringWheel, INPUT);
  pinMode(throttlePedal, INPUT);

  pinMode(rightMotorDir, OUTPUT);
  pinMode(rightMotorStep, OUTPUT);
  pinMode(leftMotorDir, OUTPUT);
  pinMode(leftMotorStep, OUTPUT);
}

void loop()
{
  int gearValue = analogRead(gearShift);
  int steeringValue = analogRead(steeringWheel);
  int throttleValue = analogRead(throttlePedal);

  if (gearValue > UP_THRESHOLD && prevGearValue <= UP_THRESHOLD)
  {
    if (currentGear < DRIVE)
      currentGear = (Gear)(currentGear + 1);
  }
  else if (gearValue < DOWN_THRESHOLD && prevGearValue >= DOWN_THRESHOLD)
  {
    if (currentGear > REVERSE)
      currentGear = (Gear)(currentGear - 1);
  }
  prevGearValue = gearValue;

  Serial.print("Gear Value: ");
  Serial.print(gearValue);
  Serial.print(" -> Current Gear: ");
  Serial.println(currentGear);
  Serial.print("Steering Value: ");
  Serial.println(steeringValue);
  Serial.print("Throttle Value: ");
  Serial.println(throttleValue);
}