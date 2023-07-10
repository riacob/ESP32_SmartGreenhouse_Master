/**
 * @file xbmparser.h
 * @author Riccardo Iacob
 * @brief XBitMap file parser, allows user to print icons on TFT screen
 * @version 0.1
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef XBMPARSER_H
#define XBMPARSER_H

#include "Arduino.h"
#include "TFT_eSPI.h"
#include "sdhelper.h"

namespace XBMParser
{
    // automatic w/h
    void printXBitmapFile(TFT_eSPI *tft, char *filePath, int16_t x, int16_t y, uint16_t fgcolor)
    {
        int16_t w = 0;
        int16_t h = 0;
        uint8_t *xbm = new uint8_t[16];
        tft->drawXBitmap(x, y, xbm, w, h, fgcolor);
        delete xbm;
    }
    // manual w/h
    void printXBitmapFile(TFT_eSPI *tft, char *filePath, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fgcolor)
    {
        uint8_t *xbm = new uint8_t[16];
        tft->drawXBitmap(x, y, xbm, w, h, fgcolor);
        delete xbm;
    }
};

#endif