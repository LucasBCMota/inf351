// Robo India Tutorial 
// Digital Input and Output on LED 
// Hardware: NodeMCU
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "2.4NET VIRTUA_62";
const char* password = "1049917000";
const char* mqtt_server = "37.187.106.16";
const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char toString[10] = "";
  for (int i = 0; i < length; i++) {
    toString[i] = (char)payload[i];
  }
  int num = atoi(toString);
  if (strcmp(topic, "cor") == 0) {
    ledColor(num);
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("reset");
      client.subscribe("cor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void ledColor(int color) {
  if (color == 0) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  } else if (color == 1) {
    analogWrite(RED, 200);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  } else if (color == 2) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 200);
    analogWrite(BLUE, 0);
  } else if (color == 3) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 200);
  } else if (color == 4) {
    analogWrite(RED, 200);
    analogWrite(GREEN, 25);
    analogWrite(BLUE, 0);
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

