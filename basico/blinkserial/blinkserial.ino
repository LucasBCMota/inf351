String command;
String sub_command;
bool blinking = false;
bool ticking = false;
float frequency = 1000;
int ticks = 1;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);                                      
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    command = Serial.readString();
    if (command == "S") {
      blinking = true;
      ticking = false;
    }
    else if (command == "+")
      frequency /= 2;
    else if (command == "-")
      frequency *= 2;
    else if (command.startsWith("I")) {
      Serial.println(command);
      command = command.substring(2);
      frequency = command.toFloat();
    }
    else if (command.startsWith("R")) {
      Serial.println(command);
      command = command.substring(2);
      ticks = command.toInt() + 1;
      ticking = true;            
    }    
  }
  
  if (ticks > 0)
    ticks--;
  if (ticks == 0 && ticking) {
      blinking = false;
      ticking = false;
  }
  
  if (blinking) {
    digitalWrite(LED_BUILTIN, LOW);                                      
    delay(frequency);                    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);                    
  }
}
