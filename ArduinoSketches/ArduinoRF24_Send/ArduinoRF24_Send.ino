#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Hardware configuration
  // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
    RF24 radio(9, 10);
  // Radio pipe addresses for the 2 nodes to communicate.
    const uint64_t pipes[2] = {
      0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

  char summaryStr[27];
  char timeStr[10];
  char locationStr[22];
  
  String summary;
  String time;
  String loc;

void setup(void)
{
  clearStrings();
  radio.begin();

  // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
  // optionally, reduce the payload size.  seems to improve reliability
    radio.setPayloadSize(28);

  Serial.begin(9600);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
}

void loop() {
  bool ok = false;
  if(Serial.available()>0) {
     summary = Serial.readStringUntil('\0');
     time = Serial.readStringUntil('\0');
     loc = Serial.readStringUntil('\0');
   
//    Serial.print("Got Summary: ");
//    Serial.println( summary);
//    Serial.print("Got Time: ");
//    Serial.println( time);
//    Serial.print("Got Loc: ");
//    Serial.println(loc); 
       
    summary.toCharArray(summaryStr, 27);
    time.toCharArray(timeStr,10);
    loc.toCharArray(locationStr,22);
    
//    Serial.println(sizeof(summaryStr));
//    Serial.println(summaryStr);
//    Serial.println(sizeof(timeStr));
//    Serial.println(timeStr);
//    Serial.println(sizeof(locationStr));
//    Serial.println(locationStr);

    sendStringsRadio ();
    }
   clearStrings() ; 
}

void clearStrings() {
  for ( int i = 0; i < sizeof( summaryStr);  i++ )
     summaryStr[i] = ' ';
  for ( int i = 0; i < sizeof( timeStr);  i++ )
     timeStr[i] = ' ';
  for ( int i = 0; i < sizeof( locationStr);  i++ )
     locationStr[i] = ' ';
}

void sendStringsRadio () {
   bool ok = false;
   radio.stopListening();
     ok = radio.write( &summaryStr, sizeof(summaryStr) );
   radio.startListening();
   delay(20);
   radio.stopListening();
     ok = radio.write( &timeStr, sizeof(timeStr) );
   radio.startListening();
   delay(20);
   radio.stopListening();
     ok = radio.write( &locationStr, sizeof(locationStr) );
   radio.startListening();
}

