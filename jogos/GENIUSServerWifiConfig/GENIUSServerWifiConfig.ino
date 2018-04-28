#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

const char *ssid = "VAN PF #184";
const char *password = "policiafederal";
String nssid,npass,mqtt_server;
const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;
char msg[50];
String colorList;
int interval = 1000;
int colorIndex = 0, totalColors = 5;
bool mqtt = false;

ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);


String configPage ="<FORM action=\"/\" method=\"post\">"
"<P>"
"GENIUS<br>"
"<INPUT type=\"text\" name=\"SSID\" >ssid<BR>"
"<INPUT type=\"password\" name=\"SENHA\">senha<BR>"
"<INPUT type=\"text\" name=\"MQTTSERVER\" >ip mqtt<BR>"
"<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
"</P>"
"</FORM>";
void handleRoot()
{
  if (server.hasArg("SSID")) {
    Serial.println("TEM");
    handleSubmit();
  }
  else {
    Serial.println("NAOTEM");
    server.send(200, "text/html", configPage);
  }
}
void handleSubmit(){
  nssid = server.arg("SSID");
  npass = server.arg("SENHA");
  mqtt_server = server.arg("MQTTSERVER");
  setupWifiMode();
}
void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configurando...");
	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("Configuração de conexão em: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
	server.begin();
	Serial.println("HTTP server started");
}
void setupWifiMode(){
  server.close();
  WiFi.disconnect();
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(nssid);
  WiFi.begin(nssid.c_str(),npass.c_str());
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server.c_str(), 1883);
  client.setCallback(callback);
  mqtt = true;
  startGame();
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
void loop() {
  if(!mqtt)
	  server.handleClient();
  else{
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
}
