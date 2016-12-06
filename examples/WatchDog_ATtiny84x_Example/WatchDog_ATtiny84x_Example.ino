/* 
    WATCHDOG LIBRARY - ATTINY84x EXAMPLE
    ------------------------------------

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

    For more details about the WatchDog Timer see relevant AVR datasheet.

    HW SETUP
    -------- 
    The minimal hardware setup for running this example includes the following connections to the ATtiny84A / ATtiny841:
    - PIN 1 (VCC)  - Connect to 5V source
    - PIN 10 (PA3) - Connect to LED anode (with LED's cathode going to GND via appropriate resistor)
      [Note that physical PIN 10 (PA3) on the ATtiny84A is referred to as Pin 7 in the Arduino mapping]
    - PIN 14 (GND) - Connect to Common-Ground
    - DECOUPLING   - Connect a 100nF ceramic capacitor between VCC and GND of the ATtiny

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

const byte LED_PIN = 7;                                     // Physical Pin 10 (PA3)

unsigned long timeNow;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    WatchDog::init(blinkISR, OVF_500MS);
    timeNow = millis();
}

void loop() {
    if (millis() - timeNow >= 3000) {
        (WatchDog::getPeriod() == 500) ? WatchDog::setPeriod(OVF_250MS) : WatchDog::setPeriod(OVF_500MS);
        timeNow = millis();
    }    
}

void blinkISR() {                                                       // watchdog timer interrupt service routine
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}