#include "HC595.h"
#include "Arduino.h"

HC595::HC595(int ser, int srClk, int rClk, int srClr){
  this->ser = ser;
  this->srClk = srClk;
  this->rClk = rClk;
  this->srClr = srClr; 

  pinMode(ser, OUTPUT);
  pinMode(srClk, OUTPUT);
  pinMode(rClk, OUTPUT);
  pinMode(srClr, OUTPUT);

}


void HC595::reset(){
  digitalWrite(srClr, LOW);
  //delay(1);
  digitalWrite(srClr, HIGH);
}
//push the right most bit to the shift register
void HC595::pushOne(int data) {
  digitalWrite(srClr, HIGH);
  digitalWrite(ser, (data & 1) ? HIGH: LOW);//write in the data
  digitalWrite(srClk, LOW);
  digitalWrite(srClk, HIGH);//rising edge
}

void HC595::latch() {
  digitalWrite(rClk, LOW);
  digitalWrite(rClk, HIGH);//rising edge
}

void HC595::pushn(unsigned long data, int nbits){
  for(int i=0; i<nbits; i++){
    pushOne((data >> i) & 1);
  }
}
