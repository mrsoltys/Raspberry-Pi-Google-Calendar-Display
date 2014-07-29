#!/usr/bin/python

# Larmie
# Arduino-powered alarm clock scheduler
# http://www.lucaseckels.com/larmie
#
# This script reads a Google Calendar, finds the next event, and sends it
# over USB to a Larmie/Arduino board.  The board then interfaces with a
# standard alarm clock to set the alarm appropriately.
#
# This script should generally be run periodically using cron/launchd/whatever.
#
# Make sure you change PORT and CALENDAR_NAME as appropriate.  Requires
# pySerial and the GData Python API.

import serial
import time
import sys

try:
  from xml.etree import ElementTree # for Python 2.5 users
except ImportError:
  from elementtree import ElementTree
import gdata.calendar.service
import gdata.service
import atom.service
import gdata.calendar
import atom
import getopt
import string
import datetime

PORT = '/dev/tty.usbserial-A7007sBx'

CALENDAR_NAME = 'insert your public calendar id here'

def nextAlarmTime():
  
  calendar_service = gdata.calendar.service.CalendarService()
  query = gdata.calendar.service.CalendarEventQuery(CALENDAR_NAME, 'public', 'full')
  now = datetime.datetime.utcnow();
  tomorrow = now + datetime.timedelta(days=1)

  query.start_min = now.isoformat()
  query.start_max = tomorrow.isoformat()
  query.orderby = 'starttime'
  query.singleevents = 'true'
  query.sortorder = 'a'

  feed = calendar_service.CalendarQuery(query)

  # 2009-02-28T22:00:00.000-08:00
  when = feed.entry[0].when[0].start_time
  fmt = '%c%c%c%c' % (when[11], when[12], when[14], when[15])

  return fmt
      
      
def main():
  alarm_time = '9999'
  try:
    alarm_time = nextAlarmTime()
  except:
    pass

  ser = serial.Serial(PORT, timeout=10)
  ser.write(time.strftime('F%H%M') + alarm_time)
  
if __name__ == "__main__":
    sys.exit(main())

