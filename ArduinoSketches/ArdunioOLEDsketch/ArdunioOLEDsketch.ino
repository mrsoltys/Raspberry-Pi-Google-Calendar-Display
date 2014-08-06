#include "U8glib.h"
//Use Gray scale if not using lines
U8GLIB_NHD31OLED_GR u8g(7, 8, 9, 6);	

String coilPower;
String mode = "";
String mode2 = "";
String percent = "%";

void setup()
{
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor(); 
 
}

void loop(){
     mode = "Programming Stuff";
    mode2 = "Turn knob to begin";
    coilPower = "03:30PM";
    percent = ""; 
    
     u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_fub14r);
    u8g.setPrintPos(5,20);
    u8g.print(mode);
    u8g.setFont(u8g_font_fur14r);
    u8g.setPrintPos(5,60);
    u8g.print(mode2);
    //Coil Temp/////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(5,40,"at:");  
    u8g.setPrintPos(5,60);
    u8g.print("DLC 174");
    //Coil Power////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(180,40,"until:");  
    u8g.setFont(u8g_font_fur14);
    u8g.setPrintPos(180,60);
    u8g.print(coilPower);
    u8g.setPrintPos(220,60);
    u8g.print(percent); 
  } while( u8g.nextPage() );
}


