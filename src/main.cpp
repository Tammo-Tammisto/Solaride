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

static bool stepState = false;

const int MAX_DELAY = 1000;
const int MIN_DELAY = 10;

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

  if (currentGear == DRIVE && throttleValue != 0)
  {
    digitalWrite(leftMotorDir, LOW);
    digitalWrite(rightMotorDir, HIGH);

    stepState = !stepState;
    if (stepState)
    {
      digitalWrite(leftMotorStep, HIGH);
      digitalWrite(rightMotorStep, LOW);
    }
    else
    {
      digitalWrite(leftMotorStep, LOW);
      digitalWrite(rightMotorStep, HIGH);
    }
    delayMicroseconds(map(throttleValue, 0, 1023, MAX_DELAY, MIN_DELAY));
  }
  else if (currentGear == REVERSE && throttleValue != 0)
  {
    digitalWrite(leftMotorDir, HIGH);
    digitalWrite(rightMotorDir, LOW);

    stepState = !stepState;
    if (stepState)
    {
      digitalWrite(leftMotorStep, HIGH);
      digitalWrite(rightMotorStep, LOW);
    }
    else
    {
      digitalWrite(leftMotorStep, LOW);
      digitalWrite(rightMotorStep, HIGH);
    }
    delayMicroseconds(map(throttleValue, 0, 1023, MAX_DELAY, MIN_DELAY));
  }
  else
  {
    digitalWrite(leftMotorStep, LOW);
    digitalWrite(rightMotorStep, LOW);
    delay(100);
  }

  // Serial.print("Gear Value: ");
  // Serial.print(gearValue);
  // Serial.print(" -> Current Gear: ");
  // Serial.println(currentGear);
  // Serial.print("Steering Value: ");
  // Serial.println(steeringValue);
  // Serial.print("Throttle Value: ");
  // Serial.println(throttleValue);
}

// Sinu ülesandeks on kirjutada kood, mis vastab järgmistele nõuetele:
//
// ● Autol on kolm käiku, mille vahel saab vahetada käigukangile üles/alla vajutades:
// ○ DRIVE – gaasi andes pöörlevad mootorid pärisuunas
// ○ NEUTRAL – mootorid ei pöörle
// ○ REVERSE – gaasi andes pöörlevad mootorid vastassuunas
//
// ● Gaasipedaali vajutades (potentsiomeetrit üles lohistades) saab kontrollida auto üldist kiirust
//
// ● Rooli keerates saab kontrollida auto kiiruse jaotust:
// ○ Kui rool on otse, siis pöörlevad mootorid sama kiirusega
// ○ Kui rooli keeratakse paremale, siis vasak mootor pöörleb kiiremini ja parem mootor aeglasemalt
// ○ Kui rooli keeratakse vasakule, siis parem mootor pöörleb kiiremini ja vasak mootor aeglasemalt
// ○ Mida rohkem rooli keeratakse, seda tugevam on mõju
//
// ● Mootorid pöörlevad sujuvalt ja nende töös ei toimu märgatavaid peatusi
//
// ● Ei tohi kasutada väliseid teeke