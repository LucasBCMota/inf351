bool blinking = false;
bool countdown = false;
float interval = 1000;
float currentTime;
float previousTime = 0;
int until_shutdown = 0;
int ledState = LOW;
String command;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    command = Serial.readString();
    if (command == "S") {
      blinking = true;
    }
    else if (command == "+") {
      interval /= 2;
    }
    else if (command == "-") {
      interval *= 2;
    }
    else if (command.startsWith("I")) {
      command = command.substring(2);
      interval = command.toFloat();
    }
    else if (command.startsWith("R")) {
      command = command.substring(2);
      countdown = true;
      until_shutdown = command.toInt() * 2;
    }
    command = "";
  }
  currentTime = millis();
  
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;
    if (countdown) {
      if (until_shutdown == 0) {
        countdown = false;
        blinking = false;      
      }
      until_shutdown--;
    }
    if (blinking) {
      tick();
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  
}

void tick() { 
  if (blinking) {
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(LED_BUILTIN, ledState);
  }
}

