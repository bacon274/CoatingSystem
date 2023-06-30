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

// LCD and Menu Classes // /// DO I NEED TESTA?
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
const byte pumpRelayPin = 50; 
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




/////////////// Declare Functions ///////////////
void menuItemFunction();
void runFunction();
void settingsFunction();
void backFunction();
void cleanFunction(); 
void speedFunction();
void setSpeedFunction();

/////////////////// Menu Setup ///////////////////
// Declare Menu Titles //
// NOTE: you cannot print unsigned char array, it is in a different format
unsigned char runCharsUnsigned[] = "Run";  
char runChars[] = "Run";
unsigned char cleanCharsUnsigned[] = "Clean";  
char cleanChars[] = "Clean";  
unsigned char settingsCharsUnsigned[] = "Settings";  
char settingsChars[] = "Settings";

// Declare Back Item:

unsigned char backCharsUnsigned[] = "Back";  
char backChars[] = "Back";
MenuItem backItem(AR_SIZE(backCharsUnsigned), backCharsUnsigned, backChars,&backFunction);

// Declare Blank Item: 
unsigned char blankCharsUnsigned[] = "";  
char blankChars[] = "";
MenuItem blankItem(AR_SIZE(blankCharsUnsigned), blankCharsUnsigned, blankChars,&menuItemFunction);


// Creating Menu Item objects //
MenuItem runItem(AR_SIZE(runCharsUnsigned), runCharsUnsigned, runChars,&runFunction);
MenuItem cleanItem(AR_SIZE(cleanCharsUnsigned), cleanCharsUnsigned, cleanChars,&cleanFunction);
MenuItem settingsItem = {AR_SIZE(settingsCharsUnsigned), settingsCharsUnsigned, settingsChars,&settingsFunction};

// Create List of Menu Items //
MenuItem itemList1[8] = {runItem,cleanItem,settingsItem,blankItem,blankItem,blankItem,blankItem,blankItem}; // Create list of menu item objects

// Declare Settings Menu Titles //
unsigned char conveyerCharsUnsigned[] = "Conveyer";  
char conveyerChars[] = "Conveyer";
unsigned char speedCharsUnsigned[] = "Belt Speed";  
char speedChars[] = "Belt Speed";
unsigned char flowCharsUnsigned[] = "Flow Rate"; 
char flowChars[] = "Flow Rate";

// Creating Setting Menu Item objects //
MenuItem conveyerItem(AR_SIZE(conveyerCharsUnsigned), conveyerCharsUnsigned, conveyerChars,&menuItemFunction);
MenuItem speedItem(AR_SIZE(speedCharsUnsigned), speedCharsUnsigned, speedChars,&speedFunction);
MenuItem flowItem(AR_SIZE(flowCharsUnsigned), flowCharsUnsigned, flowChars,&menuItemFunction);

// Create List of Settings Menu Items //
MenuItem itemListSettings[8] = {conveyerItem,speedItem,flowItem,backItem,blankItem,blankItem,blankItem,blankItem}; // Create list of menu item objects for RUN


// Declare Speed Setting Menu Titles //

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

// Creating Setting Menu Item objects in List //
MenuItem itemListSpeed[8] = {MenuItem(AR_SIZE(speed1Unsigned), speed1Unsigned, speed1Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed2Unsigned), speed2Unsigned, speed2Chars, &setSpeedFunction),
                             MenuItem(AR_SIZE(speed3Unsigned), speed3Unsigned, speed3Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed4Unsigned), speed4Unsigned, speed4Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed5Unsigned), speed5Unsigned, speed5Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed6Unsigned), speed6Unsigned, speed6Chars, &setSpeedFunction), 
                             MenuItem(AR_SIZE(speed7Unsigned), speed7Unsigned, speed7Chars, &setSpeedFunction), 
                             backItem};





// Create Menu Objects //
MenuClass speedMenu(8,itemListSpeed);            // Speed Settings
MenuClass settingsMenu(4,itemListSettings);      // Settings
MenuClass testMenu(3,itemList1);                 // Main Menu

 

// Current Menu Pointer //
MenuClass *menuCurrentPt = &testMenu; // Dynamic Memory Pointer for current MenuObject

////////////////////// Functions //////////////////////
// Rotary Encoder Functions //
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

// Menu Item Functions //
void menuItemFunction(){
  Serial.println("MENU FUNCTION HAPPENING");
}
void cleanFunction(){
  PORTA = B11001010; 
  Serial.println("ClEAN FUNCTION HAPPENING");
  cleanState = true;
  runState = false;
}
void runFunction(){
  PORTA = B00101000; // blank blank p2 s5 s4 s3 s2 s1
  Serial.println("RUN FUNCTION HAPPENING");
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
  EEPROM.write(speedMemoryLocation,PORTC);
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

////////////////////// SETUP //////////////////////

void setup() {
  ///////// Initialise Ports //////////
  // Read Speed Settings from Memory 
  byte speedSettingfromMemory = EEPROM.read(speedMemoryLocation);
  /// Initialising the relay pin ports
  DDRA = B11111111; 
  DDRC = B00000111;
  PORTA = B11111111;
  PORTC = speedSettingfromMemory;

  ///////// Initialise Serial //////////
  Serial.begin(9600); 
  delay(500);

  ///////// Initialise Screen + Rotary Encoder //////////
  LCDA1.initDriverPin(4,7,10);   // INIT SPI Interface //
  LCDA1.Initialise();            // INIT SCREEN //
  // INIT THE ROTARY INTERRUPTS //
  delay(500);
  if ( !Rotary.Begin() ) {
    Serial.println("unable to init rotate button");
    while (1);
  }
  // Initialise Rotary Callbacks ////
  Rotary.OnButtonClicked(OnButtonClicked);
  Rotary.OnButtonLeft(OnButtonLeft);
  Rotary.OnButtonRight(OnButtonRight);

  ///////// Initialise Output Pins  //////////
  // Pump buttons and relay Pin Setup //
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(pumpRelayLEDPin, OUTPUT);
  pinMode(pumpOnPin, INPUT_PULLUP); 
  pinMode(pumpOffPin, INPUT_PULLUP); 
  offPumpPinFunction();
  
  // Roller buttons and relay Pin Setup //
  pinMode(rollerRelayPin, OUTPUT);
  pinMode(rollerOnPin, INPUT_PULLUP); 
  pinMode(rollerOffPin, INPUT_PULLUP); 
  offRollerPinFunction();

  // Blower buttons and relay Pin Setup //
  pinMode(blowerRelayPin, OUTPUT);
  pinMode(blowerOnPin, INPUT_PULLUP); 
  pinMode(blowerOffPin, INPUT_PULLUP); 
  offBlowerPinFunction();
  
  ///////// Initialise Menu Display  //////////
  testMenu.displayMenuOptions(); 
}

void loop() {
  unsigned long currentMillis = millis();
  Rotary.Process( millis() );
  ///////// Button Logic Statements  //////////
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
  ///////// Period Cleaning Cycle  //////////
  if(cleanState == true && currentMillis - previousMillis >= interval){
    digitalWrite(s2RelayPin, !digitalRead(s2RelayPin));
    previousMillis = currentMillis;
  }
  
  delay(5);
}
