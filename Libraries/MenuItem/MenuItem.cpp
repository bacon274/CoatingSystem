#include "Arduino.h"
#include "MenuItem.h"

MenuItem::MenuItem(int newtitleLength,unsigned char* newunsignedTitle,char* newsignedTitle, void (*newitemFunction)()){
    titleLength = newtitleLength;
    unsignedTitle = newunsignedTitle; 
    signedTitle = newsignedTitle;
    itemFunction = newitemFunction; 

}