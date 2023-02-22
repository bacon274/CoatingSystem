/// Class Header File
#ifndef ActuatorClass_h
#define ActuatorClass_h
#include "Arduino.h" 
// Code: 
class ActuatorClass {
  public: 
    ActuatorClass(int pin); // Constructor
    bool state;
    void actuatorONOFF();
    void toggle();
  private:
    int _pin;
    bool _state;
    
    
};

#endif
