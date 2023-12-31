/**
 * @file globals.h
 * @author Riccardo Iacob
 * @brief Contains all the data regarding the device configuration
 * @version 0.1
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

namespace Globals {
    long pollDelay = 5000;
    uint32_t baudrate = 115200;
};

#endif