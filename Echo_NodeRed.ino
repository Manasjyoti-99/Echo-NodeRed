#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define TRIG_PIN D1
#define ECHO_PIN D2

const char* ssid = "vivo V27";         // Your WiFi SSID
const char* password = "12345678"; // Your WiFi password
const char* mqtt_server = "broker.hivemq.com"; // MQTT broker (example)
const char* topic = "echo/value";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculate the distance in cm
  float distance = duration * 0.034 / 2;
  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000); 

  String message = String(distance);
  client.publish(topic, message.c_str());
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(2000); // Publish every 2 seconds
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCUClient")) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}