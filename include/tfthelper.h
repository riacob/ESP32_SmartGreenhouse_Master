/**
 * @file tfthelper.h
 * @author Riccardo Iacob
 * @brief Handles the TFT display
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
#include "greenhouse.h"
#include "rtchelper.h"
#include "buttonwidget.h"
#include "globals.h"
#include "icons.h"
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
        uint16_t calData[5] = {338, 3387, 343, 3489, 4};
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
            if (stateCurrent == TFTStates::IDLE)
            {
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
            tft.fillScreen(TFT_WHITE);

            ButtonWidget* btnConfig = new ButtonWidget(&touchx, &touchy, &tft, 200, 350, 63, 63);
            ButtonWidget* btnTemp1 = new ButtonWidget(&touchx, &touchy, &tft, 20, 0, 63, 63);
            ButtonWidget* btnTemp2 = new ButtonWidget(&touchx, &touchy, &tft, 20, 70, 63, 63);
            ButtonWidget* btnTemp3 = new ButtonWidget(&touchx, &touchy, &tft, 20, 135, 63, 63);
            ButtonWidget* btnHum1 = new ButtonWidget(&touchx, &touchy, &tft, 170, 0, 63, 63);
            ButtonWidget* btnHum2 = new ButtonWidget(&touchx, &touchy, &tft, 170, 70, 63, 63);
            ButtonWidget* btnHum3 = new ButtonWidget(&touchx, &touchy, &tft, 170, 135, 63, 63);

            btnConfig->setStyle(TFT_CYAN, TFT_BLACK, ButtonWidget::ButtonStyles::ELLIPSE);
            btnTemp1->setStyle(TFT_WHITE, TFT_RED, ButtonWidget::ButtonStyles::ROUND_RECT);
            btnTemp2->setStyle(TFT_WHITE, TFT_RED, ButtonWidget::ButtonStyles::ROUND_RECT);
            btnTemp3->setStyle(TFT_WHITE, TFT_RED, ButtonWidget::ButtonStyles::ROUND_RECT);
            btnHum1->setStyle(TFT_WHITE, TFT_BLUE, ButtonWidget::ButtonStyles::ROUND_RECT);
            btnHum2->setStyle(TFT_WHITE, TFT_BLUE, ButtonWidget::ButtonStyles::ROUND_RECT);
            btnHum3->setStyle(TFT_WHITE, TFT_BLUE, ButtonWidget::ButtonStyles::ROUND_RECT);

            btnConfig->setIcon(ICONS_63X63::cog);
            btnTemp1->setIcon(ICONS_63X63::thermometer);
            btnTemp2->setIcon(ICONS_63X63::thermometer);
            btnTemp3->setIcon(ICONS_63X63::thermometer);
            btnHum1->setIcon(ICONS_63X63::humidity);
            btnHum2->setIcon(ICONS_63X63::humidity);
            btnHum3->setIcon(ICONS_63X63::humidity);

            btnTemp1->setTooltip("Temp1", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnTemp1->getSizeX() / 4, TFT_PURPLE);
            btnTemp2->setTooltip("Temp2", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnTemp2->getSizeX() / 4, TFT_PURPLE);
            btnTemp3->setTooltip("Temp3", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnTemp3->getSizeX() / 4, TFT_PURPLE);
            btnHum1->setTooltip("Hum1", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnHum1->getSizeX() / 4, TFT_PURPLE);
            btnHum2->setTooltip("Hum2", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnHum2->getSizeX() / 4, TFT_PURPLE);
            btnHum3->setTooltip("Hum3", 2, 1, ButtonWidget::TooltipPositions::RIGHT, btnHum3->getSizeX() / 4, TFT_PURPLE);

            btnConfig->draw();
            btnTemp1->draw();
            btnTemp2->draw();
            btnTemp3->draw();
            btnHum1->draw();
            btnHum2->draw();
            btnHum3->draw();

            if (btnConfig->isPressed())
            {
                resetTouch();
                debugln("[tfthelper.h] IDLE:CONFIG pressed");
                setState(TFTStates::CONFIG);
            }

            delete btnConfig;
            delete btnTemp1;
            delete btnTemp2;
            delete btnTemp3;
            delete btnHum1;
            delete btnHum2;
            delete btnHum3;


            break;
        }
        case TFTStates::CONFIG:
        {
            debugln("CONFIG");

            tft.fillScreen(TFT_BLACK);

            ButtonWidget* btnBack = new ButtonWidget(&touchx, &touchy, &tft, 200, 350, 100, 100);
            ButtonWidget* btnConfig = new ButtonWidget(&touchx, &touchy, &tft, 100, 350, 100, 100);

            btnBack->setStyle(TFT_PURPLE, TFT_WHITE, ButtonWidget::ButtonStyles::ROUND_RECT, 15);

            btnBack->setText("Back", 2, 1);
            btnConfig->setText("Spinbox",2,1);

            btnBack->draw();
            btnConfig->draw();

            if (btnBack->isPressed())
            {
                resetTouch();
                debugln("[tfthelper.h] CONFIG:IDLE pressed");
                setState(TFTStates::IDLE);
            }

            delete btnBack;
            delete btnConfig;

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
            break;
        }
        }
    }
};

#endif