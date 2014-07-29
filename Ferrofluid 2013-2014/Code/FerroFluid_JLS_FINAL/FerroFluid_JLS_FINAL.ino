/*Talk about this program...*/


#include <LiquidCrystal.h>  //Include library to control LCDint potPin = A1;  // Select input pin for the potentiometer
//#include <Metro.h>

int potPin = 0;  // Select input pin for the potentiometer
int tempPin1 = 1;  //Select input pin for temperature sensor 1
int tempPin2 = 2;  //Select input pin for temperature sensor 2
int tempPin3 = 3;  //Select input pin for temperature sensor 3
int outPin = 5;      // Select output pin to control motor driver duty cyle
boolean play = 4; //Select input pin for the 'play' function
const int highTemp=100;    //The temperature at which the electromagnet will turn off (F)
const int safeTemp=95;    //The temperature at which the electromagnet can be turned back on (F)
int outDuty = 0;  //The duty cycle of the output. 0 is minimum, 255 is maximum
int potValue = 0;  //The input value of the potentiometer
int tempReadings1[10]={
  0,0,0,0,0,0,0,0,0,0};  //Ten temperature readings to be averaged from sensor 1
int tempReadings2[10]={
  0,0,0,0,0,0,0,0,0,0};  //Ten temperature readings to be averaged from sensor 2
int tempReadings3[10]={
  0,0,0,0,0,0,0,0,0,0};  //Ten temperature readings to be averaged from sensor 3
int total1;    //The total of the temperature readings
int total2;    //The total of the temperature readings
int total3;    //The total of the temperature readings
int tempValue1;    //The averaged temperature reading from sensor 1
int tempValue2;    //The averaged temperature reading from sensor 2
int tempValue3;    //The averaged temperature reading from sensor 3
int maxTempValue;    //The maximum of the three temperature sensor values
float tempFar;  //The maximum temperature in faranheit
int potReadings[2] = {
  0, 0};    //Keeps the last 2 input values to see if it is changing
boolean movement = true;  //Whether or not there is change in the input
boolean tooHot = false;  //Whether or not the electromagnet is too hot
int fan = 10; //Select output pin for fans

boolean splash = false;  //preventing splash variable

long timer = 60000; //Timer with an interval of 1 minute, or 60000 milliseconds
long time0 = 0;
long moveTime = 0;

//Metro timer = Metro(60000);   //Timer with an interval of 1 minute, or 60000 milliseconds
LiquidCrystal lcd(2, 3, 6, 7, 8, 9);  //Initialize library with the numbers of the interface pins

