#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID2 "d62e30e3-08e2-41de-8133-278059c871a2"
#define CHARACTERISTIC_UUID3 "1e580b4e-2c8b-48e7-a325-d3bf5e620082"

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 5;
const int echoPin = 18;


const int IN1Pin = 26;
const int IN2Pin = 27;
const int IN3Pin = 14;
const int IN4Pin = 12;

//Identificadores para el control de la velocidad
const int PWMChannelA = 0;
const int PWMChannelB = 1;
const int PWMPinA = 2;
const int PWMPinB = 15;

const int freq = 1000;
const int resolution = 8;

bool distancia = false;
bool distanciaAtras = false;

const int infrarrojoPin = 4;

int valorInfrarrojo = HIGH;

String xStatus;
String yStatus;

long duration;
float distanceCm;
String distanceWarning;
char distance_array[30] = "¡¡WARNING!!";


// Caracteristica para la componente X
BLECharacteristic xWrite (
  CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_WRITE
);

// Caracteristica para la componente Y
BLECharacteristic yWrite (
  CHARACTERISTIC_UUID2,
  BLECharacteristic::PROPERTY_WRITE
);

// Caracteristica para el sensor de proximidad frontal
BLECharacteristic sensorFrontal (
  CHARACTERISTIC_UUID3,
  BLECharacteristic::PROPERTY_NOTIFY
);

void controlCoche() {
  // Rutina para controlar los motores del coche
  if (xStatus == "1" && yStatus == "0" && !distancia) {
    // Movimiento hacia delante con velocidad baja
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (xStatus == "2" && yStatus == "0" && !distancia) {
    // Movimiento hacia delante con velocidad alta
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (xStatus == "-1" && yStatus == "0" && !distanciaAtras) {
    // Movimiento hacia atrás con velocidad baja
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  }
  else if (xStatus == "-2" && yStatus == "0" && !distanciaAtras) {
    // Movimiento hacia atrás con velocidad alta
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  }
  else if (xStatus == "0" && yStatus == "1" && !distancia) {
    // Giro derecha con velocidad baja
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, LOW);
  }
  else if (xStatus == "0" && yStatus == "2" && !distancia) {
    // Giro derecha con velocidad alta
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, LOW);
  }
  else if (xStatus == "0" && yStatus == "-1" && !distancia) {
    // Giro izquierda con velocidad baja
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (xStatus == "0" && yStatus == "-2" && !distancia) {
    // Giro izquierda con velocidad alta
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (((xStatus == "1" && yStatus == "1") || (xStatus == "1" && yStatus == "2") || (xStatus == "2" && yStatus == "1") || (xStatus == "2" && yStatus == "2")) && !distancia) {
    // Movimiento hacia delante con giro a la derecha si existe distancia suficiente
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (((xStatus == "1" && yStatus == "-1") || (xStatus == "1" && yStatus == "-2") || (xStatus == "2" && yStatus == "-1") || (xStatus == "2" && yStatus == "-2")) && !distancia) {
    // Movimiento hacia delante con giro a la izquierda si existe distancia suficiente
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  }
  else if (((xStatus == "-1" && yStatus == "1") || (xStatus == "-1" && yStatus == "2") || (xStatus == "-2" && yStatus == "1") || (xStatus == "-2" && yStatus == "2")) && !distanciaAtras) {
    // Movimiento hacia atrás con giro a la derecha si existe distancia suficiente
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  }
  else if (((xStatus == "-1" && yStatus == "-1") || (xStatus == "-1" && yStatus == "-2") || (xStatus == "-2" && yStatus == "-1") || (xStatus == "-2" && yStatus == "-2")) && !distanciaAtras) {
    // Movimiento hacia atrás con giro a la izquierda si existe distancia suficiente
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  } else {
    // Parada de los motores
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, LOW);
  }
}

void lecturaMando() {
  // Rutina para leer los valores del mando
  std::string xControlValue = xWrite.getValue();
  std::string yControlValue = yWrite.getValue();

  if ((xControlValue != xStatus.c_str() || yControlValue != yStatus.c_str())) {
    Serial.print("Value changed.. new value: x: ");
    Serial.print(xControlValue.c_str());
    Serial.print(" ,y: ");
    Serial.println(yControlValue.c_str());
    xStatus = xControlValue.c_str();
    yStatus = yControlValue.c_str();
  }
}

void lecturaSensor() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Grupo5");
  BLEServer *pServer = BLEDevice::createServer(); // Crear servidor
  BLEService *pService = pServer->createService(SERVICE_UUID);  // Crear servicio

  pService->addCharacteristic(&xWrite);
  pService->addCharacteristic(&yWrite);
  pService->addCharacteristic(&sensorFrontal);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Funcion que ayuda con la conexión de iPhone
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined!");

  //Configuración sensor de proximidad frontal
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(IN1Pin, OUTPUT); // Sets the IN1Pin as an Output
  pinMode(IN2Pin, OUTPUT); // Sets the IN2Pin as an Output
  pinMode(IN3Pin, OUTPUT); // Sets the IN3Pin as an Output
  pinMode(IN4Pin, OUTPUT); // Sets the IN4Pin as an Output

  ledcSetup(PWMChannelA, freq, resolution);
  ledcSetup(PWMChannelB, freq, resolution);

  ledcAttachPin(PWMPinA, PWMChannelA);
  ledcAttachPin(PWMPinB, PWMChannelB);
  ledcWrite(PWMChannelA, 96);
  ledcWrite(PWMChannelB, 96);
  digitalWrite(IN1Pin, LOW);
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);

  pinMode(infrarrojoPin, INPUT);


  xStatus = "0";
  yStatus = "0";
}

void loop() {

  lecturaSensor();
  
  if (distanceCm < 20) {
    distanceWarning = "¡¡DISTANCIA MENOR A 20 CENTIMETROS!!";

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    //distance_array=distanceWarning.c_str();
    Serial.println(distance_array);

    dtostrf(distanceCm, 4, 2, distance_array);
    sensorFrontal.setValue(distance_array);
    sensorFrontal.notify();

    distancia = true;
  } else
    distancia = false;

  valorInfrarrojo = digitalRead(infrarrojoPin);
  if (valorInfrarrojo == HIGH) {
    distanciaAtras = false;
  } else {
    distanciaAtras = true;
  }

  lecturaMando();

  controlCoche();

  delay(50);
}
