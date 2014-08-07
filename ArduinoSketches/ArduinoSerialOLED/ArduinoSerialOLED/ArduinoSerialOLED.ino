#include "U8glib.h"
//Use Gray scale if not using lines
U8GLIB_NHD31OLED_GR u8g(4, 5, 6, 3);
String summaryStr="";
String timeStr="";
String locationStr="";

byte number = 0;
char inChar= '!';

void setup(){
  Serial.begin(9600);
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor(); 
}

void loop(){
  if (Serial.available())  {
    summaryStr="";
    timeStr="";
    locationStr="";
    do {
      do {
      inChar = Serial.read(); // Read a character
      summaryStr = String(summaryStr + inChar); // Store it
      } while (inChar!='\0');
      do {
        inChar = Serial.read(); // Read a character
        timeStr = String(timeStr + inChar); // Store it
      } while (inChar!='\0');
      do {
        inChar = Serial.read(); // Read a character
        locationStr = String(locationStr + inChar); // Store it
      } while (inChar!='\0');
    } while (Serial.available() > 0);
    
    u8g.firstPage();  
    do {
        draw();
      } while( u8g.nextPage() );
  }
}

void draw(void) {
  //Mode////////////////////////////////////// 
    u8g.setFont(u8g_font_fur14r);
    u8g.setPrintPos(1,20);
    u8g.print(summaryStr);
   // u8g.setFont(u8g_font_fur14r);
   // u8g.setPrintPos(5,60);
   // u8g.print(mode2);
    //Coil Temp/////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(1,40,"At:");  
    u8g.setFont(u8g_font_fur14r);
    u8g.setPrintPos(1,60);
    u8g.print(locationStr); 
//    u8g.setFont(u8g_font_fur17);
//    u8g.setPrintPos(180,29);
//    u8g.print(tempFar);
//    u8g.setPrintPos(220,29);
//    u8g.print("\260F");
    //Coil Power////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(170,40,"Until:");  
    u8g.setFont(u8g_font_fur14r);
    u8g.setPrintPos(170,60);
    u8g.print(timeStr); 
 //   u8g.setFont(u8g_font_fur17);
 //   u8g.setPrintPos(180,60);
 //   u8g.print(coilPower);
 //   u8g.setPrintPos(220,60);
 //   u8g.print(percent); 
}
