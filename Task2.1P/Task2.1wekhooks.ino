#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include "secretinfo.h"

// DHT sensor setup
#define DHT_PIN 2
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Analogue light sensor setup
#define LIGHT_SENSOR_PIN A0



// ThingSpeak channel details
WiFiClient client;
unsigned long myChannelNumber = CH_ID;
const char* myWriteAPIKey = WRITE_APIKEY;

// Sensor readings
float temperature = 0;
int lightLevel = 0;

// Connect to Wi-Fi
void connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(SSID, PASS);
        Serial.print(".");
        delay(3000);
    }
    Serial.println();
    Serial.println("Wi-Fi connected.");
}

// Read temperature from DHT sensor
void readTemperature()
{
    temperature = dht.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
}

// Read light level from analogue light sensor
void readLight()
{
    lightLevel = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Light Level: ");
    Serial.println(lightLevel);
}

// Send temperature and light data to ThingSpeak
void sendDataToThingSpeak()
{
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, lightLevel);
    int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (result == 200)
    {
        Serial.println("ThingSpeak update successful.");
    }
    else
    {
        Serial.println("ThingSpeak update failed. HTTP error code " + String(result));
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    dht.begin();

    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Wi-Fi module not detected.");
        while (true)
        {
            ;
        }
    }

    ThingSpeak.begin(client);
}

void loop()
{
    connectWiFi();
    readTemperature();
    readLight();

    if (isnan(temperature))
    {
        Serial.println("Failed to read from DHT sensor.");
    }
    else
    {
        sendDataToThingSpeak();
    }

    delay(30000);
}