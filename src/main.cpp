/**
 * @file main.cpp
 * @author Riccardo Iacob
 * @brief Program entry point
 * @version 0.1
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "debug.h"
#include "tfthelper.h"
#include "radiohelper.h"
#include "greenhouse.h"
#include "globals.h"
#include "rtc.h"

void setup(void)
{
    Serial.begin(Globals::baudrate);
    TFT::doSetup();
    Radio::doSetup();
    RTC::doSetup();
    debugln("[main.cpp] setup completed");
}

void loop()
{
  TFT::doTick();
  Radio::doTick();
}
