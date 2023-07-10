/**
 * @file debug.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define DEBUG true

#if DEBUG == true
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#endif