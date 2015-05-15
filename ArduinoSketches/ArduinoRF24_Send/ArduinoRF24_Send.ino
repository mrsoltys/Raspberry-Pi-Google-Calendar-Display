#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Hardware configuration

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9, 10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL
};

//Note: Maximum Payload size is 32 bytes... This struct is 50 bytes!
typedef struct {
  char summaryStr[20];
  char timeStr[10];
  char locationStr[20];
} calPacket;
calPacket duino;
//calPacket duinoRec;

char inChar = '!';

void setup(void)
{
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  // optionally, reduce the payload size.  seems to
  // improve reliability
   radio.setPayloadSize(22);

  Serial.begin(9600);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
}



void loop() {
  bool ok = false;
  if (Serial.available())  {
    //need to reset Char arrays to be empty
    //need to make sure arrays don't overfill...
    int i = 0;
    //do {
    inChar = Serial.read(); // Read a character
    while (inChar != '\0') {
      if (inChar > 0) {
        duino.summaryStr[i] = inChar; // Store it
        i++;
      }
      inChar = Serial.read(); // Read a character
    }
    i = 0;
    inChar = Serial.read(); // Read a character
    while (inChar != '\0') {
      if (inChar > 0) {
        duino.timeStr[i] = inChar; // Store it
        i++;
      }
      inChar = Serial.read(); // Read a character
    }
    i = 0;
    inChar = Serial.read(); // Read a character
    while (inChar != '\0') {
      if (inChar > 0) {
        duino.locationStr[i] = inChar; // Store it
        i++;
      }
      inChar = Serial.read(); // Read a character
    }

    //}
    //while (Serial.available() > 0);

    // Stop listening so we can talk.
    radio.stopListening();
   // do {
   ok = radio.write( &duino.summaryStr, sizeof(duino.summaryStr) );
   // } while(!ok);
   radio.startListening();
    delay(50);
    radio.stopListening();
     // do {
      ok = radio.write( &duino.timeStr, sizeof(duino.timeStr) );
    //} while(!ok);
    radio.startListening();
    delay(50);
    radio.stopListening();
   // do {
    ok = radio.write( &duino.locationStr, sizeof(duino.locationStr) );
   // } while(!ok);
    radio.startListening();

    // Try again 1s later
    delay(1000);
  }
}


