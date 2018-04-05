bool blinking = false;
bool countdown = false;
float interval = 1000;
int until_shutdown = 0;
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
      until_shutdown = command.toInt();
    }
    command = "";
  }

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

void tick() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(interval);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(interval);
}

