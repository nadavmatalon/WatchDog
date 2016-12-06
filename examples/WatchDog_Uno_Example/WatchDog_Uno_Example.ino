/* 
    WATCHDOG LIBRARY - ARDUINO UNO EXAMPLE
    --------------------------------------

    INTRODUCTION
    ------------ 

    Many AVR's include an 'Enhanced Watchdog Timer' (WDT) which runs independently on a separate on-chip 128KHz oscilliator.
    The Watchdog Timer is typically used for pre-defined system reset duties (i.e. as a fail-safe or protection mechanism
    against system crashes). However, given that it has its own interrupt vector, the WatchDog Timer may also be used
    as a regular (albeit relatively restricted) time counter for managing a user-defined Interrupt Service Routine (ISR).

    Taking advantage of the above characteristics of the WatchDog Timer, the present light-weight library contains a small
    collection of functions for controlling this Timer in its capacity as an interrupt generator. In other words, the user
    can select the overflow period (in mS) of the timer, as well as compose the content of the ISR (Interrupt Service Routine)
    which is set to run on each overflow event (e.g. getting a sensor reading every 500mS or blinking a LED at the rate of
    once per second).

    Importantly, behind the scenes, the 'reset' capabilities of the WatchDog timer are entirely disabled by the library,
    leaving only the overflow time-counting mechanism and corresponding interrupt service routine to run as they normally do.
    Hence, no system reset is preformed when using any of the library's functions.

    It is also worth mentioning that in order to keep the memory footprint of the library to a minimum, no objects are
    instantiated during its operation and only one variable is stored in the system (namely: the WatchDog Timer's selected
    overflow period).

    For more details about the WatchDog Timer, see the ATmega328P DATASHEET:
    http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_datasheet.pdf

    HW SETUP & RUNNING EXAMPLE
    --------------------------
    There is no need for any external components in order to run this sketch as it uses the built-in LED on Pin 13 of
    the Arduino.

    To run this exmple, simply upload the sketch to the Arduino, open the Seria Monitor (set the baud-rate to 9600 if needed),
    and follow the on-screen instructions.

    BUG REPORTS
    -----------
    Please report any bugs/issues/suggestions in the Github Repository of this library at:
    https://github.com/nadavmatalon/WatchDog/issues

    LICENSE
    -------
    The MIT License (MIT)
    Copyright (c) 2016 Nadav Matalon

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the Software without restriction, including without
    limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
    LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "WatchDog.h"

const byte LED_PIN = 13;

int cmd;

unsigned long timeNow = 0;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WatchDog::init(blinkISR, 500);
    printMenu();
}
 
void loop() {

    if (millis() - timeNow >= 750) {
        Serial.println(timeNow);
        timeNow = millis();
    }
  
    if (Serial.available() > 0) {
        cmd = Serial.read();
        if (cmd == '0') {
            if (WatchDog::status()) {
                WatchDog::stop();
                Serial.print(F("\n\nwatchdog stopped\n\n"));
            } else {
                Serial.print(F("\n\nwatchdog already stopped\n\n"));   
            }
        } else if (cmd == '1') {
            if (WatchDog::status() && WatchDog::getPeriod()) {
                Serial.print(F("\n\nwatchdog already running\n\n"));               
            } else if (!WatchDog::status() && !WatchDog::getPeriod()) { 
                Serial.print(F("\n\nwatchdog not started (invalid overflow period)\n\n"));        
            } else {
                WatchDog::start();
                Serial.print(F("\n\nwatchdog re-started\n\n"));
            }
        } else if (cmd == '2') {
            Serial.print(WatchDog::status() ? (F("\n\nwatchdog running\n\n")) : (F("\n\nwatchdog stopped\n\n")));
        } else if (cmd == '3') {
            Serial.print(F("\n\nwatchdog overflow period: "));
            Serial.print(WatchDog::getPeriod());
            Serial.print(F("mS\n\n"));
        } else if (cmd == '4') {
            WatchDog::setPeriod(OVF_250MS);
            Serial.print(F("\n\nwatchdog overflow period set to: 250mS\n\n"));
        } else if (cmd == '5') {
            WatchDog::setPeriod(OVF_500MS);
            Serial.print(F("\n\nwatchdog overflow period set to: 500mS\n\n"));
        } else if (cmd == '6') {
            WatchDog::setPeriod(OVF_1000MS);
            Serial.print(F("\n\nwatchdog overflow period set to: 1000mS\n\n"));
        } else if ((cmd == 'm') || (cmd == 'M')) {
            printMenu();
        }
    }
}
  
void blinkISR() {                                         // watchdog timer interrupt service routine    
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void printMenu() {
    Serial.print(F("\n\nWatchDog Timer Functions:                                    \
                    \nSend 0 to Stop WatchDog Timer                                  \
                    \nSend 1 to (Re-)Start WatchDog Timer                            \
                    \nSend 2 for WatchDog Timer Status                               \
                    \nSend 3 for WatchDog Timer Overflow Period                      \
                    \nSend 4 to Set WatchDog Timer Overflow Period to  250mS         \
                    \nSend 5 to Set WatchDog Timer Overflow Period to  500mS         \
                    \nSend 6 to Set WatchDog Timer Overflow Period to 1000mS         \
                    \nSend M to Re-Print This Menu                                   \
                    \n(Running numbers show elapsed time in mS to indicate no reset) \
                    \n\n"));
}
