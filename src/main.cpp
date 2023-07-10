/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <xbmparser.h>
#include "debug.h"
#include "tfthelper.h"
#include "radiohelper.h"
#include "sdhelper.h"
#include "greenhouse.h"
#include "globals.h"
#include "rtc.h"

void setup(void)
{
    Serial.begin(Globals::baudrate);
    TFT::doSetup();
    Radio::doSetup();
    SDCard::doSetup();
    RTC::doSetup();
    //Radio::last_ms = millis();
    debugln("[main.cpp] setup completed");
}

void loop()
{
  TFT::doTick();
  Radio::doTick();
}
