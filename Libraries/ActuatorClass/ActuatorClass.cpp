/// Source File

#include "Arduino.h"
#include "ActuatorClass.h"

ActuatorClass::ActuatorClass(int pin){
  pinMode(pin, OUTPUT); 
  _pin = pin; 
  _state = state;

}

void ActuatorClass::actuatorONOFF(){
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(1000);
}


void ActuatorClass::toggle(){
  state = !state;

}