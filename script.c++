#include <ArduinoJson.h>
#include <Servo.h>
#include <DHT.h>

#define pinServo 4
#define dhttype DHT11
#define pinDHT 7
#define trigger 6
#define echo 5

Servo myServo; 
int dist = 0;
char resposta = '0';

DHT dht(pinDHT, dhttype);

StaticJsonDocument<100> json;

void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(pinServo, OUTPUT);
  myServo.attach(pinServo);
  myServo.write(0);

  dht.begin();
}

void loop() {

  digitalWrite(trigger, HIGH);
  delayMicroseconds(2000);
  digitalWrite(trigger, LOW);

  dist = pulseIn(echo, HIGH);
  dist = dist / 58;


  if(Serial.available() > 0){
    resposta = Serial.read();
  }

 
  if(resposta == '0' || (dist < 20 && resposta == 'a')){
    myServo.write(0);
  } else if(resposta == '1' || (dist >= 60 && resposta == 'a')){
    myServo.write(180);
  }


  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  json["DistanciaAtual"] = dist;
  json["Temperatura"] = temperatura;
  json["Umidade"] = umidade;


  serializeJson(json, Serial);
  Serial.println();

  delay(500);
}