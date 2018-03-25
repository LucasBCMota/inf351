String command;
bool blinking = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);                                      
  Serial.begin(115200);
}

void loop() {
  command = Serial.readString();

  if (command == "S")
    blinking = true;

  if (blinking) {
    digitalWrite(LED_BUILTIN, LOW);                                      
    delay(1000);                    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);                    
  }
}
