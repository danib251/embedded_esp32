#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(921600);
  Serial.println("Hello, ESP32!");
}

void loop()
{
  // put your main code here, to run repeatedly:

  Serial.println("Looping...");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}