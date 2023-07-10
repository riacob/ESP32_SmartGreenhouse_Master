/**
 * @file tfthelper.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef TFTHELPER_H
#define TFTHELPER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <xbmparser.h>
#include "greenhouse.h"
#include "rtchelper.h"
#define TFT_GREY 0x5AEB

namespace TFT
{

    TFT_eSPI tft = TFT_eSPI();

    // treating tft as a fsm
    enum class TFTStates
    {
        IDLE,
        CONFIG,
        TFT_CALIBRATION
    };

    // does the tft need to be updated?
    bool touchPressed = false;
    bool newData = false;
    // current state of the tft
    TFTStates stateCurrent = TFTStates::IDLE;
    // x and y touch positions
    uint16_t touchx = 0;
    uint16_t touchy = 0;
    // Timer for touch debouncing
    long last_ms = millis();

    void doSetup();
    void setState(TFTStates screen);
    void IRAM_ATTR touchISR();
    void doTick();
    void resetTouch();

    // initialize tft and show idle screen
    void doSetup()
    {
        uint16_t calData[5] = {338,3387,343,3489,4};
        tft.setTouch(calData);
        attachInterrupt(22, touchISR, CHANGE);
        tft.init();
        tft.setRotation(0);
        setState(TFTStates::IDLE);
        debugln("[tfthelper.h] setup completed");
    }

    // touch interrupt
    void IRAM_ATTR touchISR()
    {
        touchPressed = true;
    }

    // loop function for handling tft updates
    void doTick()
    {
        if (touchPressed)
        {
            // Debouncing of touch position
            if ((millis() - last_ms >= 500) || (touchx <= 0 && touchy <= 0))
            {
                tft.getTouch(&touchx, &touchy);
                last_ms = millis();
            }
            debug("[tfthelper.h] touch pressed at ");
            debug(touchx);
            debug(" ");
            debugln(touchy);
            // Refresh the screen
            setState(stateCurrent);
            touchPressed = false;
        }
        if (newData)
        {
            debugln("[tfthelper.h] new data available");
            // Refresh the screen if in idle state (homepage)
            if (stateCurrent == TFTStates::IDLE) {
                setState(stateCurrent);
            }
            newData = false;
        }
    }

    void resetTouch()
    {
        touchx = 0;
        touchy = 0;
    }

    // set finite state of tft
    void setState(TFTStates screen)
    {
        stateCurrent = screen;
        debug("[tfthelper.h] screen set to ");
        switch (screen)
        {
        case TFTStates::IDLE:
        {
            debugln("IDLE");
            // temporary UI
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setCursor(0, 0, 2);
            tft.setTextSize(3);
            tft.print("Temp1: ");
            tft.println(Greenhouse::data.temp1);
            tft.print("Temp2: ");
            tft.println(Greenhouse::data.temp2);
            tft.print("Temp3: ");
            tft.println(Greenhouse::data.temp3);
            tft.print("Hum1: ");
            tft.println(Greenhouse::data.hum1);
            tft.print("Hum2: ");
            tft.println(Greenhouse::data.hum2);
            tft.print("Hum3: ");
            tft.println(Greenhouse::data.hum3);
            RTC::readTime();
            tft.setTextSize(1);
            tft.print("Last update: ");
            tft.print(RTC::currentTimeString);
            tft.print(" ");
            tft.println(RTC::currentDateString);
            // temporary touch mapping to button
            tft.drawRect(200, 350, 100, 100, TFT_WHITE);
            tft.setCursor(210, 370, 2);
            tft.setTextSize(2);
            tft.print("Config");
            if (touchx > 200 && touchx < 300 && touchy > 350 && touchy < 450)
            {
                resetTouch();
                debugln("[tfthelper.h] IDLE:CONFIG pressed");
                setState(TFTStates::CONFIG);
            }
            break;
        }
        case TFTStates::CONFIG:
        {
            debugln("CONFIG");
            // temporary UI
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setCursor(0, 0, 2);
            tft.setTextSize(3);
            tft.println("Configuration page");
            // temporary touch mapping to button
            tft.drawRect(200, 350, 100, 100, TFT_WHITE);
            tft.setCursor(210, 370, 2);
            tft.setTextSize(2);
            tft.print("Back");
            if (touchx > 200 && touchx < 300 && touchy > 350 && touchy < 450)
            {
                resetTouch();
                debugln("[tfthelper.h] CONFIG:BACK pressed");
                setState(TFTStates::IDLE);
            }
            break;
        }
        case TFTStates::TFT_CALIBRATION:
        {
            uint16_t calData[5];
            uint8_t calDataOK = 0;
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(20, 0);
            tft.setTextFont(2);
            tft.setTextSize(1);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.println("Touch corners as indicated");
            tft.setTextFont(1);
            tft.println();
            tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
            debug("[tfthelper.h] calibration values { ");
            for (uint8_t i = 0; i < 5; i++)
            {
                debug(calData[i]);
                if (i < 4)
                    debug(", ");
            }
            debugln(" };");
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.println("Calibration complete!");
            tft.println("Calibration code sent to Serial port.");
        }
        }
    }
};

#endif