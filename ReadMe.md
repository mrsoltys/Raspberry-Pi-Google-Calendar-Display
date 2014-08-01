Raspberry Pi Google Calendar Display
====================================

Gets current event from a google calendar and displays to OLED display.

Hardware:
* [Raspberry Pi B+](https://www.sparkfun.com/products/12994/)
* [NHD 3.12 OLED Display](http://www.digikey.com/product-detail/en/NHD-3.12-25664UMY3/NHD-3.12-25664UMY3-ND/3712528)
* [Logic Level Converter Bi-Directional](https://www.sparkfun.com/products/12009)

Repository Contents
-------------------

The main code to run is gCalDisp.py.  This will call google_calendar.py for authentication, which is verbatium from the [Google Calendar API v3 website](https://developers.google.com/google-apps/calendar/instantiate).  You'll need to create a google developer account, discussed below.


I adapted code from two other Projects, and am including copies of them here
* **/Larmie** - By [Lucas Eckels](http://blog.lucaseckels.com/2009/03/02/larmie-the-arduino-alarm-scheduler/) for some google calendar and serial commands, although the calendar API used here is out of date
* **/Ferrofluid 2013-2014** - An arduino exhibit at the University of Colorado that uses the same display

Additionlly, You may want to check out [The Roominator](https://github.com/bryanduxbury/roominator) which is a similar project, but looked a little more expensive and intensive than I needed.

Google Calendar API V3
----------------------

This project relies heavily on [Google Calendar API V3](https://developers.google.com/google-apps/calendar/).  Head over to the [Google Developers Console](https://console.developers.google.com/project), create a project, and click "Credentials" to get started.  You'll need to create a new client ID as well as Public API access.  Note: I used a server application API access key, which is tied to an IP address.  This could be an issue for the future, but to find your IP just google ["What's my IP address?"](https://www.google.com/search?q=whats+my+ip+address).  Be aware of quotas, and that exceeding quotas may result in charges.  

Getting Started with Raspberry Pi
---------------------------------

1.  Install Setuptools
          > wget https://bootstrap.pypa.io/ez_setup.py -O - | sudo python
2.  Install the Google APIs Client Library for Python
          > sudo easy_install --upgrade google-api-python-client
3.  Install gflags
    a.  Download from [https://code.google.com/p/python-gflags/](https://code.google.com/p/python-gflags/)
    b.  from the download directory, run: 
'''
          sudo python./setup.py install
'''
4.  Remove Dillo and Midori, install Chromium
'''
          sudo apt-get remove dillo midori
          sudo apt-get install chromium
'''
5. Run for the first stime, and authenticate
    a.  from this directory run
'''
           sudo python gCalDisp.py
'''
    b.  A link will appear, and you'll have to verify to continue.
6. Use Ctrl-C to terminate the code.


License Information
-------------------

The hardware is released under [Creative Commons Share-alike 3.0](http://creativecommons.org/licenses/by-sa/3.0/).  
All other code is open source so please feel free to do anything you want with it; you buy me a beer if you use this and we meet someday ([Beerware license](http://en.wikipedia.org/wiki/Beerware))
