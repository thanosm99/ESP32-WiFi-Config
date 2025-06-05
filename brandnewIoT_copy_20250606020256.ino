#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

  const char* ssid = "Vodafone-E48220612";
  const char* password = "T6945373395m.";

  const char* mqtt_server = "broker.hivemq.com";
  const int mqtt_port = 8883;

  WiFiClientSecure espClient; 
  PubSubClient client(espClient);

  const char* mqtt_topic = "/ene262017077/iotcontrol";

  const int relayPin = 5;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  if (message.equalsIgnoreCase("on")) {
    digitalWrite(relayPin, HIGH); 
    Serial.println("Relay ON");
  } else if (message.equalsIgnoreCase("off")) {
    digitalWrite(relayPin, LOW); 
    Serial.println("Relay OFF");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("/ene262017077/iotcontrol");
      client.publish(mqtt_topic, "Hello from ESP32");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);  
  digitalWrite(relayPin, LOW); 
  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}