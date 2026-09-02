#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi details
char ssid[] = "1234578";
char pass[] = "12345678";

// Create WiFi and MQTT connection
WiFiClient wifi;
PubSubClient mqtt(wifi);

// MQTT broker details
const char broker[] = "broker.emqx.io";
const int port = 1883;

// MQTT topics
const char wave[] = "ES/Wave";
const char pat[] = "ES/Pat";

// Ultrasonic sensor pins
const int trig = 9;
const int echo = 10;

// LED pins
const int led1 = 6;
const int led2 = 7;

long distance;
long timeTaken;


// This function runs when a message is received
void messageReceived(char* topic, byte* message, unsigned int length)
{
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");

  // Print the received message
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
  }

  Serial.println();

  // If a wave message is received, turn both LEDs ON
  if (strcmp(topic, wave) == 0)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    Serial.println("Lights ON");
  }

  // If a pat message is received, turn both LEDs OFF
  if (strcmp(topic, pat) == 0)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    Serial.println("Lights OFF");
  }
}


void setup()
{
  // Start serial communication
  Serial.begin(9600);

  // Set ultrasonic sensor pins
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Set LED pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Start with LEDs OFF
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);


  // Connect to WiFi
  Serial.print("Connecting to WiFi");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(2000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");


  // Set MQTT broker
  mqtt.setServer(broker, port);

  // Set function for received messages
  mqtt.setCallback(messageReceived);


  // Connect to MQTT
  while (!mqtt.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (mqtt.connect("ArduinoSIT210"))
    {
      Serial.println("MQTT connected");

      // Subscribe to wave and pat topics
      mqtt.subscribe(wave);
      mqtt.subscribe(pat);
    }
    else
    {
      Serial.println("MQTT connection failed");
      delay(3000);
    }
  }
}


void loop()
{
  // Check MQTT connection
  if (!mqtt.connected())
  {
    while (!mqtt.connected())
    {
      if (mqtt.connect("ArduinoSIT210"))
      {
        mqtt.subscribe(wave);
        mqtt.subscribe(pat);
      }
      else
      {
        delay(3000);
      }
    }
  }

  // Check for new MQTT messages
  mqtt.loop();


  // Send a short pulse from ultrasonic sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);


  // Measure the returned signal
  timeTaken = pulseIn(echo, HIGH);

  // Calculate distance in centimetres
  distance = timeTaken * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // Detect a wave
  // Change these values after testing your sensor
  if (distance >= 10 && distance <= 30)
  {
    Serial.println("Wave detected");

    // Send name to ES/Wave
    mqtt.publish(wave, "Madhur");

    delay(2000);
  }


  // Detect a pat
  if (distance < 10 && distance > 0)
  {
    Serial.println("Pat detected");

    // Send name to ES/Pat
    mqtt.publish(pat, "Madhur");

    delay(2000);
  }

  // Small delay before taking another reading
  delay(100);
}
