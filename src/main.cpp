// Inspired by  https://github.com/witnessmenow/ScrollingLEDMatrixTelegram
// Youtube https://www.youtube.com/watch?v=ngrVccEU4e4&t=43s

// How to manually add swagger/openapi
// https://techtutorialsx.com/2017/05/28/esp8266-adding-swagger-ui-to-rest-api/



//Libraries
#include <Arduino.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <NetBIOS.h>
#include <Fonts/Org_01.h>
#include <SPIFFS.h>

#include "serialDebug.h"



//#define SERIAL_DEBUG 1

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
  dma_display->color565(255, 255, 255), // White
  dma_display->color565(255, 0, 0),     // Red
  dma_display->color565(0, 0, 255),     // Blue
  dma_display->color565(0, 255, 0),     // Green
  dma_display->color565(238, 130, 238)  // Purple

};



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


// ************* GIF Routines *************
#define FILESYSTEM SPIFFS
AnimatedGIF gif;
File f;
int x_offset, y_offset;


// Draw a line of image directly on the LED Matrix
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > MATRIX_WIDTH)
      iWidth = MATRIX_WIDTH;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + pDraw->iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < pDraw->iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          for(int xOffset = 0; xOffset < iCount; xOffset++ ){
            dma_display->drawPixel(x + xOffset, y, usTemp[xOffset]); // 565 Color Format
          }
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--; 
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else // does not have transparency
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
      {
        dma_display->drawPixel(x, y, usPalette[*s++]); // color 565
      }
    }
} /* GIFDraw() */


void * GIFOpenFile(const char *fname, int32_t *pSize)
{

  f = FILESYSTEM.open(fname);
  if (f)
  {
    DEBUG_PRINT("Playing gif: ");
    DEBUG_PRINTLN(fname);
    *pSize = f.size();
    return (void *)&f;
  }
  return NULL;
} /* GIFOpenFile() */

void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
     f->close();
} /* GIFCloseFile() */

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
} /* GIFReadFile() */

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{ 
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  
  return pFile->iPos;
} /* GIFSeekFile() */

unsigned long start_tick = 0;

void ShowGIF(char *name)
{

  start_tick = millis();
   
  if (gif.open(name, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
  {
    x_offset = (MATRIX_WIDTH - gif.getCanvasWidth())/2;
    if (x_offset < 0) x_offset = 0;
    y_offset = (MATRIX_HEIGHT - gif.getCanvasHeight())/2;
    if (y_offset < 0) y_offset = 0;
    DEBUG_PRINTLN("Successfully opened GIF");
    while (gif.playFrame(true, NULL))
    {      
      dma_display->flipDMABuffer();
      server.handleClient();
      if ( (millis() - start_tick) > 8000) { // we'll get bored after about 8 seconds of the same looping gif
        break;
      }
    }
    gif.close();

  }

} /* ShowGIF() */

//File root;

// ************* End GIF Routines *********


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

  gif.begin(LITTLE_ENDIAN_PIXELS);


}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;
bool scrollMessageCompleted = false;

void loop() {
  server.handleClient();
  if (message.length() == 0 && newMessage.length() == 0) {
    sleep(10);
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
      message = newMessage;
      //play animation GIF if incomming message is not blank
      if (message.length() > 0) {
        String filename = "/pacman.gif";
        char fileArray[filename.length()+1];
        filename.toCharArray(fileArray,filename.length()+1);
        ShowGIF(fileArray);
      }

    }
  }


}