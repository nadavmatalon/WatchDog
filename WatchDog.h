/*==============================================================================================================*

    @file     WatchDog.h
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon

    WatchDog Timer Functions

    Ver. 1.0.0 - First release (16.11.16)

 *===============================================================================================================*
    INTRODUCTION
 *===============================================================================================================*

    
 *===============================================================================================================*
    BUG REPORTS
 *===============================================================================================================*

    Please report any bugs/issues/suggestions at the Github Repo of this library at: 
    https://github.com/nadavmatalon/WatchDog
 
 *===============================================================================================================*
    LICENSE
 *===============================================================================================================*

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

#ifndef WatchDog_h
#define WatchDog_h

#if !defined(ARDUINO_ARCH_AVR)
#error “The WatchDog library only supports AVR processors.”
#endif

#include <Arduino.h>

namespace Watchdog {
                            //    7        6         5         4        3        2          1          0
    typedef enum:byte {     //   WDIF     WDIE     WDP[3]     WDCE     WDE     WDP[2]     WDP[1]     WDP[0]
        OVF_16MS   = 64,    //    0        1         0         0        0        0          0          0
        OVF_32MS   = 65,    //    0        1         0         0        0        0          0          1
        OVF_64MS   = 66,    //    0        1         0         0        0        0          1          0
        OVF_125MS  = 67,    //    0        1         0         0        0        0          1          1
        OVF_250MS  = 68,    //    0        1         0         0        0        0          0          0
        OVF_500MS  = 69,    //    0        1         0         0        0        1          0          0
        OVF_1000MS = 70,    //    0        1         0         0        0        1          0          1
        OVF_2000MS = 71,    //    0        1         0         0        0        1          1          1
        OVF_4000MS = 96,    //    0        1         1         0        0        0          0          0
        OVF_8000MS = 97     //    0        1         1         0        0        0          0          1
    } ovf_period_t;

    class WatchDog {
        public:
            static void init(void (*isrFunc)(), ovf_period_t _ovf_period);
            static void start();
            static void stop();
            static byte status();
            static unsigned int getPeriod();
            static void setPeriod(ovf_period_t newPeriod);
            void   attachInterrupt(void (*isr)());
            void   attachInterrupt(void (*isr)(), ovf_period_t selectedPeriod);
            static void (*isrCallback)();
            static void isrDefaultUnused();
        private:
            static ovf_period_t ovfPeriod;
    };
}

using namespace Watchdog;

extern WatchDog Wdog1;

#endif
   //  B   //  B    0        1         0         0        0        0          0          0