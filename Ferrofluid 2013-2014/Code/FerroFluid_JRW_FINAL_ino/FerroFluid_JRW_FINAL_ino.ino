
#include "U8glib.h"
//Use Gray scale if not using lines
U8GLIB_NHD31OLED_GR u8g(7, 8, 9, 6);	                // SPI Com: SCK = 7, MOSI = 8, CS = 9, A0 = 6
//Use Black and White if using lines
//U8GLIB_NHD31OLED_BW u8g(7, 8, 9, 6);	        


int potPin = 0;                                         // Select input pin for the potentiometer
int tempPin1 = 1;                                       //Select input pin for temperature sensor 1
int tempPin2 = 2;                                       //Select input pin for temperature sensor 2
int tempPin3 = 3;                                       //Select input pin for temperature sensor 3
int outPin = 5;                                         // Select output pin to control motor driver duty cyle
boolean play = 4;                                       //Select input pin for the 'play' function
const int highTemp=100;                                 //The temperature at which the electromagnet will turn off (F)
const int safeTemp=95;                                  //The temperature at which the electromagnet can be turned back on (F)
int outDuty = 0;                                        //The duty cycle of the output. 0 is minimum, 255 is maximum
int potValue = 0;                                       //The input value of the potentiometer
int tempReadings1[10]={0,0,0,0,0,0,0,0,0,0};            //Ten temperature readings to be averaged from sensor 1
int tempReadings2[10]={0,0,0,0,0,0,0,0,0,0};            //Ten temperature readings to be averaged from sensor 2
int tempReadings3[10]={0,0,0,0,0,0,0,0,0,0};            //Ten temperature readings to be averaged from sensor 3
int total1;                                             //The total of the temperature readings
int total2;                                             //The total of the temperature readings
int total3;                                             //The total of the temperature readings
int tempValue1;                                         //The averaged temperature reading from sensor 1
int tempValue2;                                         //The averaged temperature reading from sensor 2
int tempValue3;                                         //The averaged temperature reading from sensor 3
int maxTempValue;                                       //The maximum of the three temperature sensor values
int tempFar;                                            //The maximum temperature in faranheit
int potReadings[2] = {0, 0};                            //Keeps the last 2 input values to see if it is changing
boolean movement = true;                                //Whether or not there is change in the input
boolean moveold = false;
boolean tooHot = false;                                 //Whether or not the electromagnet is too hot
int fan = 10;                                           //Select output pin for fans
boolean splash = false;                                 //preventing splash variable
long timer = 60000;                                     //Timer with an interval of 1 minute, or 60000 milliseconds
long time0 = 0;
long moveTime = 0;
int duty_cycle;
String coilPower;
String mode = "";
String mode2 = "";
String percent = "%";
int button;
int sensorValue;
int pwm_an;

void setup()
{
  pinMode(fan, OUTPUT);
  pinMode(outPin, OUTPUT);
  pinMode(play, INPUT);
  Serial.begin(9600);
  u8g.setContrast(255);
  u8g.setDefaultForegroundColor(); 
  long currentTime = millis();
  long loopTime = currentTime;
}

void loop(){
  //check too hot
  //Temperature Control///////////////////////////////////////////////////////////////////////////////////////////////////
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
  //Temperature Control///////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //check button
  button = digitalRead(play);
  
  
  if(tooHot){
    mode = "Too Hot!";
    mode2 = "Cooling off";
    coilPower = "Off";
    percent = "";
  }
  else if(button){
    mode = "Playing!";
    mode2 = "Routine Started";
    coilPower = "Auto";
    percent = "";
  }
  else if(!movement){
    mode = "Sleeping";
    mode2 = "Turn knob to begin";
    coilPower = "Off";
    percent = "";
  }
  else{
    mode = "Active";
    mode2 = "Knob adjustable";
    percent = "%";
  }
  //check sleep
  //Sleeping Control//////////////////////////////////////////////////////////////////////////////////////////////////////  
  //Detect if potvalue is changing and modulate timer
  sensorValue = analogRead(potPin); //read in pot value
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
  //Sleeping Control//////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //Update Display////////////////////////////////////////////////////////////////////////////////////////////////////////
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  //Update Display////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if(tooHot){
  if(moveold){
    rampDown();
    moveold = false;
  }  
  }
  else if(button){
    //Playing!//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    moveold = true;
    analogWrite(fan,255);
    for (int i=0; i<10; i++)  //Burst!!
    {
      pwm_an = 5;
      analogWrite(outPin,pwm_an);
      delay(100);
    }
    for (int i=0; i<10; i++)  //slow turn down to prevent splashing
    {
      pwm_an = 205+(i*5);
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
    //\Playing!/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  }
  else if(!movement){
    //Sleeping//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(moveold){
      rampDown();
      moveold = false;
    }
    //Sleeping//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
  }
  else{
    //Adjustable////////////////////////////////////////////////////////////////////////////////////////////////////////////
    moveold = true;
    analogWrite(fan,255);  //turn on fans
    pwm_an = map(sensorValue,0,1023,255,0); //convert pot value to an analog pwm value
    duty_cycle = map(pwm_an,255,0,0,100); //convert pwm value to duty cycle percentage
    coilPower = (String)duty_cycle;

    // Splash boolean logic

    if (potReadings[1] < 150 && potReadings[0] > 200)
    {
      rampDown();
    }
    else
    {
      analogWrite(outPin,pwm_an); //output analog pwm value
    }
    //Adjustable////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
  }  
}


void rampDown(){
  for (int i=0; i<10; i++){
    analogWrite(outPin,205+(i*5));
    delay(100);
    sensorValue = analogRead(potPin); //read in pot value
    pwm_an = map(sensorValue,0,1023,255,0); //convert pot value to an analog pwm value
    duty_cycle = map(pwm_an,255,0,0,100); //convert pwm value to duty cycle percentage
    coilPower = (String)duty_cycle;
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}

void draw(void) {
  //Mode////////////////////////////////////// 
    u8g.setFont(u8g_font_fub17r);
    u8g.setPrintPos(5,20);
    u8g.print(mode);
    u8g.setFont(u8g_font_fur14r);
    u8g.setPrintPos(5,60);
    u8g.print(mode2);
    //Coil Temp/////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(180,9,"Coil Temp");  
    u8g.setFont(u8g_font_fur17);
    u8g.setPrintPos(180,29);
    u8g.print(tempFar);
    u8g.setPrintPos(220,29);
    u8g.print("\260F");
    //Coil Power////////////////////////////////
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr(180,40,"Coil Power");  
    u8g.setFont(u8g_font_fur17);
    u8g.setPrintPos(180,60);
    u8g.print(coilPower);
    u8g.setPrintPos(220,60);
    u8g.print(percent); 
}


