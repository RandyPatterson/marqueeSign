# Scrolling Text Sign
WiFi Connected Marquee Sign.  
![Display](.\content\matrixdisplay.jpg)

Connections
![Connections](.\content\connections.jpg)

Uses this P3 Led Panel on [Amazon](https://www.amazon.com/gp/product/B079JSKF21/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1)
## Content

| Directory | Notes |
|-----------|-------| 
\src |  Source Code for ESP32 using PlatformIO.
\KiCad | Optional PCB to wrangle all of the connections required for the display


Sign exposes an API to retrieve the current message and set the next message 

| Verb | Url | Notes |
|------|-----|-------|
| GET | /message | Gets the current message displayed on the sign |
| POST | /message | Sets the next message to display.  Will change after the current message has finished scrolling |
| GET | / | OpenAPI (swagger) UI for testing and discovery |