void setup()
{
  pinMode(fan, OUTPUT);
  pinMode(outPin, OUTPUT);
  pinMode(play, INPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  long currentTime = millis();
  long loopTime = currentTime;
}

void loop() {

  int button = digitalRead(play);

  if (button) //automated demo code here
  { 
    lcd.clear();
    lcd.print("Playing!!");
    analogWrite(fan,255);

    for (int i=0; i<10; i++)  //Burst!!
    {
      int pwm_an = 5;
      analogWrite(outPin,pwm_an);
      delay(100);
    }
  

    for (int i=0; i<10; i++)  //slow turn down to prevent splashing
    {
      int pwm_an = 205+(i*5);
      analogWrite(outPin,pwm_an);
      delay(200);
    }

    for (int i=0; i<254; i++) //ramp up
    {

      analogWrite(outPin, 255-(i));
      if (i!=254) 
      {
        delay(20);
      }
      else
      {
        delay(1000); //hold at highest level
      }
    }


    for (int i=0; i<230; i++) //ramp down
    {

      analogWrite(outPin, i);
      if (i!=230) 
      {
        delay(20);
      }
      else
      {
        delay(1000); //hold at lowest level
      }
    }

    for (int i=0; i<4; i++) //number of loops here controls duration of automated 'play'
    {
      analogWrite(outPin, 150); 
      delay(2000);
      analogWrite(outPin, 50);
      delay(3000);
    }
    
    for (int i=0; i<10; i++)  //slow turn down to prevent splashing
    {
      analogWrite(outPin,(155+(i*10)));
      delay(200);
    }

  }


  else // Code here runs whenever device is not in 'Playing' mode.  Reads in Pot, Senses Temperature,etc.

  {

    analogWrite(fan,255);  //turn on fans
    int sensorValue = analogRead(potPin); //read in pot value
//    Serial.print("Pot:");
//    Serial.println(sensorValue);
    int pwm_an = map(sensorValue,0,1023,255,0); //convert pot value to an analog pwm value
    //Serial.println(pwm_an);
    int duty_cycle = map(pwm_an,255,0,0,100); //convert pwm value to duty cycle percentage
    //Serial.println(duty_cycle);

    //Detect if potvalue is changing and modulate timer

    potReadings[0]=potReadings[1];
    potReadings[1]=sensorValue;

    long currentTime = millis(); //save current time


    if (abs(potReadings[1]-potReadings[0])>50) //if not moving for length 'timer' set 'movement' to false
    {
      movement=true;
      moveTime = millis();
    }
    else
    { 
      if (abs((moveTime - currentTime))>timer) //wait 1 minute before turning off electromagnent
      {
        movement=false;
      }
      if (abs((moveTime - currentTime))>timer*10) //delay turning off fans for another 10 minutes (timer * 10)
      {
        analogWrite(fan,0);
      }
    }

    // Splash boolean logic

    if (potReadings[1] < 150 && potReadings[0] > 200)
    {
      splash = true;
    }
    else
    {
      splash = false;
    }

    //Temperature Control

    /*Take ten temperature readings for each of the 3 sensors. Take the average of the ten readings. 
     Choose whichever average is the highest. Convert value to degrees faranheit. Delay between reading
     from different pins to prevent multiplex errors*/
    total1 = 0;
    total2 = 0;
    total3 = 0;
    delay(50);
    for (int i=0; i<10; i++)
    {
      tempReadings1[i] = analogRead(tempPin1);
      total1 = total1 + tempReadings1[i];
    }
    delay(50);
    for (int i=0; i<10; i++)
    {
      tempReadings2[i] = analogRead(tempPin2);
      total2 = total2 + tempReadings2[i];
    }
    delay(50);
    for (int i=0; i<10; i++)
    {
      tempReadings3[i] = analogRead(tempPin3);
      total3 = total3 + tempReadings3[i];
    }
    tempValue1 = total1/10;
    tempValue2 = total2/10;
    tempValue3 = total3/10;
    maxTempValue = max(tempValue1, tempValue2);
    maxTempValue = max(maxTempValue, tempValue3);
    tempFar = float(maxTempValue) * 400 / 1023; //originally *500/1023

    //Check if the magnet is too hot
    if (tempFar > highTemp)
    {
      tooHot = true;
    }
    //Check if magnet is at a safe temperature
    else if (tempFar < safeTemp)
    {
      tooHot = false;
    }

    //If input changing and not too hot then output same value. Otherwise output 0.
    if (movement && !tooHot)
    {
      if (splash)
      {

        for (int i=0; i<10; i++)  //slow turn down to prevent splashing
        {
          analogWrite(outPin,205+(i*5));
          delay(100);
          
          int sensorValue = analogRead(potPin); //read in pot value
          int pwm_an = map(sensorValue,0,1023,255,0); //convert pot value to an analog pwm value
          int duty_cycle = map(pwm_an,255,0,0,100); //convert pwm value to duty cycle percentage
                
          lcd.setCursor(0,0); 
          lcd.print("Electromagnet   ");
          lcd.setCursor(0,1);
          lcd.print("power at ");
          lcd.print(int(duty_cycle));
          lcd.print("%   ");
          
        }
        
      }
      else //if not in splashing case, behave normally

      analogWrite(outPin,pwm_an); //output analog pwm value
    }

    else
    {
      analogWrite(outPin, 255);
      int duty_cycle = 0;
    }

    //print duty cycle and seconds since reset on LCD screen
    lcd.clear();
    lcd.setCursor(0,0); 
    if (!movement && !tooHot)  //Message if not in use
    {
      lcd.print("Turn the knob   ");
      lcd.setCursor(0, 1);
      lcd.print("to start playing");
    }

    if (movement && !tooHot)  //Message if in normal use
    {
      lcd.print("Electromagnet   ");
      lcd.setCursor(0,1);
      lcd.print("power at ");
      lcd.print(int(duty_cycle));
      lcd.print("%   ");
    }
    if (tooHot)  //Message if magnet is above safe temperature
    {
      lcd.print("Too Hot! Magnet temp is  ");
      lcd.setCursor(0, 1);
      lcd.print(int(tempFar));
      lcd.print(" F            ");
    }

  }
}

