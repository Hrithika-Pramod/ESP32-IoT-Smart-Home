#define BLYNK_TEMPLATE_ID "TMPL3hGL3e1oU"
#define BLYNK_TEMPLATE_NAME "Home automation"
#define BLYNK_AUTH_TOKEN "Tmnlu-NAf7ANyjgjvw8u_DzUHvWwu6NK"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32Servo.h>

char ssid[] = "Karthikeyan";
char pass[] = "12345678";

// Add your pin numbers here
int livingRoomPin =18 ;
int bedRoomPin =19 ;
int nightLightPin = 22;

int ldrPin = 34;
int irPin =32 ;
int dhtPin = 33;
int servoPin =27 ;

Servo gateServo;
DHT dht(dhtPin, DHT11);

BlynkTimer timer;

// Living Room Light
BLYNK_WRITE(V0)
{
  int buttonState = param.asInt();
  digitalWrite(livingRoomPin, buttonState);
}

// Bedroom Light
BLYNK_WRITE(V1)
{
  int buttonState = param.asInt();
  digitalWrite(bedRoomPin, buttonState);
}

// Manual Gate Control
BLYNK_WRITE(V2)
{
  int buttonState = param.asInt();

  if(buttonState == 1)
  {
    gateServo.write(90);
  }
  else
  {
    gateServo.write(0);
  }
}

// Send Temperature & Humidity
void sendSensorData()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);
}

// Automatic Night Light
void automaticNightLight()
{
  int ldrValue = analogRead(ldrPin);

  if(ldrValue > 2000)
  {
    digitalWrite(nightLightPin, HIGH);

    Blynk.virtualWrite(V5, 1);
    Blynk.virtualWrite(V6, "ON");
  }
  else
  {
    digitalWrite(nightLightPin, LOW);

    Blynk.virtualWrite(V5, 0);
    Blynk.virtualWrite(V6, "OFF");
  }
}

// Automatic Gate Using IR Sensor
void automaticGate()
{
  int irState = digitalRead(irPin);

  if(irState == LOW)
  {
    gateServo.write(90);

    delay(3000);

    gateServo.write(0);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(livingRoomPin, OUTPUT);
  pinMode(bedRoomPin, OUTPUT);
  pinMode(nightLightPin, OUTPUT);

  pinMode(irPin, INPUT);

  gateServo.attach(servoPin);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensorData);
  timer.setInterval(1000L, automaticNightLight);
  timer.setInterval(500L, automaticGate);
}

void loop()
{
  Blynk.run();
  timer.run();
}