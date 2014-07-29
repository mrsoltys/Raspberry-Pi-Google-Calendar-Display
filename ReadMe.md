Raspberry Pi Google Calendar Display
====================================

Gets current event from a google calendar and displays to OLED display.

Hardware:
* [Raspberry Pi B+](https://www.sparkfun.com/products/12994/)
* [NHD 3.12 OLED Display](http://www.digikey.com/product-detail/en/NHD-3.12-25664UMY3/NHD-3.12-25664UMY3-ND/3712528)
* [Logic Level Converter Bi-Directional](https://www.sparkfun.com/products/12009)

Repository Contents
-------------------

I adapted code from two other Projects, and am including copies of them here
* **/Larmie** - By [Lucas Eckels](http://blog.lucaseckels.com/2009/03/02/larmie-the-arduino-alarm-scheduler/) for some google calendar and serial commands, although the calendar API used here is out of date
* **/Ferrofluid 2013-2014** - An arduino exhibit at the University of Colorado that uses the same display

Additionlly, You may want to check out [The Roominator](https://github.com/bryanduxbury/roominator) which is a similar project, but looked a little more expensive and intensive than I needed.

Google Calendar API V3
----------------------

This project relies heavily on [Google Calendar API V3](https://developers.google.com/google-apps/calendar/).  Head over to the [Google Developers Console](https://console.developers.google.com/project), create a project, and click "Credentials" to get started.  You'll need to create a new client ID as well as Public API access.  Note: I used a server application API access key, which is tied to an IP address.  This could be an issue for the future, but to find your IP just google ["What's my IP address?"](https://www.google.com/search?q=whats+my+ip+address).  Be aware of quotas, and that exceeding quotas may result in charges.  

License Information
-------------------

The hardware is released under [Creative Commons Share-alike 3.0](http://creativecommons.org/licenses/by-sa/3.0/).  
All other code is open source so please feel free to do anything you want with it; you buy me a beer if you use this and we meet someday ([Beerware license](http://en.wikipedia.org/wiki/Beerware))
