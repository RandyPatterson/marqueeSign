// Displays an annimated gif on Matrix display
// based off of driver library examples https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/tree/master/examples/AnimatedGIFPanel

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <SPIFFS.h>
#include "global.h"
#include "serialDebug.h"
#include "annimation.h"

MatrixPanel_I2S_DMA *Annimation::_dma_display;
AnimatedGIF Annimation::_gif;
File Annimation::_f;
int Annimation::x_offset;
int Annimation::y_offset;
WebServer *Annimation::_webserver;


void Annimation::init(MatrixPanel_I2S_DMA *dma_display, WebServer *webserver) {
  _gif.begin(LITTLE_ENDIAN_PIXELS);
  _dma_display = dma_display;
  _webserver = webserver;
}



// Draw a line of image directly on the LED Matrix
void Annimation::GIFDrawFile(GIFDRAW *pDraw) {
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
            _dma_display->drawPixel(x + xOffset, y, usTemp[xOffset]); // 565 Color Format
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
        _dma_display->drawPixel(x, y, usPalette[*s++]); // color 565
      }
    }
} /* GIFDraw() */


void * Annimation::GIFOpenFile(const char *fname, int32_t *pSize)
{

  _f = SPIFFS.open(fname);
  if (_f)
  {
    DEBUG_PRINT("Playing gif: ");
    DEBUG_PRINTLN(fname);
    *pSize = _f.size();
    return (void *)&_f;
  }
  return NULL;
} /* GIFOpenFile() */

void Annimation::GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
     f->close();
} /* GIFCloseFile() */

int32_t Annimation::GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
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

int32_t Annimation::GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{ 
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  
  return pFile->iPos;
} /* GIFSeekFile() */

unsigned long start_tick = 0;

void Annimation::ShowGIF(char *name)
{

  start_tick = millis();
  if (_gif.open(name, Annimation::GIFOpenFile, Annimation::GIFCloseFile, Annimation::GIFReadFile, Annimation::GIFSeekFile, Annimation::GIFDrawFile))
  {
    x_offset = (MATRIX_WIDTH - _gif.getCanvasWidth())/2;
    if (x_offset < 0) x_offset = 0;
    y_offset = (MATRIX_HEIGHT - _gif.getCanvasHeight())/2;
    if (y_offset < 0) y_offset = 0;
    DEBUG_PRINTLN("Successfully opened GIF");
    while (_gif.playFrame(true, NULL))
    {      
      _dma_display->flipDMABuffer();
      //handle any incomming server requests while the annimation is playing
      _webserver->handleClient();
      if ( (millis() - start_tick) > 8000) { // we'll get bored after about 8 seconds of the same looping gif
        break;
      }
    }
    _gif.close();

  }

} /* ShowGIF() */


