//struct MenuItem {int titleLength; unsigned char* unsignedTitle; char* signedTitle; }; 
/// Class Header File
#ifndef MenuItem_h
#define MenuItem_h
#include "Arduino.h" 

// Code: 
class MenuItem {
  public: 
  // Intstance Members
  int titleLength;
  unsigned char* unsignedTitle;
  char* signedTitle;
  void (*itemFunction)();
  MenuItem( int newtitleLength,unsigned char* newunsignedTitle,char* newsignedTitle, void (*newitemFunction)());


};

#endif
