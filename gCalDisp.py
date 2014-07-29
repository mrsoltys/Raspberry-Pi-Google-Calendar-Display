calendarId = 'lkqtiiehlj121aqp0a7g3ma25k@group.calendar.google.com'

# pip install --upgrade google-api-python-client
import sys
sys.path.append("/usr/local/lib/python2.7/site-packages")
import google_calendar
import pprint
# import serial
import time
import datetime
#import dateutil.parser


def getEvents(pageToken=None):
    now = datetime.datetime.utcnow();
    tomorrow = now + datetime.timedelta(days=1)
    print now.isoformat()
    print tomorrow.isoformat()
  #query.start_min = now.isoformat()
  #query.start_max = tomorrow.isoformat()

    page_token = None
    while True:
      events = google_calendar.service.events().list(
        calendarId= calendarId
        timeMin=now.isoformat()+'z',
        timeMax=tomorrow.isoformat()+'z',
        pageToken=page_token
        ).execute()
      for event in events['items']:
        print 'Prof Mike Soltys is:'
        try:
          print event['summary']
        except KeyError:
          continue
        print 'At:'  
        try:
          print event['location']
        except KeyError:
          continue
        print 'Until:'    
        endTime = event['end']['dateTime']
        endTime = endTime[:-6] #Need to strip timezone info
        ET=datetime.datetime.strptime(endTime, "%Y-%m-%dT%H:%M:%S")
        print ET.strftime("%I:%M%p")

      page_token = events.get('nextPageToken')
      if not page_token:
        break


    calendar = google_calendar.service.calendars().get(
        calendarId=calendarId).execute()

    #print calendar['summary']
    #print 'hello world'
    # events = google_calendar.service.events().list(
    #     calendarId='lkqtiiehlj121aqp0a7g3ma25k@group.calendar.google.com',
    #     singleEvents=True,
    #     maxResults=1000,
    #     orderBy='startTime',
    #     timeMin='2014-11-01T00:00:00-08:00',
    #     timeMax='2014-11-30T00:00:00-08:00',
    #     pageToken=pageToken,
    #     ).execute()
    # return events
    return calendar

def main():
    #print 'hello world 1'
     events = getEvents()
    # while True:
    #     for event in events['items']:
    #         pprint.pprint(event)
    #     page_token = events.get('nextPageToken')
    #     if page_token:
    #         events = getEvents(page_token)
    #     else:
    #         break

if __name__ == '__main__':
    main()