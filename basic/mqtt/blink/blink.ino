
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

// Update these with values suitable for your network.

const char* ssid = "Sbornia...";
const char* password = "lauravadia";
const char* mqtt_server = "37.187.106.16";

WiFiClient espClient;
PubSubClient client(espClient);
long previousTime = 0;
char msg[50];
int value = 0;
int interval = 1000, ledState = HIGH, count = -1;
bool blinking = true;

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
  if(strcmp(topic,"freq")==0){
    if (num == -1) 
      interval = interval - 100;
    else if (num == -2)
      interval = interval + 100;
    else if (num > 0)
      interval = num;
  }
  if(strcmp(topic,"blink")==0){
    if (num == 1)
      blinking = true;
    else
      blinking = false;
  }
  if(strcmp(topic,"count")==0){
    count = num+1;
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
      client.subscribe("freq");
      client.subscribe("blink");
      client.subscribe("count");
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
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(count == 0){
    blinking = false;
    count = -1;
  }
  long now = millis();
  if (now - previousTime > interval && blinking) {
    if(count > 0 && ledState == LOW)
    count--;
    previousTime = now;
    digitalWrite(BUILTIN_LED, ledState);
    if(ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    //client.publish("outTopic", msg);
  }
  else if(!blinking)
    digitalWrite(BUILTIN_LED, HIGH);
}
