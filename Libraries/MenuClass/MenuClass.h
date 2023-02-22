/// Class Header File
#ifndef MenuClass_h
#define MenuClass_h
#include "Arduino.h" 
//#include "MenuItem.h"
#include <MenuItem.h>

// Code: 
class MenuClass {
  public: 
    // Memmber variables 
    int itemListLength;
    int index = 0; 
    // MenuItem& item; 
   // MenuItem itemList[];
    MenuItem listofMenuItems[8];
    MenuClass* parentMenu;

    // Member functions
    MenuClass(int len, MenuItem (&inputListofMenuItems)[8]);// Constructor array new
    // MenuClass(MenuItem& itemInput) ;// Constructor  single
    void addMenuItemList(int inputListLength, MenuItem itemListInput[]);
    void displayMenuOptions();
    // void displayItem();
    void updateIndex(bool increment);
    void setParentMenu(MenuClass* inputParentMenu);
    
  private:
   
    

    
    
};

#endif
