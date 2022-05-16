


#ifndef annimation_h
#define annimation_h

#include <Arduino.h>
#include <AnimatedGIF.h>

class Annimation {
    public:
        static void begin(MatrixPanel_I2S_DMA *dma_display);
        static void ShowGIF(char *name);


        static void GIFDrawFile(GIFDRAW *pDraw);
        static void * GIFOpenFile(const char *fname, int32_t *pSize);
        static int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
        static void GIFCloseFile(void *pHandle);
        static int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition);
    private: 
        static MatrixPanel_I2S_DMA *_dma_display;
        static AnimatedGIF _gif;
        static File _f;
        static int x_offset;
        static int y_offset;
};

#endif










