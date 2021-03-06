// Inspired by  https://github.com/witnessmenow/ScrollingLEDMatrixTelegram
// Youtube https://www.youtube.com/watch?v=ngrVccEU4e4&t=43s

// How to manually add swagger/openapi
// https://techtutorialsx.com/2017/05/28/esp8266-adding-swagger-ui-to-rest-api/



//Libraries
#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <NetBIOS.h>
#include <Fonts/Org_01.h>
#include <SPIFFS.h>

#include "serialDebug.h"
#include "annimation.h"
#include "global.h"

//#define SERIAL_DEBUG 1

MatrixPanel_I2S_DMA *dma_display = nullptr;


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


// Pinout for KiCad Adapter
#define R1_PIN 27
#define G1_PIN 22
#define B1_PIN 25
#define R2_PIN 32
#define G2_PIN 21
#define B2_PIN 12
#define CH_A_PIN 13
#define CH_B_PIN 16
#define CH_C_PIN 10
#define CH_D_PIN 18
#define CH_E_PIN -1 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK_PIN 14 
#define LAT_PIN 26
#define OE_PIN 33 

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another
 


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
  dma_display->color565(255, 255, 255), // White
  dma_display->color565(255, 0, 0),     // Red
  dma_display->color565(0, 0, 255),     // Blue
  dma_display->color565(0, 255, 0),     // Green
  dma_display->color565(238, 130, 238)  // Purple

};


// Set web server port number to 80
WebServer server(80);


void handleGetMessage() {
  DEBUG_PRINTLN("Received GET Message");
  server.send(200, "text/plain",message);

}

void handlePostMessage() {
  DEBUG_PRINTLN("Received POST Message");
  newMessage = server.arg(0);
  server.send(200);

}

void configPanel(){
    HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );
  //Map pins for adapter. Pins are changed for easier PCB trace routing
  mxconfig.gpio.r1= R1_PIN;
  mxconfig.gpio.g1 = G1_PIN;
  mxconfig.gpio.b1 = B1_PIN;
  mxconfig.gpio.r2 = R2_PIN;
  mxconfig.gpio.g2 = G2_PIN;
  mxconfig.gpio.b2 = B2_PIN;
  mxconfig.gpio.a = CH_A_PIN;
  mxconfig.gpio.b = CH_B_PIN;
  mxconfig.gpio.c = CH_C_PIN;
  mxconfig.gpio.d = CH_D_PIN;
  mxconfig.gpio.e = CH_E_PIN;
  mxconfig.gpio.lat = LAT_PIN;
  mxconfig.gpio.oe = OE_PIN;
  mxconfig.gpio.clk = CLK_PIN;
  mxconfig.double_buff = true;
  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->setBrightness8(90); //0-255
  if( not dma_display->begin() )
      DEBUG_PRINTLN("****** I2S memory allocation failed ***********");
  dma_display->clearScreen();
}

void configWebServer() {
  //setup api
  server.on("/message",HTTP_POST,handlePostMessage);
  server.on("/message", handleGetMessage);
  server.serveStatic("/", SPIFFS, "/swaggerui.html");
  server.serveStatic("/swagger.json", SPIFFS, "/swagger.json");
  server.begin();
}

// Called when config mode launched
void configModeCallback(WiFiManager *myWiFiManager)
{
  dma_display->clearScreen();
  dma_display->setCursor(1,5);
  dma_display->print("Connect To");
  dma_display->setCursor(1,12);
  dma_display->print(myWiFiManager->getConfigPortalSSID());
  dma_display->setCursor(1,19);
  dma_display->print(WiFi.softAPIP());
  
}

void setSaveConfigCallback(WiFiManager *myWiFiManager) 
{
  dma_display->setCursor(1,26);
  dma_display->print("Saving Configuration");

}

String connectWiFi() {
  WiFi.mode(WIFI_STA); 
  WiFiManager wm;
  bool res;

  //Debug
  //wm.resetSettings();
  
  // Set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);  

  res = wm.autoConnect("Marquee"); // anonymous ap


  if(!res) {
      DEBUG_PRINTLN("Failed to connect");
      dma_display->setCursor(10,1);
      dma_display->print("Failed to connect");
      ESP.restart();
  } 

  return WiFi.localIP().toString();
  

}

void setup() {
  DEBUG_INIT
  configPanel();

  //init file system
  SPIFFS.begin(true);
  
  //connect wifi
  dma_display->setFont(&Org_01);
  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE); 
  dma_display->setCursor(1,7);
  dma_display->print("Connecting");

  String ipaddr  = connectWiFi();
  message = "http://"+ipaddr;
 

  //configure display for scrolling text
  dma_display->clearScreen();
  dma_display->setFont(); 
  dma_display->setTextSize(FONT_SIZE);
  dma_display->setTextWrap(false); 
  dma_display->setTextColor(colors[currentColor]);
  textXPosition = dma_display->width(); // Will start one pixel off screen
  textYPosition = dma_display->height() / 2 - (FONT_SIZE * 8 / 2); // This will center the text


  configWebServer();

  Annimation::init(dma_display, &server);

}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;
bool scrollMessageCompleted = false;

void loop() {
  server.handleClient();
  if (message.length() == 0 && newMessage.length() == 0) {
    return;  //Nothing to do
  }

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
      scrollMessageCompleted = true;
      textXPosition = dma_display->width();
    }

    dma_display->setCursor(textXPosition, textYPosition);

    //The display has to do less updating if you only black out the area
    dma_display->fillRect(0, textYPosition, dma_display->width(), FONT_SIZE * 8+6, myBLACK);
    dma_display->print(message);

    //This code swaps the second buffer to be visible (puts it on the display)
    dma_display->flipDMABuffer();
  }

  //Set new message text if the current message scrolled off screen OR set immediately if current
  //message is empty
  if (scrollMessageCompleted|| (message.length() == 0 && newMessage.length() > 0))
  {
    if (message.length() == 0)
      textXPosition = dma_display->width();

    scrollMessageCompleted = false;
    dma_display->setTextColor(colors[currentColor++]);
    
    if (currentColor >= colorCount-1 ) 
      currentColor = 0;
    
    if (message != newMessage) {
      //play animation GIF if incomming message is not blank
      if (newMessage.length() > 0) {
        String filename = "/pacman.gif";
        char fileArray[filename.length()+1];
        filename.toCharArray(fileArray,filename.length()+1);
        Annimation::ShowGIF(fileArray);
      }
      message = newMessage;

    }
  }


}