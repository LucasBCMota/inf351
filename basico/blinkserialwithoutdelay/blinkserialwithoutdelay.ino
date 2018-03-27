String command = "";
bool blinking = false;
bool ticking = false;
unsigned long previous_millis = 0;
unsigned long current_millis;
int led_state = LOW;
int ticks = 0;
float interval = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);                                      
  Serial.begin(57600);
}

void loop() {
  current_millis = millis();
  if (current_millis - previous_millis >= interval) {
    previous_millis = current_millis;
    if (blinking) {
      if (led_state == LOW)
        led_state = HIGH;
      else
        led_state = LOW;
    digitalWrite(LED_BUILTIN, led_state);
    }          
    if (ticks > 0)
      ticks--;
    if (ticks == 0 && ticking) {
      blinking = false;
      ticking = false;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  
  read_command();
  if (command == "S")
    blinking = true;
  else if (command == "+")
    interval /= 2;
  else if (command == "-")
    interval *= 2;
  else if (command.startsWith("I")) {
    command = command.substring(2);
    interval = command.toFloat();
  }  
  else if (command.startsWith("R")) {
      command = command.substring(2);
      ticks = 2 * (command.toInt());
      ticking = true;            
  }    
}

void read_command() {
  command = "";
  if (Serial.available()) {
    command = Serial.readString();
  }  
}

