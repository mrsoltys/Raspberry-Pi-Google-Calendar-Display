# This script checks in a google calendar for the current event and then parses the
# text to currently print to the terminal.

# This is my calendar...
calendarId = 'lkqtiiehlj121aqp0a7g3ma25k@group.calendar.google.com'

# Note: pip install --upgrade google-api-python-client installed to the wrong path,
# So this is just a hack to tell python where to find it...
import sys
sys.path.append("/usr/local/lib/python2.7/site-packages")

import google_calendar
import pprint
# Note: If i decide to talk via serial commands, I'll need this...
# import serial
import time
import datetime
import lcd_display

def getEvents(pageToken=None):
    # Get the current date and time
    now = datetime.datetime.utcnow();
    future = now + datetime.timedelta(minutes=1)
    
    # for debugging, print now and future to terminal.
    print now.isoformat()
    print future.isoformat()

    #query google_calendar for current event
    page_token = None
    events = google_calendar.service.events().list(
        calendarId= calendarId,
        timeMin=now.isoformat()+'z',
        timeMax=future.isoformat()+'z',
        pageToken=page_token
        ).execute()
    
    return events

def main():
    lcd_display.main()
    # Get Current Event
    while True:
      events = getEvents()


    # Print out current event summary, location, and end time. If any of those 
    # fields does not exist, ignore the KeyError exception and move along.
      for event in events['items']:
        print 'Prof Mike Soltys is:'
        try:
          print event['summary']
          lcd_display.lcd_byte(lcd_display.LCD_LINE_1, lcd_display.LCD_CMD)
 	  lcd_display.lcd_string(event['summary'])
          endTime = event['end']['dateTime']
        # Note: endTime is in iso format with timezone info, we just need to strip
        # the last 6 digits to get the current local time
          endTime = endTime[:-6] 
        # Print Time in 12H:MM AM/PM format...
          ET=datetime.datetime.strptime(endTime, "%Y-%m-%dT%H:%M:%S")
          print ET.strftime("%I:%M%p")
          lcd_display.lcd_byte(lcd_display.LCD_LINE_2, lcd_display.LCD_CMD)
          lcd_display.lcd_string('Until: '+ET.strftime("%I:%M%p"))
        except KeyError:
          continue
      time.sleep(60)

# This is just nice to have because it can tell the terminal 
# running python where to find main()
if __name__ == '__main__':
    main()