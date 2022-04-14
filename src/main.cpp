#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another
 
MatrixPanel_I2S_DMA *dma_display = nullptr;


#define FONT_SIZE 2 // Text will be FONT_SIZE x 8 pixels tall.

int delayBetweeenAnimations = 35; // How fast it scrolls, Smaller == faster
int scrollXMove = -1; //If positive it would scroll right

int textXPosition = 0;
int textYPosition = 0;

String text = "Hello World!!"; //Starting Text

unsigned long isAnimationDue;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

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

  textXPosition = dma_display->width(); // Will start one pixel off screen
  textYPosition = dma_display->height() / 2 - (FONT_SIZE * 8 / 2); // This will center the text

  dma_display->setTextSize(FONT_SIZE);
  dma_display->setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display->setTextColor(myRED); // Can change the colour here

}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;
bool checkTelegram = false;

void loop() {
  unsigned long now = millis();
  if (now > isAnimationDue)
  {
    //This sets the timer for when we should scroll again.
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition += scrollXMove;

    //Checking if the very right of the text off screen to the left
    dma_display->getTextBounds(text, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if (textXPosition + w <= 0)
    {
      checkTelegram = true;
      textXPosition = dma_display->width();
    }

    dma_display->setCursor(textXPosition, textYPosition);

    //The display has to do less updating if you only black out the area
    dma_display->fillRect(0, textYPosition, dma_display->width(), FONT_SIZE * 8, myBLACK);
    dma_display->print(text);

    //This code swaps the second buffer to be visible (puts it on the display)
    dma_display->flipDMABuffer();
  }
}