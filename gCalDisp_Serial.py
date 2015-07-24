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
import serial
import time
import datetime

ser = serial.Serial('/dev/ttyAMA0',9600,timeout=1)

def getEvents(pageToken=None):
  # Get the current date and time
  now = datetime.datetime.utcnow();
  future = now + datetime.timedelta(minutes=1)
    
  #query google_calendar for current event
  page_token = None
  events = google_calendar.service.events().list(
      calendarId= calendarId,
      timeMin=now.isoformat()+'z',
      timeMax=future.isoformat()+'z',
      pageToken=page_token
      ).execute()

  if (events['items']):
    for event in events['items']:
      print 'Prof Mike Soltys is:'
      try:
        printCurrentEvent(event)
      except KeyError:
        ser.write("No Data" + '\0' + '\0' + '\0' )
        print 'debug3: No Data'
        continue
  else:
    ser.write("No Data" + '\0' + '\0' + '\0')
    print 'debug4: nodata'

  return events

def printCurrentEvent(event):
  
  # Print out current event summary, location, and end time. If any of those 
  # fields does not exist, ignore the KeyError exception and move along.
  print event['summary']
  ser.write(event['summary']+'\0')

  # Note: endTime is in iso format with timezone info, we just need to strip
  # the last 6 digits to get the current local time and then
  # Print Time in 12H:MM AM/PM format...

  endTime = event['end']['dateTime']
  endTime = endTime[:-6] 
  ET=datetime.datetime.strptime(endTime, "%Y-%m-%dT%H:%M:%S")
  print ET.strftime("%I:%M%p")
  ser.write(ET.strftime("%I:%M%p") +'\0')

  try:
    ser.write(event['location']+'\0')
  except KeyError:
    ser.write('\0')
    print 'debug1: No Location'
  print 'debug2: End Print Event'


def main():
  ser.open()
  # Get Current Event
  #while True:
  try:
    events = getEvents()
  except KeyboardInterrupt:
    ser.close
  ser.close	            
  #time.sleep(60)

# This is just nice to have because it can tell the terminal 
# running python where to find main()
if __name__ == '__main__':
  main()
