#include <Wire.h>
#include <BH1750.h>

BH1750 lightSensor;

// Pins
const int PIR_PIN = 2;
const int BUTTON_PIN = 3;
const int LED1_PIN = 4;
const int LED2_PIN = 5;

// Darkness threshold
const float DARK_LEVEL = 30.0;

// Light ON time
const unsigned long LIGHT_TIME = 20000;  // 20 seconds

// Interrupt flags
volatile bool pirTriggered = false;
volatile bool buttonTriggered = false;

// Timer
unsigned long lightStartTime = 0;
bool lightsOn = false;

// PIR interrupt
void pirInterrupt()
{
  pirTriggered = true;
}

// Button interrupt
void buttonInterrupt()
{
  buttonTriggered = true;
}

void setup()
{
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Start BH1750
  Wire.begin();

  if (lightSensor.begin())
  {
    Serial.println("BH1750 has detected darkness.");
  }
  else
  {
    Serial.println("BH1750 has not detected.");
  }

  // PIR interrupt
  attachInterrupt(
    digitalPinToInterrupt(PIR_PIN),
    pirInterrupt,
    RISING
  );

  // Push button interrupt
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonInterrupt,
    FALLING
  );

  Serial.println("System ready.");
}

void loop()
{
  // PIR MOTION

  if (pirTriggered)
  {
    pirTriggered = false;

    float lux = lightSensor.readLightLevel();

    Serial.print("Motion has been detected. Light: ");
    Serial.print(lux);
    Serial.println(" lux");

    // Only turn on automatically when dark
    if (lux < DARK_LEVEL)
    {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);

      lightsOn = true;
      lightStartTime = millis();

      Serial.println("Lights ON for 20 seconds");
    }
    else
    {
      Serial.println("Lights remain OFF");
    }
  }
  // PUSH BUTTON

  if (buttonTriggered)
  {
    buttonTriggered = false;

    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);

    lightsOn = true;
    lightStartTime = millis();

    Serial.println("Button has been pressed. Lights ON for 20 seconds");
  }
  // 20 SECOND TIMER

  if (lightsOn &&
      millis() - lightStartTime >= LIGHT_TIME)
  {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    lightsOn = false;

    Serial.println("20 seconds have been completed");
  }

  delay(100);
}
