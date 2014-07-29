// Larmie
// Alarm clock scheduler, Arduino component
// Copyright 2009 Lucas Eckels
// http://lucaseckels.com/larmie

// This code accepts a number of commands over the serial port,
// then triggers the clock as appropriate.

// Commands:
//   Fxxxxyyyy
//     Set time to xxxx (if it hasn't already been set) and alarm to yyyy.
//     This should be the command actually used -- the rest are for debugging.
//     xxxx is used to avoid setting off the alarm while changing the time.
//   Txxxx
//     Set the time to the 24-hour time indicated in xxxx
//   Ayyyy
//     Set the alarm to the 24-hour time indicated in yyyy
//   Pz
//     Beginning pressing the button indicated by z.  Valid values are:
//       A alarm
//       H hour
//       M minute
//       S sleep
//   Rz
//     Release the button indicated by z.  See P for valid values

int ALARM_PIN = 12;
int HOUR_PIN = 10;
int MINUTE_PIN = 9;
int SLEEP_PIN = 8;

int PWR_PIN = 11;
int BLINK_PIN = 13;

int BUTTON_PRESSED = LOW;
int BUTTON_RELEASED = HIGH;

int DEBOUNCE_DELAY = 25;
int FUZZ = 2;

bool timeSet = false;
int alarmMinuteSet = -1;
int alarmHourSet = -1;

void setup() {
  pinMode(BLINK_PIN, OUTPUT);
  pinMode(PWR_PIN, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(HOUR_PIN, OUTPUT);
  pinMode(MINUTE_PIN, OUTPUT);
  pinMode(SLEEP_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  digitalWrite(ALARM_PIN, BUTTON_RELEASED);
  digitalWrite(HOUR_PIN, BUTTON_RELEASED);
  digitalWrite(MINUTE_PIN, BUTTON_RELEASED);
  digitalWrite(SLEEP_PIN, BUTTON_RELEASED);
  
  Serial.begin(9600);
}

void pressButton(int button, int count) {
  for (int i = 0; i < count; ++i) {
    digitalWrite(button, BUTTON_PRESSED);
    delay(DEBOUNCE_DELAY); 
    digitalWrite(button, BUTTON_RELEASED);
    delay(DEBOUNCE_DELAY);
  }
}

void reset(bool time) {
  digitalWrite(ALARM_PIN, BUTTON_PRESSED);
  if (time) {
    digitalWrite(SLEEP_PIN, BUTTON_PRESSED);
  }
  delay(DEBOUNCE_DELAY);
  digitalWrite(HOUR_PIN, BUTTON_PRESSED);
  digitalWrite(MINUTE_PIN, BUTTON_PRESSED);
  delay(DEBOUNCE_DELAY);
  digitalWrite(HOUR_PIN, BUTTON_RELEASED);
  digitalWrite(MINUTE_PIN, BUTTON_RELEASED);
  digitalWrite(ALARM_PIN, BUTTON_RELEASED);
  if (time) {
    digitalWrite(SLEEP_PIN, BUTTON_RELEASED);
  }
  delay(DEBOUNCE_DELAY);
  
}

bool setAlarm(int hour, int minute) {
  reset(false);
  
  digitalWrite(ALARM_PIN, BUTTON_PRESSED);
  delay(DEBOUNCE_DELAY);
  pressButton(HOUR_PIN, hour);
  pressButton(MINUTE_PIN, minute);
  digitalWrite(ALARM_PIN, BUTTON_RELEASED);
  delay(DEBOUNCE_DELAY);
  
  return true;
}

bool setTime(int hour, int minute) {
  reset(true);
  
  pressButton(HOUR_PIN, hour);
  pressButton(MINUTE_PIN, minute);
  
  timeSet = true;
  return true;
}

int getDigits(int count = 2) {
  while (Serial.available() < count) {
  }
  int val = 0;
  for (int i = 0; i < count; ++i) {
    val *= 10;
    val += Serial.read() - '0';
  }
  return val;
}
  
bool getTime(bool(func)(int, int)) {
  int hour = getDigits();
  int minute = getDigits();
  return func(hour, minute);
}

bool minutesMayBeZero(int minute) {
  return (minute < FUZZ || minute + FUZZ > 59);
}

bool doFull() {
  int timeHour = getDigits(2);  
  int timeMinute = getDigits(2);  
  int alarmHour = getDigits(2);  
  int alarmMinute = getDigits(2);

  if (!timeSet) {
    setTime(timeHour, timeMinute);
  }

  if (minutesMayBeZero(timeMinute)) {
    return false;
  }

  if (alarmHour == 99 && alarmMinute == 99) {
    if (timeHour < 13) {
      alarmHour = alarmMinute = 0;
    } else {
      alarmHour = 12;
      alarmMinute = 0;
    }
  }
  
  if (alarmHour == alarmHourSet && alarmMinute == alarmMinuteSet) {
    return true;
  }
  
  reset(false);
  digitalWrite(ALARM_PIN, BUTTON_PRESSED);
  delay(DEBOUNCE_DELAY);

  if (timeHour != alarmHour) {
    pressButton(HOUR_PIN, alarmHour);
    pressButton(MINUTE_PIN, alarmMinute);
  } else if (timeHour != 0 || alarmMinute < timeMinute) {
    pressButton(MINUTE_PIN, alarmMinute);
    pressButton(HOUR_PIN, alarmHour);
  } else {
    pressButton(HOUR_PIN, 1);
    pressButton(MINUTE_PIN, alarmMinute);
    pressButton(HOUR_PIN, 23);
  }
 
  digitalWrite(ALARM_PIN, BUTTON_RELEASED);
  delay(DEBOUNCE_DELAY);
  
  alarmHourSet = alarmHour;
  alarmMinuteSet = alarmMinute;
  
  return true;
}

bool doButton(bool press) {
  while (Serial.available() < 1) {
    delay(1);
  }

  int val = press ? BUTTON_PRESSED : BUTTON_RELEASED;
  int button = -1;
  switch (Serial.read()) {
    case 'A':
      button = ALARM_PIN;
      break;
    case 'H':
      button = HOUR_PIN;
      break;
    case 'M':
      button = MINUTE_PIN;
      break;
    case 'S':
      button = SLEEP_PIN;
      break;
    default:
      break;
  }
  if (button != -1) {
    digitalWrite(button, val);
    return true;
  }
  return false;
}


void loop() {
  bool result = false;
  digitalWrite(BLINK_PIN, HIGH);
  Serial.print("READY\n");
  
  while (Serial.available() == 0) {
    ;
  }
  digitalWrite(BLINK_PIN, LOW);
  switch (Serial.read()) {
    case 'T':
      result = getTime(setTime);
      break;
    case 'A':
      result = getTime(setAlarm);
      break;
    case 'F':
      result = doFull();
      break;
    case 'P':
      result = doButton(true);
      break;
    case 'R':
      result = doButton(false);
      break;
    default:
      Serial.print("NOT RECOGNIZED\n");
      break;
  }
  
  if (!result) {
    Serial.print("ERROR\n");
  }
}
