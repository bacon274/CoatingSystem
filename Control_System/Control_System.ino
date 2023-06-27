#include <ActuatorClass.h>
#include <MenuClass.h>
#include <MenuItem.h>
#include <KY040rotary.h>
#include <EEPROM.h>
#include "LCD12864RSPI.h"
#include "DFrobot_bmp.h"
#include "DFrobot_char.h"

#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )

/////////////// Global Variables ///////////////
// EEPROM location //
int speedMemoryLocation = 0;

// LCD and Menu Classes //
ActuatorClass testA(13); 
LCD12864RSPI LCDA1 = LCD12864RSPI();

// ROTARY ENCODER //
KY040 Rotary(9,8,11);
int counter = 0;

// IO Interrupt Pins and Individual Relays

// Pump //
const byte pumpOnPin = 2;
const byte pumpOffPin = 3;
volatile byte pumpState = LOW;
const byte pumpRelayPin = 50; // need to check
const byte pumpRelayLEDPin = 51;

// Rollers //
const byte rollerOnPin = 20;
const byte rollerOffPin = 21;
volatile byte rollerState = HIGH;
const byte rollerRelayPin = 53; 

// Blower //
const byte blowerOnPin = 18;
const byte blowerOffPin = 19;
volatile byte blowerState = HIGH;
const byte blowerRelayPin = 52; 

// Solenoid Relay Pins //
const byte runRelayPin = 23; 
const byte cleanRelayPin = 24;

// Cleaning Timer Variables //
unsigned long previousMillis = 0;  
const long interval = 1000; 
volatile byte cleanState = false;
volatile byte runState = false;
const byte s2RelayPin = 23;



// Declare Functions:
void menuItemFunction();
void runFunction();
void settingsFunction();
void backFunction();
void cleanFunction(); 
void speedFunction();
void setSpeedFunction();

/// Menu Items Text (will put in the corresponding functions for each menu)
unsigned char testCharsUnsigned[] = "TEST123";  // NOTE: you cannot print unsigned char array, it is in a different format
char testChars[] = "TEST123";
unsigned char runCharsUnsigned[] = "Run";  
char runChars[] = "Run";
unsigned char cleanCharsUnsigned[] = "Clean";  
char cleanChars[] = "Clean";  
unsigned char settingsCharsUnsigned[] = "Settings";  
char settingsChars[] = "Settings";



unsigned char testCharsUnsigned2[] = "2TEST123";  // NOTE: you cannot print unsigned char array, it is in a different format
char testChars2[] = "2TEST123";
unsigned char runCharsUnsigned2[] = "2Run";  
char runChars2[] = "2Run";
unsigned char cleanCharsUnsigned2[] = "2Clean";  
char cleanChars2[] = "2Clean";  
unsigned char settingsCharsUnsigned2[] = "2Settings";  
char settingsChars2[] = "2Settings";

/// Back Item:

unsigned char backCharsUnsigned[] = "Back";  
char backChars[] = "Back";
MenuItem backItem(AR_SIZE(backCharsUnsigned), backCharsUnsigned, backChars,&backFunction);

// Blank Item: 
unsigned char blankCharsUnsigned[] = "";  
char blankChars[] = "";
MenuItem blankItem(AR_SIZE(blankCharsUnsigned), blankCharsUnsigned, blankChars,&menuItemFunction);


// Creating Menu Item objects for each item
MenuItem runItem(AR_SIZE(runCharsUnsigned), runCharsUnsigned, runChars,&runFunction);
MenuItem cleanItem(AR_SIZE(cleanCharsUnsigned), cleanCharsUnsigned, cleanChars,&cleanFunction);
MenuItem settingsItem = {AR_SIZE(settingsCharsUnsigned), settingsCharsUnsigned, settingsChars,&settingsFunction};
MenuItem testItem(AR_SIZE(testCharsUnsigned), testCharsUnsigned, testChars,&menuItemFunction);

MenuItem runItem2(AR_SIZE(runCharsUnsigned2), runCharsUnsigned2, runChars2,&menuItemFunction);
MenuItem cleanItem2(AR_SIZE(cleanCharsUnsigned2), cleanCharsUnsigned2, cleanChars2,&menuItemFunction);
MenuItem settingsItem2 = {AR_SIZE(settingsCharsUnsigned2), settingsCharsUnsigned2, settingsChars2,&menuItemFunction};
MenuItem testItem2(AR_SIZE(testCharsUnsigned2), testCharsUnsigned2, testChars2,&menuItemFunction);


MenuItem itemList1[8] = {runItem,cleanItem,settingsItem,blankItem,blankItem,blankItem,blankItem,blankItem}; // Create list of menu item objects
//MenuItem itemList2[4] = {testItem2, runItem2,settingsItem2,backItem}; 



/// Settings Items

