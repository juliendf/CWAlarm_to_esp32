#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "certs.h"
#include "parameters.h"

const int ledRed = 2;
const int ledYellow = 4;
const int ledGreen = 5;

WiFiClientSecure secureClient = WiFiClientSecure();
PubSubClient mqttClient(secureClient);
char in_message[100];

// connecting to wifi
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
  }
}

// connecting to aws
void connectToAWS() {
  secureClient.setCACert(AWS_PUBLIC_CERT);
  secureClient.setCertificate(AWS_DEVICE_CERT);
  secureClient.setPrivateKey(AWS_PRIVATE_KEY);
  mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
  mqttClient.setCallback(callback);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to AWS MQTT ....");
    mqttClient.connect(DEVICE_NAME);
    delay(5000);
  } 
  if(mqttClient.connected()){
    Serial.println("AWS MQTT Connected!");
    boolean mqttConnected= mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    if(mqttConnected){
      Serial.println((String)"Subscribe to MQTT Topic : " + AWS_IOT_SUBSCRIBE_TOPIC);
      }
    else {
      Serial.println((String)"Cannot connect to MQTT Topic : " + AWS_IOT_SUBSCRIBE_TOPIC);
      }
  }
}
// Read MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic [");
  Serial.print(topic);
  Serial.print("] ");
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  if (doc["NewStateValue"]=="ALARM"){
    digitalWrite (ledRed, HIGH);
    digitalWrite (ledYellow, LOW);
    digitalWrite (ledGreen, LOW);
  }
  else if (doc["NewStateValue"]=="OK"){
    digitalWrite (ledRed, LOW);
    digitalWrite (ledYellow, LOW);
    digitalWrite (ledGreen, HIGH);
  }
  else if (doc["NewStateValue"]=="INSUFFICIENT_DATA"){
    digitalWrite (ledRed, LOW);
    digitalWrite (ledYellow, HIGH);
    digitalWrite (ledGreen, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initialisation");
  connectToWiFi();
  connectToAWS();
  pinMode (ledGreen, OUTPUT);
  pinMode (ledYellow, OUTPUT);
  pinMode (ledRed, OUTPUT);
  //Default mode
  digitalWrite (ledYellow, HIGH);
}

void loop() {
  Serial.println("Synchro...");
  mqttClient.loop();
  // Read frequency in ms
  delay(2000);
}
