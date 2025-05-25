#include <ESP32Servo.h>

#define SERVO_PIN 14
int i;

Servo myServo;

void setup() {
  //pinMode(LED, OUTPUT);
  myServo.attach(SERVO_PIN);
  
}

void loop() {
  myServo.write(0); // Установить угол 0 градусов
  delay(1500); // Задержка 1 секунда
  myServo.write(90); // Установить угол 270 градус
  delay(1500);
 
}