unsigned char conveyerCharsUnsigned[] = "Conveyer";  
char conveyerChars[] = "Conveyer";
unsigned char speedCharsUnsigned[] = "Belt Speed";  
char speedChars[] = "Belt Speed";
unsigned char flowCharsUnsigned[] = "Flow Rate"; 
char flowChars[] = "Flow Rate";

MenuItem conveyerItem(AR_SIZE(conveyerCharsUnsigned), conveyerCharsUnsigned, conveyerChars,&menuItemFunction);
MenuItem speedItem(AR_SIZE(speedCharsUnsigned), speedCharsUnsigned, speedChars,&speedFunction);
MenuItem flowItem(AR_SIZE(flowCharsUnsigned), flowCharsUnsigned, flowChars,&menuItemFunction);

MenuItem itemListSettings[8] = {conveyerItem,speedItem,flowItem,backItem,blankItem,blankItem,blankItem,blankItem}; // Create list of menu item objects for RUN

// Speed Items: 
//
unsigned char speed1Unsigned[] = "Speed 1";  
char speed1Chars[] =  "Speed 1";  
unsigned char speed2Unsigned[] = "Speed 2";  
char speed2Chars[] =  "Speed 2";  
unsigned char speed3Unsigned[] = "Speed 3";  
char speed3Chars[] =  "Speed 3";  
unsigned char speed4Unsigned[] = "Speed 4";  
char speed4Chars[] =  "Speed 4";  
unsigned char speed5Unsigned[] = "Speed 5";  
char speed5Chars[] =  "Speed 5";  
unsigned char speed6Unsigned[] = "Speed 6";  
char speed6Chars[] =  "Speed 6";  
unsigned char speed7Unsigned[] = "Speed 7";  
char speed7Chars[] =  "Speed 7";  
unsigned char speed8Unsigned[] = "Speed 8";  
char speed8Chars[] =  "Speed 8";  

MenuItem itemListSpeed[8] = {MenuItem(AR_SIZE(speed1Unsigned), speed1Unsigned, speed1Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed2Unsigned), speed2Unsigned, speed2Chars, &setSpeedFunction),
                             MenuItem(AR_SIZE(speed3Unsigned), speed3Unsigned, speed3Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed4Unsigned), speed4Unsigned, speed4Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed5Unsigned), speed5Unsigned, speed5Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed6Unsigned), speed6Unsigned, speed6Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed7Unsigned), speed7Unsigned, speed7Chars, &setSpeedFunction), 
                             backItem};





///// Create MenuObjects
MenuClass speedMenu(8,itemListSpeed);
MenuClass settingsMenu(4,itemListSettings);
MenuClass testMenu(3,itemList1);                              // new constructor menu initialised
//MenuClass testMenu2(AR_SIZE(itemList2),itemList2); 
//MenuClass testMenu(runItem);                              // create menu initialised with a random constructor
//MenuClass testMenu2(cleanItem);
 

//MenuClass testMenu(MenuItems); // The constructor should be in this formt or gfet rid of

//MenuClass  menuCurrent = testMenu; // Current Menu 
MenuClass *menuCurrentPt = &testMenu; // Dynamic Memory Pointer for current MenuObject
//MenuClass *menuStack[10] = {menuCurrentPt}; // array of pointers

void OnButtonClicked(void) {
  Serial.println("Button 1: clicked");
  Serial.println();
  int currentIndex = menuCurrentPt->index;
  Serial.println(menuCurrentPt->listofMenuItems[currentIndex].signedTitle);
  menuCurrentPt->listofMenuItems[currentIndex].itemFunction();
}
void OnButtonLeft(void) {
  Serial.println("Button 1: rotating left");
 
  bool increment = false;
  menuCurrentPt->updateIndex(increment);
  Serial.println(menuCurrentPt->index);
}
void OnButtonRight(void) {
  Serial.println("Button 1: rotating right");
  bool increment = true;
  menuCurrentPt->updateIndex(increment);
  Serial.println(menuCurrentPt->index);
}

void setMenu(MenuClass currentItem){
  // this function takes a menuObject and sets the menucurrent to that value
  // adds to the stack of menu items 
  // calls the display menu function of the menu Object
}
void test1(MenuItem menuItems[], int num){
  Serial.println(num);
  for (int i =0; i<6; i++){
    Serial.print(i);
    Serial.print(" ");
    Serial.println(menuItems[i].signedTitle);
  }
}

void menuItemFunction(){
  Serial.println("MENU FUNCTION HAPPENING");
  
  
}
void cleanFunction(){
  Serial.println("ClEAN FUNCTION HAPPENING");
//  digitalWrite(cleanRelayPin, LOW); 
//  digitalWrite(runRelayPin, HIGH); 
  PORTA = B11001010; 
  cleanState = true;
  runState = false;
}
void runFunction(){
  Serial.println("RUN FUNCTION HAPPENING");
  PORTA = B00110101; 
  cleanState = false;
  runState = true;
}

