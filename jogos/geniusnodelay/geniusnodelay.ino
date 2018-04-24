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

const char* ssid = "Sbornia...";
const char* password = "lauravadia";
const char* mqtt_server = "37.187.106.16";
const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
String colorList;
int interval = 1000;
int previousTime = 0;
int rounds = 0, colorIndex = 0, totalColors = 5;
bool playing = true, ledState = false;

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
  if (strcmp(topic, "reset") == 0) {
    if (playing)
      playing = false;
    else {
      playing = true;
    }
  }
  if (strcmp(topic, "cor") == 0) {
    int cor = colorList[colorIndex] - '0';
    if (num == cor) {
      ledColor(cor);
      delay(1000);
      ledColor(0);
      colorIndex++;
      if (colorIndex >= colorList.length())
        startNewRound();
    }
    else
      resetGame();
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
void startNewRound() {
  colorIndex = 0;
  rounds++;
  colorList = colorList+ random(totalColors-1);
  playing = true;
}
void resetGame() {
  colorIndex = 0;
  rounds = 0;
  playing = true;
  colorList = "";
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
void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  colorList = "" + random(1, 5);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - previousTime > interval && playing) {
    previousTime = now;
    if (ledState) {
      ledColor(colorList[colorIndex] - '0');
      ledState = false;
      colorIndex++;
    } else {
      ledState = true;
      ledColor(0);
    }
  }
}
