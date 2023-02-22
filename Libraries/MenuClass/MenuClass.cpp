    /// Source File

#include "Arduino.h"
#include "MenuClass.h"
#include <MenuItem.h> /// need to include menu item class in <>
#include "LCD12864RSPI.h"
#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] ) // this doesnt work with pointers!

extern LCD12864RSPI LCDA1; // I am able to pass the LCD object to the menu class



MenuClass::MenuClass(int len, MenuItem (&inputListofMenuItems)[8]) : listofMenuItems(inputListofMenuItems){    /// Need to put the constructor list initialiser in
   itemListLength = len;

}

// MenuClass::MenuClass(MenuItem& itemInput) : item(itemInput){    /// Need to put the constructor list initialiser in
//    //item = itemInput;

// }

void MenuClass::addMenuItemList(int inputListLength, MenuItem itemListInput[]){
    itemListLength = inputListLength;
    for (int i =0; i<itemListLength-1; i++){
         listofMenuItems[i] = *itemListInput;
    }
      //// For some reason this seems to happen automatically, I really can't see how. 
}

// void MenuClass::displayItem(){
//     LCDA1.DisplayString(0,0,item.unsignedTitle,item.titleLength);
// }

void MenuClass::updateIndex(bool increment){
    
    if(increment == false && index>0){
        index--;
    }else if (increment == true && index<itemListLength-1){
        index ++;
    };
    displayMenuOptions();
}

void MenuClass::displayMenuOptions(){
    LCDA1.CLEAR();
    int maxRows = 4; 
    int maxIndex;
    int cursor = 0;
    int i; 
    int len = itemListLength;
    unsigned char arrow[] = ">";
    unsigned char backarrow[] = "<";
    unsigned char back[] = "Back";
    char backChar[] = "Back";
   // If blank here, then reduce item list?

    /// Scrolling feature
    if (index <maxRows){
        i = 0; 
        maxIndex = maxRows;
         //if (maxIndex >itemListLength-1){ maxIndex = itemListLength+1;} // I thought that if max rows > itemnumber then error but no
    }else{ 
        i = index-maxRows+1;
        maxIndex = i+ maxRows;
       
        }
    
    for (i; i<(maxIndex); i++){
        LCDA1.DisplayString(cursor,0,listofMenuItems[i].unsignedTitle,listofMenuItems[i].titleLength);
        if(i==index){
            if ((listofMenuItems[i].signedTitle)==backChar){ LCDA1.DisplayString(cursor,7,backarrow,1);}
            else{ LCDA1.DisplayString(cursor,7,arrow,1);}
           
        }
        cursor++;
    }
    
    // if(listofMenuItems[index].signedTitle=="Back"){LCDA1.DisplayString(3,7,backarrow,1);};
    

}

void MenuClass::setParentMenu(MenuClass* inputParentMenu ){
    parentMenu = inputParentMenu;

}