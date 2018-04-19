// Robo India Tutorial 
// Digital Input and Output on LED 
// Hardware: NodeMCU

const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;
int verm;
int verd;
int azul;
int leu = -1;
void setup() {
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  Serial.println(leu);
  if(Serial.available() && leu < 0)
    leu = Serial.parseInt();
  if(leu > 0 && Serial.available()){
    if(leu == 1){
      verm = Serial.parseInt();
      Serial.println(verm);
    }
    if(leu == 2)
      verd = Serial.parseInt();
    if(leu == 3)
      azul = Serial.parseInt();
    leu = -1;
  }
  delay(200);
  Serial.println("RGB");
  Serial.print(verm);
  Serial.print(" ");
  Serial.print(verd);
  Serial.print(" ");
  Serial.print(azul);
  Serial.print('\n');
  analogWrite(RED, verm);
  analogWrite(GREEN, verd);
  analogWrite(BLUE, azul); 
}