void backFunction(){
  Serial.println("GOING BACK");
//  Serial.println((menuCurrentPt->parentMenu->listofMenuItems[0].signedTitle));
  menuCurrentPt->parentMenu->displayMenuOptions();
  menuCurrentPt =  menuCurrentPt->parentMenu;
  
}

void settingsFunction(){
  Serial.println("Settings Menu");
  settingsMenu.setParentMenu(menuCurrentPt);
  Serial.println(settingsMenu.parentMenu->listofMenuItems[0].signedTitle);
  menuCurrentPt = &settingsMenu;
//  menuStack
  settingsMenu.displayMenuOptions();
  
}

void speedFunction(){
  Serial.println("Speed selection Menu");
  speedMenu.setParentMenu(menuCurrentPt);
  menuCurrentPt = &speedMenu;
  speedMenu.displayMenuOptions();

  
}

void setSpeedFunction() {
  Serial.println("Setting Speed");
  int speedIndex = menuCurrentPt->index;
  char *speedChar[] = {(menuCurrentPt->listofMenuItems[speedIndex].signedTitle)};
  char * pch; 
  pch = strtok(*speedChar," "); // Extracting the numerical part of the speed
  pch = strtok(NULL," ");
  int speedValue = *pch-'0';
//  Serial.println(pch);
//  Serial.println(speedValue);
  
  switch (speedValue){
    case 1:
      PORTC = B00000110;
      break;
    case 2:
      PORTC = B00000101;
      break;
    case 3:
      PORTC = B00000100;
      break;
    case 4:
      PORTC = B00000011;
      break;
    case 5:
      PORTC = B00000010;
      break;
    case 6:
      PORTC = B00000001;
      break;
    case 7:
      PORTC = B00000000;
      break;
  }
  Serial.println(PORTC);
  EEPROM.write(speedMemoryLocation,PORTC);

  
  //PORTC = B00000000;
}


void onPumpPinFunction(){
  digitalWrite(pumpRelayPin, HIGH); 
  digitalWrite(pumpRelayLEDPin, LOW); 
  
  pumpState = HIGH;
  Serial.print(pumpState);
}
void offPumpPinFunction(){
  digitalWrite(pumpRelayPin, LOW); 
  digitalWrite(pumpRelayLEDPin, HIGH);
  pumpState = LOW;
  Serial.print(pumpState);
}

void onRollerPinFunction(){
  rollerState = LOW;
  digitalWrite(rollerRelayPin, LOW); 
}
void offRollerPinFunction(){
  rollerState = HIGH;
  digitalWrite(rollerRelayPin, HIGH); 
}

void onBlowerPinFunction(){
  digitalWrite(blowerRelayPin, LOW); 
}
void offBlowerPinFunction(){
  digitalWrite(blowerRelayPin, HIGH); 
}


//void createSpeedItems(){
//  for(int j=1; j<9; j++){
//    int integer = j; 
//    char speedString[] = "Speed "; 
//    char integerString[32];
//    
//    sprintf(integerString, "%d", integer);
//    strcat(speedString, integerString); 
//    Serial.println(speedString);
//    char tempString[] = {*speedString}; 
//    unsigned char speedUnsigned[] = {(unsigned char)*tempString};
//    Serial.println(speedString);/// Something strange with *string giving this on the end ⸮H
//    speedItemList[j-1] = MenuItem(AR_SIZE(speedString), speedUnsigned, speedString, &setSpeedFunction);
//  }
// //MenuClass speedMenu(8,speedItemList);
//// speedMenu.displayMenuOptions();
//
//}


