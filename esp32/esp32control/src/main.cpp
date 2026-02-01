#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// Настройки сети
const char* ssid = "Trrr";
const char* password = "1419trr14";
const char* mqtt_server = "d1ec323fd6904363bc62ae1f22132549.s1.eu.hivemq.cloud";
const char* mqtt_user = "krushka";
const char* mqtt_password = "Kr5st9k2006";

// Пины для сервопривода и шагового двигателя
const int servoPin = 15;
const int servoPin2 = 14;
const int servoPin3 = 12;
// Шаговый двигатель 1
const int stepper1StepPin = 18;
const int stepper1DirPin = 19;

// Параметры шаговых двигателей
const int stepsPerRevolution = 400;

const int LED = 2; // Светодиод

Servo myServo;
Servo secondServo;
Servo thirdServo;
WiFiClientSecure espClient;
PubSubClient *client;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, LOW); // Выключить светодиод при подключении к WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void rotateStepper(int stepPin, int dirPin, int degrees, bool clockwise) {
  // Рассчитывает количество шагов
  int steps = (degrees / 360.0) * stepsPerRevolution;
  
  // Устанавливаем направление
  digitalWrite(dirPin, clockwise ? HIGH : LOW);
  
  // Выполняем шаги
  for(int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Обработка сервоприводов
  if (String(topic) == "servo/control") {
    int angle = message.toInt();
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle);
      Serial.print("Servo set to: ");
      Serial.println(angle);
    }
  }

  if (String(topic) == "servo2/control") {
    int angle = message.toInt();
    if (angle >= 0 && angle <= 180) {
      secondServo.write(angle);
      Serial.print("Servo set to: ");
      Serial.println(angle);
    }
  }

  if (String(topic) == "servo3/control") {
    int angle = message.toInt();
    if (angle >= 0 && angle <= 180) {
      thirdServo.write(angle);
      Serial.print("Servo set to: ");
      Serial.println(angle);
    }
  }
  
  // Обработка шагового двигателя 
  if (String(topic) == "stepper1/control") {
    int deIndex = message.indexOf("de");
    int diIndex = message.indexOf("di");
    
    if (deIndex != -1 && diIndex != -1) {
        // Парсинг градусов (все что между "de" и "di")
        int degrees = message.substring(deIndex + 2, diIndex).toInt();
        
        // Парсинг направления (все что после "di")
        String dirStr = message.substring(diIndex + 2);
        bool direction = dirStr == "cw"; // true для CW, false для CCW
        
        Serial.print("Stepper 1 rotating: ");
        Serial.print(degrees);
        Serial.print(" degrees ");
        Serial.println(direction ? "CCW" : "CW");
        
        rotateStepper(stepper1StepPin, stepper1DirPin, degrees, direction);
    }
  }
}

void reconnect() {
  while (!client->connected()) {
    digitalWrite(LED, LOW); // Выключить светодиод при отсутствии подключения
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32MotorController";
    
    if (client->connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      digitalWrite(LED, HIGH); // Включить светодиод при успешном подключении
      client->subscribe("servo/control");
      client->subscribe("servo2/control");
      client->subscribe("servo3/control");
      client->subscribe("stepper1/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // Изначально выключить светодиод
  
  // Настройка сервопривода
  myServo.attach(servoPin);
  secondServo.attach(servoPin2);
  thirdServo.attach(servoPin3);
  
  // Настройка шаговых двигателей
  pinMode(stepper1StepPin, OUTPUT);
  pinMode(stepper1DirPin, OUTPUT);
  
  setup_wifi();
  
  espClient.setInsecure();
  client = new PubSubClient(espClient);
  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);
}

void loop() {
  if (!client->connected()) {
    reconnect();
  }
  
  client->loop();
}