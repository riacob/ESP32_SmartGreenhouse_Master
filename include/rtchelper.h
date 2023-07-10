/**
 * @file rtchelper.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef RTCHELPER_H
#define RTCHELPER_H

#include <Arduino.h>

namespace RTC
{
    tm currentDateTime;
    String currentTimeString;
    String currentDateString;

    void doSetup();
    void doTick();
    void readTime();

    void doSetup()
    {
        // initialize ds3231
        // read data
    }

    // Update time from NTP servers?
    void doTick()
    {
    }

    void readTime()
    {
        // read data from ds3231
        // prepare formatted date and time strings
        currentTimeString = "";
        currentDateString = "";
        // time hh:mm:ss
        if (currentDateTime.tm_hour < 10)
        {
            currentTimeString.concat("0");
        }
        currentTimeString.concat(currentDateTime.tm_hour);
        currentTimeString.concat(":");
        if (currentDateTime.tm_min < 10)
        {
            currentTimeString.concat("0");
        }
        currentTimeString.concat(currentDateTime.tm_min);
        currentTimeString.concat(":");
        if (currentDateTime.tm_sec < 10)
        {
            currentTimeString.concat("0");
        }
        currentTimeString.concat(currentDateTime.tm_sec);
        // date dd:mm:yyyy
        if (currentDateTime.tm_mday < 10)
        {
            currentDateString.concat("0");
        }
        currentDateString.concat(currentDateTime.tm_mday);
        currentDateString.concat("/");
        if (currentDateTime.tm_mon < 10)
        {
            currentDateString.concat("0");
        }
        currentDateString.concat(currentDateTime.tm_mon);
        currentDateString.concat("/");
        currentDateString.concat(currentDateTime.tm_year + 1900);
    }
};

#endif