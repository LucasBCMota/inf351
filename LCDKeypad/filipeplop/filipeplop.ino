/*
Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
library works with all LCD displays that are compatible with the Hitachi HD44780 driver.

This sketch is used for testing the lcd.createChar function.

TITLE: Thumbs Up

Demo by JO3RI

www.JO3RI.be/arduino
*/

// include the library code:
#include  <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins:
LiquidCrystal lcd(8,9,4,5,6,7); 

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // clear the LCD screen:
  lcd.clear();
}

byte FAA[8] = {
  0b11111,
  0b11111,
  0b11000,
  0b11000,
  0b11000,
  0b11111,
  0b11111,
  0b11000
};
byte FBA[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte FAB[8] = {
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b11100,
  0b11100,
  0b00000
};
byte blank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
byte IA[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001
};
byte IB[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};
byte LAA[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte LBA[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11111,
  0b11111
};
byte LBB[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};
byte PAB[8] = {
  0b11110,
  0b11111,
  0b00011,
  0b00011,
  0b00011,
  0b11110,
  0b11100,
  0b00000
};
byte EAA[8] = {
  0b11111,
  0b11111,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11111
};
byte EBA[8] = {
  0b11111,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11111,
  0b11111
};
byte EAB[8] = {
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11100
};
byte EBB[8] = {
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};
byte OAA[8] = {
  0b01111,
  0b11111,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte OBA[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11111,
  0b01111
};
byte OAB[8] = {
  0b11110,
  0b11111,
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b00011
};
byte OBB[8] = {
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b11111,
  0b11110
};
void escreveLetra(byte AA[8],byte AB[8],byte BA[8],byte BB[8],int x, int y){
  lcd.createChar(0, AA);
  lcd.createChar(1, BA);
  lcd.createChar(2, AB);
  lcd.createChar(3, BB);
  lcd.setCursor(x,y);
  lcd.write((byte)0);
  lcd.setCursor(x,y+1);
  lcd.write((byte)1);
  lcd.setCursor(x+1,y);
  lcd.write((byte)2);
  lcd.setCursor(x+1,y+1);
  lcd.write((byte)3);
}
int interval = 500;
void loop(){
  escreveLetra(FAA,FAB,FBA,blank,0,0);
  delay(interval);
  lcd.clear();
  escreveLetra(IA,IB,IA,IB,2,0);
  delay(interval);
  lcd.clear();
  escreveLetra(LAA,blank,LBA,LBB,4,0);
  delay(interval);
  lcd.clear();
  escreveLetra(IA,IB,IA,IB,6,0);
  delay(interval);
  lcd.clear();
  escreveLetra(FAA,PAB,FBA,blank,8,0);
  delay(interval);
  lcd.clear();
  escreveLetra(EAA,EAB,EBA,EBB,10,0);
  delay(interval);
  lcd.clear();
  escreveLetra(FAA,FAB,FBA,blank,12,0);
  delay(interval);
  lcd.clear();
  escreveLetra(LAA,blank,LBA,LBB,14,0);
  delay(interval);
  lcd.clear();
  escreveLetra(OAA,OAB,OBA,OBB,0,0);
  delay(interval);
  lcd.clear();
  escreveLetra(FAA,PAB,FBA,blank,2,0);
  delay(interval);
  lcd.clear();
}
