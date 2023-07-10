/**
 * @file radiohelper.h
 * @author Riccardo Iacob
 * @brief Handles the CC1101 radio module
 * @version 0.1
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef RADIOHELPER_H
#define RADIOHELPER_H

#include <Arduino.h>
#include "debug.h"
#include "globals.h"
#include "tfthelper.h"

namespace Radio
{
    // Timer for periodic readings
    long last_ms = millis();
    // If the config was modified by the user
    bool configModified = false;

    void doSetup();
    void doTick();

    void doSetup()
    {
    }

    void IRAM_ATTR handleISR() {}
    // tick function requests data from greenhouse and posts config data to greenhouse
    void doTick()
    {
        if (millis() - last_ms >= Globals::pollDelay)
        {
            debugln("[radiohelper.h] polling for new data");
            // debug only, generate fake values
            Greenhouse::loadDummyData();
            TFT::newData = true;
            last_ms= millis();
        }
        if (configModified)
        {
            configModified = false;
        }
    }
};

#endif