void setup() {
   // Creating a Menu It  em List with the above
//  MenuItem itemList1[3] = {runItem,cleanItem,settingsItem}; // Create list of menu item objects
//  MenuItem itemList2[3] = {testItem2, runItem2,settingsItem2}; 
//  pinMode(22, OUTPUT); 
  byte speedSettingfromMemory = EEPROM.read(speedMemoryLocation);
  /// Setting up the relay pins 
  DDRA = B11111111; 
  DDRC = B00000111;
  PORTA = B11111111;
  PORTC = speedSettingfromMemory;

  
  
//  digitalWrite(22, HIGH);
//  delay(500);
//  digitalWrite(22, LOW);
  Serial.begin(9600); 
  delay(500);
  LCDA1.initDriverPin(4,7,10);   //INIT SPI Interface
  LCDA1.Initialise();            // INIT SCREEN
  delay(500);
//  Serial.println(speedSettingfromMemory);


//  unsigned char settingUpCchar[] = "Setting Up";
//  LCDA1.DisplayString(0,0,settingUpCchar, 10);
  
//  testMenu2.addMenuItemList(AR_SIZE(itemList2),itemList2); //// For some reason the item list is a global variable that gets reassiged when declared in the setup loop seems to stop the issue
//
//  testMenu.addMenuItemList(AR_SIZE(itemList1),itemList1); // add menu item array to menu object 


//  speedMenu.displayMenuOptions();
//  testMenu.displayMenuOptions(); /// UNCOMMENT to initialise menu

  
//  test1(itemList1,1);
//  test1(itemList2,2);
//  Serial.println("from object");
//   for (int i =0; i<3; i++){
//    Serial.print(i);
//    Serial.print(" ");
//    Serial.println(testMenu.listofMenuItems[i].signedTitle);
//  }
//  Serial.println(testMenu.itemListLength);

  
//  delay(500);
//  testMenu2.displayMenuOptions();
//  delay(500);

  //// INIT THE ROTARY INTERRUPTS
   if ( !Rotary.Begin() ) {
    Serial.println("unable to init rotate button");
    while (1);
  }

  // init callbacks ////
  Rotary.OnButtonClicked(OnButtonClicked);
  Rotary.OnButtonLeft(OnButtonLeft);
  Rotary.OnButtonRight(OnButtonRight);

  // set up interrupt pins 
  // Example
//  pinMode(relayPin1, OUTPUT);
//  pinMode(interruptPin1, INPUT_PULLUP); //INPUT_PULLUP
//  pinMode(interruptPin2, INPUT_PULLUP); //INPUT_PULLUP
//  attachInterrupt(digitalPinToInterrupt(interruptPin1), onPinFunction, RISING);
//  attachInterrupt(digitalPinToInterrupt(interruptPin2), offPinFunction, RISING);

  // Pump buttons and relay Pin Setup
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(pumpRelayLEDPin, OUTPUT);
  pinMode(pumpOnPin, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(pumpOffPin, INPUT_PULLUP); //INPUT_PULLUP
//  attachInterrupt(digitalPinToInterrupt(pumpOnPin), onPumpPinFunction, LOW);
//  attachInterrupt(digitalPinToInterrupt(pumpOffPin), offPumpPinFunction, LOW);
  offPumpPinFunction();
  
  // Roller buttons and relay Pin Setup
  pinMode(rollerRelayPin, OUTPUT);
  pinMode(rollerOnPin, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(rollerOffPin, INPUT_PULLUP); //INPUT_PULLUP
//  attachInterrupt(digitalPinToInterrupt(rollerOnPin), onRollerPinFunction, LOW);
//  attachInterrupt(digitalPinToInterrupt(rollerOffPin), offRollerPinFunction, LOW);
  offRollerPinFunction();

  // Blower buttons and relay Pin Setup
  pinMode(blowerRelayPin, OUTPUT);
  pinMode(blowerOnPin, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(blowerOffPin, INPUT_PULLUP); //INPUT_PULLUP
//  attachInterrupt(digitalPinToInterrupt(blowerOnPin), onBlowerPinFunction, LOW);
//  attachInterrupt(digitalPinToInterrupt(blowerOffPin), offBlowerPinFunction, LOW);
  offBlowerPinFunction();
  
  
  
  
  
  //Serial.println("KY-040 rotary encoder OK");
  //Serial.println(testMenu2.listofMenuItems[3].signedTitle);
//   test(itemList1,1);
//   test(itemList2,2);4
  testMenu.displayMenuOptions(); /// UNCOMMENT to initialise menu
  
}

void loop() {
  unsigned long currentMillis = millis();
  Rotary.Process( millis() );
  if(digitalRead(pumpOnPin) == LOW){
    onPumpPinFunction();  
  }else if(digitalRead(pumpOffPin) == LOW){
    offPumpPinFunction();
  };
  if(digitalRead(rollerOnPin) == LOW){
    onRollerPinFunction();
  }else if(digitalRead(rollerOffPin) == LOW){
    offRollerPinFunction();
  }
  if(digitalRead(blowerOnPin) == LOW){
    onBlowerPinFunction();
  }else if(digitalRead(blowerOffPin) == LOW){
    offBlowerPinFunction();
  }
  if(cleanState == true && currentMillis - previousMillis >= interval){
    digitalWrite(s2RelayPin, !digitalRead(s2RelayPin));
    previousMillis = currentMillis;
  }
  

  delay(5);
//  Serial.print(pumpState);
//  Serial.print(",");
//  Serial.print(digitalRead(pumpOnPin));
//  Serial.print(",");
//  Serial.println(digitalRead(pumpOffPin));
//  Serial.println(digitalRead(rollerOnPin));
//  Serial.println(digitalRead(rollerOffPin));

  // digitalWrite(relayPin1, state);
//   Serial.println(state);
}
