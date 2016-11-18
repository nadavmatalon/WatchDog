/*==============================================================================================================*
 
    @file     WatchDog.cpp
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon

    WatchDog Timer Functionality

    Ver. 1.0.0 - First release (17.11.16)

 *==============================================================================================================*
    LICENSE
 *==============================================================================================================*
 
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
 
 *==============================================================================================================*/

#if 1
__asm volatile ("nop");
#endif

#include "WatchDog.h"

WatchDog Wdog1;                                                 // preinstatiate for watcdog ISR function

ovf_period_t WatchDog::ovfPeriod;                               // class variable for storing selected ovf period

/*==============================================================================================================*
    INITIALIZE
 *==============================================================================================================*/
//  ISR PARAMS: User-defined name of WatchDog Timer ISR function (without parenthesis)
//  PERIOD PARAMS (OPTIONAL): OVF_16MS   / OVF_32MS   / OVF_64MS   / OVF_125MS  / OVF_250MS /
//                            OVF_500MS  / OVF_1000MS / OVF_2000MS / OVF_4000MS / OVF_8000MS

void WatchDog::init(void (*isrFunc)(), ovf_period_t _ovfPeriod = OVF_1000MS)  {
    WatchDog::ovfPeriod = _ovfPeriod;                           // store selected overflow period value
    noInterrupts();                                             // disable global interrupts (cli);
        bitClear(MCUSR, WDRF);                                  // Clear WDRF in MCUSR
        WDTCSR = (1 << WDCE) | (1 << WDE);                      // enter watchdog timer configuration mode
        WDTCSR = WatchDog::ovfPeriod;                           // set watchdog timer overflow period
        bitSet(MCUSR, WDIF);                                    // clear Watchdog Timer Interrupt Flag
    interrupts();                                               // enable global interrupts (sei);
    Wdog1.attachInterrupt(isrFunc);
}

/*==============================================================================================================*
    START WATCHDOG TIMER
 *==============================================================================================================*/

void WatchDog::start() {
    noInterrupts();                                             // disable global interrupts (cli);
        bitClear(MCUSR, WDRF);                                  // Clear WDRF in MCUSR
        WDTCSR = (1 << WDCE) | (1 << WDE);                      // enter watchdog timer configuration mode
        WDTCSR = WatchDog::ovfPeriod;                           // turn on WatchDog timer
    interrupts();                                               // enable global interrupts (sei);
}

/*==============================================================================================================*
    STOP WATCHDOG TIMER
 *==============================================================================================================*/

void WatchDog::stop() {
    noInterrupts();                                             // disable global interrupts (cli);
        bitClear(MCUSR, WDRF);                                  // Clear WDRF in MCUSR
        WDTCSR = (1 << WDCE) | (1 << WDE);                      // enter watchdog timer configuration mode
        WDTCSR = 0;                                             // turn off WatchDog timer
    interrupts();                                               // enable global interrupts (sei);
}

/*==============================================================================================================*
    WATCHDOG STATUS (0 = STOPPED / 1 = RUNNING)
 *==============================================================================================================*/

byte WatchDog::status() {
    return WDTCSR ? 1 : 0;                                      // check value of WDTCSR register
}

/*==============================================================================================================*
    GET WATCHDOG OVERFLOW PERIOD
    ( 16 = OVF_16MS  /   32 = OVF_32MS   /   64 = OVF_64MS   /  125 = OVF_125MS  /  250 = OVF_250MS /
     500 = OVF_500MS / 1000 = OVF_1000MS / 2000 = OVF_2000MS / 4000 = OVF_4000MS / 8000 = OVF_8000MS)
 *==============================================================================================================*/

unsigned int WatchDog::getPeriod() {
    switch (WatchDog::ovfPeriod) {
        case (OVF_16MS)  : return   16; break;
        case (OVF_32MS)  : return   32; break;
        case (OVF_64MS)  : return   64; break;
        case (OVF_125MS) : return  125; break;
        case (OVF_250MS) : return  250; break;
        case (OVF_500MS) : return  500; break;
        case (OVF_1000MS): return 1000; break;
        case (OVF_2000MS): return 2000; break;
        case (OVF_4000MS): return 4000; break;
        case (OVF_8000MS): return 8000; break;
    }
}

/*==============================================================================================================*
    SET WATCHDOG OVERFLOW PERIOD
    //  PARAMS: OVF_16MS   / OVF_32MS   / OVF_64MS   / OVF_125MS  / OVF_250MS /
    //          OVF_500MS  / OVF_1000MS / OVF_2000MS / OVF_4000MS / OVF_8000MS
 *==============================================================================================================*/

void WatchDog::setPeriod(ovf_period_t newPeriod) {
    WatchDog::ovfPeriod = newPeriod;                                // store new overflow period value
    WatchDog::status() ? WatchDog::start() : WatchDog::stop();
}

/*==============================================================================================================*
    CUSTOM WATCHDOG OVERFLOW INTERRUPT SERVICE ROUTINE FUNCTIONS
 *==============================================================================================================*/

void WatchDog::attachInterrupt(void (*isr)()) {
    isrCallback = isr;
}

void WatchDog::attachInterrupt(void (*isr)(), ovf_period_t selectedPeriod) {
    attachInterrupt(isr);
}

void (*WatchDog::isrCallback)() = WatchDog::isrDefaultUnused;

void WatchDog::isrDefaultUnused() {}

/*==============================================================================================================*
    WATCHDOG INTERRUPT SERVICE ROUTINE
 *==============================================================================================================*/

ISR(WDT_vect) {
    Wdog1.isrCallback();
    bitSet(MCUSR, WDIF);                                            // clear Watchdog Timer Interrupt Flag
}
