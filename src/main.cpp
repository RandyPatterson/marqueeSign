// Inspired by  https://github.com/witnessmenow/ScrollingLEDMatrixTelegram
// Youtube https://www.youtube.com/watch?v=ngrVccEU4e4&t=43s

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <NetBIOS.h>


/* Pinout RGB Matrix (6124 chip)
   https://www.amazon.com/dp/B079JSKF21?psc=1&ref=ppx_yo2ov_dt_b_product_details

HUB75E pinout
R1 | G1        1 | 2
B1 | GND       3 | 4
R2 | G2        5 | 6
B2 | E         7 | 8
 A | B         9 | 10
 C | D        11 | 12
CLK| LAT      13 | 14
OE | GND      15 | 16
*/ 

/* ESP32 Mini Pinout (wemos_d1_mini32)

RGB             ESP32
Matrix          Mini
------          -----------

 1-R1            25
 2-G1            26
 3-B1            27
 4-GND           GND
 5-R2            14
 6-G2            12
 7-B2            13
 8-E             NC  // used if using two panels or 64x64 panels with 1/32 scan
 9-A             23
10-B             19
11-C             5
12-D             17
13-CLK           16
14-LAT           4
15-OE            15
16-GND           GND
*/

// Set web server port number to 80
WebServer server(80);

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another
 
MatrixPanel_I2S_DMA *dma_display = nullptr;

#define FONT_SIZE 2 // Text will be FONT_SIZE x 8 pixels tall.

int delayBetweeenAnimations = 35; // How fast it scrolls, Smaller == faster
int scrollXMove = -1; //If positive it would scroll right

int textXPosition = 0;
int textYPosition = 0;

String message = "Connected to WiFi"; //Starting Text
String newMessage = "";

unsigned long isAnimationDue;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

int currentColor = 0;
const int colorCount = 5;
uint16_t colors[colorCount] = 
{
  dma_display->color565(255, 0, 0),     // Red
  dma_display->color565(0, 0, 255),     // Blue
  dma_display->color565(0, 255, 0),     // Green
  dma_display->color565(255, 255, 255), // White
  dma_display->color565(238, 130, 238)  // Purple

};


void handleGetMessage() {
  Serial.println("Received GET Message");
  server.send(200, "text/plain",message);

}

void handlePostMessage() {
  Serial.println("Received POST Message");
  newMessage = server.arg(0);
  server.send(200);

}


void setup() {
  Serial.begin(115200);

  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  
  );
  mxconfig.double_buff = true;
  mxconfig.clkphase = true;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  Serial.println("Set Brightness");
  dma_display->setBrightness8(90); //0-255
  Serial.println("Begin");
  dma_display->begin();
  Serial.println("Clear Screen");
  dma_display->clearScreen();

  //connect wifi
  dma_display->setTextSize(1);
  dma_display->setTextWrap(true); 
  dma_display->setTextColor(myWHITE); 
  dma_display->setCursor(1,1);
  dma_display->print("Connecting");

  WiFi.mode(WIFI_STA); 
  WiFiManager wm;
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("LedSign"); // anonymous ap

  if(!res) {
      Serial.println("Failed to connect");
      dma_display->setCursor(10,1);
      dma_display->print("Failed to connect");
      ESP.restart();
  } 
  dma_display->clearScreen();
 // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  String netbiosName = "sgn" + WiFi.macAddress();
  netbiosName.replace(":","");
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(netbiosName.c_str());
  Serial.println("NETBIOS Name: ");
  Serial.println(netbiosName);
  NBNS.begin(netbiosName.c_str());

  //configure display for scrolling text

  dma_display->setTextSize(FONT_SIZE);
  dma_display->setTextWrap(false); 
  dma_display->setTextColor(colors[currentColor]);
  textXPosition = dma_display->width(); // Will start one pixel off screen
  textYPosition = dma_display->height() / 2 - (FONT_SIZE * 8 / 2); // This will center the text


  //setup api
  server.on("/message",HTTP_POST,handlePostMessage);
  server.on("/message", handleGetMessage);
  server.begin();

}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;
bool checkNewMessage = false;

void loop() {
  server.handleClient();	
  unsigned long now = millis();
  if (now > isAnimationDue)
  {
    //This sets the timer for when we should scroll again.
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition += scrollXMove;

    //Checking if the very right of the text off screen to the left
    dma_display->getTextBounds(message, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if (textXPosition + w <= 0)
    {
      checkNewMessage = true;
      textXPosition = dma_display->width();
    }

    dma_display->setCursor(textXPosition, textYPosition);

    //The display has to do less updating if you only black out the area
    dma_display->fillRect(0, textYPosition, dma_display->width(), FONT_SIZE * 8, myBLACK);
    dma_display->print(message);

    //This code swaps the second buffer to be visible (puts it on the display)
    dma_display->flipDMABuffer();
  }

  if (checkNewMessage || (message.length() == 0 && newMessage.length() != 0))
  {
    if (message.length() == 0)
      textXPosition = dma_display->width();

    checkNewMessage = false;
    dma_display->setTextColor(colors[currentColor++]);
    
    if (currentColor >= colorCount-1 ) 
      currentColor = 0;
    
    if (message != newMessage)
      message = newMessage;
  }


}