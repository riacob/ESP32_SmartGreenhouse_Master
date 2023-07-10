/**
 * @file greenhouse.h
 * @author Riccardo Iacob
 * @brief Contains all data regarding the greenhouse and its configuration
 * @version 0.1
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <Arduino.h>

namespace Greenhouse
{
    // data received from greenhouse
    struct Data_s {
        float temp1;
        float temp2;
        float temp3;
        float hum1;
        float hum2;
        float hum3;
    };
    // data to be sent to greenhouse
    struct Config_s {
        bool test;
    };
    Data_s data;
    Config_s config;
    void loadDummyData() {
        data.temp1 = random(2200,2400)/100.0;
        data.temp2 = random(2200,2400)/100.0;
        data.temp3 = random(2200,2400)/100.0;
        data.hum1 = random(7400,7600)/100.0;
        data.hum2 = random(7400,7600)/100.0;
        data.hum3 = random(7400,7600)/100.0;
    }
};

#endif