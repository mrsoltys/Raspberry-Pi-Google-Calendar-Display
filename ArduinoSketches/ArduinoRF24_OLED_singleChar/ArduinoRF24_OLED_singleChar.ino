#include "U8glib.h"
#include <SPI.h>    //SPI?
#include <nRF24L01.h>//Radio
#include <RF24.h>  //Radio
#include <printf.h> //helps with string formatting

//Use Gray scale if not using lines
//set up OLED display on pins 3,4,5,6
U8GLIB_NHD31OLED_GR u8g(4, 5, 6, 3);

byte number = 0;
char inChar= '!';


// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9, 10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

String summaryStr = "                    ";
String timeStr;
String locationStr; 

void setup(){
  Serial.begin(9600);
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor(); 

  radio.begin();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  // radio.setPayloadSize(20);

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  
  radio.startListening();
  summaryStr="";
}

void loop(){
  // if there is data ready
  if ( radio.available() ) {
    bool done = false;
    while (!done) {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &inChar, 1);
      summaryStr=String(summaryStr + inChar);
    }   
    Serial.println(inChar); 
    
    // Ping back with a response w/ the packet info 
  //  radio.stopListening();
  //  radio.write( &inChar, 1 );
  //  radio.startListening();
  // summaryStr=String(summaryStr + inChar);
  //  //Draw input on display
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
  u8g.setFont(u8g_font_fur11r);
  u8g.setPrintPos(1,63);
  u8g.print(locationStr); 
  //    u8g.setFont(u8g_font_fur17);
  //    u8g.setPrintPos(180,29);
  //    u8g.print(tempFar);
  //    u8g.setPrintPos(220,29);
  //    u8g.print("\260F");
  //Coil Power////////////////////////////////
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr(170,40,"Until:");  
  u8g.setFont(u8g_font_fur11r);
  u8g.setPrintPos(170,63);
  u8g.print(timeStr); 
  //   u8g.setFont(u8g_font_fur17);
  //   u8g.setPrintPos(180,60);
  //   u8g.print(coilPower);
  //   u8g.setPrintPos(220,60);
  //   u8g.print(percent); 
}

