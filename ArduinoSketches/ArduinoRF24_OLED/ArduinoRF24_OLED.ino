#include "U8glib.h"
#include <SPI.h>    //SPI?
#include <nRF24L01.h>//Radio
#include <RF24.h>  //Radio

//set up OLED display on pins 3,4,5,6
  U8GLIB_NHD31OLED_GR u8g(4, 5, 6, 3);

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
  RF24 radio(9, 10);
  // Radio pipe addresses for the 2 nodes to communicate.
    const uint64_t pipes[2] = {
      0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

char summaryStr[27];
char timeStr[10];
char locationStr[22];

void setup() {
  //Serial.begin(9600);
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor();

  radio.begin();
  // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);

  // optionally, reduce the payload size.  seems to improve reliability
    radio.setPayloadSize(28);

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
}

bool done = false;
unsigned long started_waiting_at = millis();
void loop() {
  // if there is data ready
  if ( radio.available() ) {
    bool timeout = false;
    started_waiting_at = millis();
    bool readSummary = false;
    bool readTime = false;
    bool readLoc = false;
    
    while (!timeout ) {
      if (!readSummary ) {
        done = radio.read( &summaryStr, sizeof(summaryStr));
        readSummary = true;
        // Serial.print(millis()-started_waiting_at);
        // Serial.println(radio.available());
        //Serial.print(" Got Summary: ");
        // Serial.println(summaryStr);
      }
      else if (!readTime && radio.available()) {
        done = radio.read( &timeStr, sizeof(timeStr));
        readTime = true;
        // Serial.print(millis()-started_waiting_at);
        // Serial.println(radio.available());
        // Serial.print(" Got Time: ");
        // Serial.println(timeStr);
      }
      else if (!readLoc && radio.available()) {
        done = radio.read( &locationStr, sizeof(locationStr));
        readLoc = true;
        //Serial.print(millis()-started_waiting_at);
        //Serial.println(radio.available());
        //Serial.print("Got Location: ");
        //Serial.println(locationStr);
      }
      else {
        delay(10);
      }

      if (millis() - started_waiting_at >= 500 ) {
        timeout = true;
      }
    }

    if (readSummary && readLoc && readTime) {
//      Serial.println("Success");
//      Serial.println(summaryStr);
//      Serial.println(timeStr);
//      Serial.println(locationStr);

      //Draw input on display
      u8g.firstPage();
      do {
        draw();
      } while ( u8g.nextPage() );
    }
    else { 
      //Serial.println("Failure"); 
    }
    clearStrings();  
  }
}

void draw(void) {
  //Summary//////////////////////////////////////
  u8g.setFont(u8g_font_fur14r);
  u8g.setPrintPos(1, 20);
  u8g.print(summaryStr);
  // u8g.setFont(u8g_font_fur14r);
  // u8g.setPrintPos(5,60);
  // u8g.print(mode2);
  //Location/////////////////////////////////
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr(1, 40, "At:");
  u8g.setFont(u8g_font_fur11r);
  u8g.setPrintPos(1, 63);
  u8g.print(locationStr);
  //Time////////////////////////////////
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr(170, 40, "Until:");
  u8g.setFont(u8g_font_fur11r);
  u8g.setPrintPos(170, 63);
  u8g.print(timeStr);
}

void clearStrings() {
  for( int i = 0; i < sizeof(summaryStr);  i++ )
   summaryStr[i] = ' ';
  for( int i = 0; i < sizeof(timeStr);  i++ )
   timeStr[i] = ' ';
  for( int i = 0; i < sizeof(locationStr);  i++ )
   locationStr[i] = ' '; 
}

