#include "U8glib.h"
//Use Gray scale if not using lines
U8GLIB_NHD31OLED_GR u8g(4, 5, 6, 3);
char inData[20]; // Allocate some space for the string
String summaryStr="";
String locationStr="";
String timeStr="";
char inChar= '!'; // Where to store the character read
byte number = 0;
byte index = 0; // Index into array; where to store the character

void setup(){
  Serial.begin(9600);
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor(); 
}

void loop(){
 
 // Is new summary available?!
  if (Serial.available())  {
    index = 0;
    summaryStr="";
    do {
      inChar = Serial.read(); // Read a character
      summaryStr = String(summaryStr + inChar); // Store it
    } while (Serial.available() > 0);
    
    delay(10);
// Is new location available?!   
    if (Serial.available())  {
      index = 0;
      locationStr="";
      do {
        inChar = Serial.read(); // Read a character
        locationStr = String(locationStr + inChar); // Store it
      } while (Serial.available() > 0);
    }
    delay(10);
// Is new time available?!       
    if (Serial.available())  {
      index = 0;
      timeStr="";
      do {
        inChar = Serial.read(); // Read a character
        timeStr = String(timeStr + inChar); // Store it
      } while (Serial.available() > 0);
    }
    
    //number = inChar;
    //Serial.print("character recieved: ");
    //Serial.println(number, DEC);
    
    u8g.firstPage();  
    do {
      u8g.setFont(u8g_font_fub14r);
      u8g.setPrintPos(5,20);
      u8g.print(inData);
      } while( u8g.nextPage() );
  }
}
