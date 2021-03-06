/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.

const char* ssid = "2.4NET VIRTUA_62";
const char* password = "1049917000";
const char* mqtt_server = "37.187.106.16";
const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
String colorList;
int interval = 1000;
int colorIndex = 0, totalColors = 5;
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
void startGame() {
  colorList = "";
  colorIndex = 0;
  startRound();
}
void showColors(){
  int colorS = 0;
  for(int i=0;i<colorList.length();i++){
    colorS = colorList[i]-'0';
    delay(interval);
    ledColor(colorS);
    delay(interval);
    ledColor(0);
  }
  ledColor(0);
}
void startRound() {
  colorList = colorList+ random(1,totalColors);
  Serial.println(colorList);
  colorIndex = 0;
  char rounds[3];
  sprintf(rounds,"%d",colorList.length());
  client.publish("round",rounds);
  showColors();
}
void callback(char* topic, byte* payload, unsigned int length) {
  char toString[10] = "";
  for (int i = 0; i < length; i++) {
    toString[i] = (char)payload[i];
  }
  int num = atoi(toString);
  if (strcmp(topic, "cor") == 0) {
    int cor = colorList[colorIndex] - '0';
    ledColor(num);
    delay(500);
    ledColor(0);
    if (num == cor) {
      colorIndex++;
      if (colorIndex >= colorList.length())
        startRound();
    }
    else{
      startGame();
    }
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
  startGame();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
