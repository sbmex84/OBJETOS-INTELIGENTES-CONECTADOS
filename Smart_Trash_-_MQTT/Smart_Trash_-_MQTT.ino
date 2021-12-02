#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

//Autores: Sergio & Erick
//Smart Trash - Universidade Presbiteriana Mackenzie
//MQTT BROKER: broker.mqtt - MQTT BOX 
//MQTT ID: 53f37a90-e8f5-4605-beda-81adf8c7aa59 

// Bibliotecas: ESP8266 v13 + PubSubClient v15 + Servo

const char* ssid = "iPhone de Gabriel";//Nome da rede wifi 
const char* password = "1234554321";//Senha da rede
const char* mqtt_server = "broker.mqttdashboard.com"; //Servidor MQTT

Servo myservo;  
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
  
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  Serial.print(" Rotation angle is:");
  Serial.print("   ");
  for(int i=0;i<length;i++)
  {
    if((int)payload[i]>194||(int)payload[i]<0)
    break;
    myservo.write((int)payload[i]);
    Serial.print((int)payload[i]);
    Serial.print("]");
    Serial.println();
  }
}

void reconnect() {
  
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
   
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
  
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(D1);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
