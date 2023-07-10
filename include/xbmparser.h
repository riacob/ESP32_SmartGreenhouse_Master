#ifndef XBMPARSER_H
#define XBMPARSER_H

#include "Arduino.h"
#include "TFT_eSPI.h"

class XBMParser {
    static void printXBitmapFile(TFT_eSPI* tft, char* filePath, int16_t x, int16_t y, uint16_t fgcolor)
    {
        int16_t w = 0;
        int16_t h = 0;
        uint8_t* xbm = new uint8_t[16];
        tft->drawXBitmap(x,y,xbm,w,h,fgcolor);
        delete xbm;
    }
};

#endif