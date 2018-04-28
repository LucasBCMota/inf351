#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


const char *ssid = "VAN PF #184";
const char *password = "policiafederal";
String nssid,npass;

ESP8266WebServer server(80);

void handleRoot() {
  String configPage = "";
  configPage += "<FORM action=\"/\" method=\"post\">"
  "<P>"
  "LED<br>"
  "<INPUT type=\"text\" name=\"SSID\" >ssid<BR>"
  "<INPUT type=\"password\" name=\"SENHA\">senha<BR>"
  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>";  
	server.send(200, "text/html", configPage);
}
void handleSubmit(){
  nssid = server.arg("SSID");
  npass = server.arg("SENHA");
}
void setup() {
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

void loop() {
	server.handleClient();
}
