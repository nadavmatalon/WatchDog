
# WatchDog

[![Platform Badge](https://img.shields.io/badge/platform-Arduino-orange.svg)](https://www.arduino.cc/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![SemVer](https://img.shields.io/badge/SemVer-1.0.0-brightgreen.svg)](http://semver.org/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## INTRODUCTION

Many AVR's include an 'Enhanced Watchdog Timer' (WDT) which runs independently on a separate on-chip 128KHz oscilliator. The Watchdog Timer is typically used for pre-defined system reset duties (i.e. as a fail-safe or protection mechanism against system crashes). However, given that it has its own interrupt vector, the WatchDog Timer may also be used as a regular (albeit relatively restricted) time counter for managing a user-defined Interrupt Service Routine (ISR).

Taking advantage of the above characteristics of the WatchDog Timer, the present light-weight library contains a small collection of functions for controlling this Timer in its capacity as an interrupt generator. In other words, the user can select the overflow period (in mS) of the timer, as well as compose the content of the ISR (Interrupt Service Routine) which is set to run on each overflow event (e.g. getting a sensor reading every 500mS or blinking a LED at the rate of once per second).

Importantly, behind the scenes, the 'reset' capabilities of the WatchDog timer are entirely disabled by the library, leaving only the overflow time-counting mechanism and corresponding interrupt service routine to run as they normally do. Hence, no system reset is preformed when using any of the library's functions.

It is also worth mentioning that in order to keep the memory footprint of the library to a minimum, no objects are instantiated during its operation and only one variable is stored in the system (namely: the timer's selected overflow period).

For more details about the WatchDog Timer, see the [ATmega328P DATASHEET](http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_datasheet.pdf)

## Repository Contents

- **WatchDog.h** - Library Header file.  
- **WatchDog.cpp** - Library Compilation file.  
- **/examples**   
  - **/WatchDog_Example**  
    - **WatchDog_Example.ino** - illustration code for using the WatchDog Timer library.  
- **/extras**
  - **License.txt** - A cope of the end-user license agreement.  
- **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE.  
- **library.properties** - General library properties for the Arduino's IDE (>1.5) Library Package Manager.  
- **README.md** - The readme file for this library.  
- **library.json** - JSON file for the Arduino's IDE (>1.5) Package Manager.  


## GENERAL NOTES

1) __Conflict with Delay()__

Like any other time-dependant element, use of [delay()](https://www.arduino.cc/en/Reference/Delay), [delayMicroseconds()](https://www.arduino.cc/en/Reference/DelayMicroseconds) and the like is fundamentally incompatible with the normal, on-going operation of the WatchDog Timer and should therefore be avoided (apart from minimal and necessay exceptions where the cost in terms of time-keeping accuracy can be reasonably tolerated).

2) __Limited Speed & Accuracy__

Although the WatchDog Timer, as operated by the current library, may have noumerous potential applications, its speed and accuracy are relatively limited and therefore it is not advisable to use this Timer for ultra-fast, time-critical missions.


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include "WatchDog.h"
```

With the library installed & included in the sketch, the following functions can be used directly to manage the WatchDog Timer:

>Note that using these function requires preceding them with the library name (see the [usage example](https://github.com/nadavmatalon/WatchDog/blob/master/examples/WatchDog_Example/WatchDog_Example.ino) sketch for a more detailed implementation):


## LIBRARY FUNCTIONS

__init();__  
Parameters:&nbsp;&nbsp;&nbsp;ISR_function_name, ovf_period_t (2nd argument is optional)    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1) Note that there are no parenthases follwing the ISR_function_name  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2) ovf_period_t params: OVF_16MS   / OVF_32MS   / OVF_64MS   / OVF_125MS  / OVF_250MS / OVF_500MS  / OVF_1000MS / OVF_2000MS / OVF_4000MS / OVF_8000MS  
Description:&nbsp;&nbsp;&nbsp;Initializes the WatchDog timer with a user-defined ISR function name and (optionally) overflow period [default: 1000mS]. Place this function in the setup() section of the sketch. Note that this function also automatically starts the WatchDog Timer.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__start();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Starts the WatchDog Timer (or Se-starts it if the timer was previously stopped).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None 

__stop();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Stops the WatchDog Timer.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None 

__status();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current status of the WatchDog Timer (0 = Stopped / 1 = Running).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte   

__getPeriod();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current overflow period of the WatchDog Timer (16 = OVF_16MS / 32 = OVF_32MS / 64 = OVF_64MS / 125 = OVF_125MS / 250 = OVF_250MS / 500 = OVF_500MS / 1000 = OVF_1000MS / 2000 = OVF_2000MS / 4000 = OVF_4000MS / 8000 = OVF_8000MS).    
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int     

__setPeriod();__  
Parameters:&nbsp;&nbsp;&nbsp;ovf_period_t (OVF_16MS / OVF_32MS / OVF_64MS / OVF_125MS / OVF_250MS / OVF_500MS  / OVF_1000MS / OVF_2000MS / OVF_4000MS / OVF_8000MS)  
Description:&nbsp;&nbsp;&nbsp;Sets the current overflow period of the WatchDog Timer.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None     

## RUNNING THE EXAMPLE SKETCH

1) Start the Arduino IDE and open the example sketch  
2) Upload the sketch to the Arduino  
3) Open the Serial Communications Window (make sure the baud-rate is set to 9600 or change it in the sketch to match your Serial Port's buad-rate) amd follow the on-screen instructions  

## BUG REPORTS

Please report any issues/bugs/suggestions at the [Issues](https://github.com/nadavmatalon/WatchDog/issues) section of this Github repository.

## TODO

## VERSION HISTORY

__Ver. 1.0.0__ - First release (16.11.16)  

## LICENSE

[The MIT License (MIT)](https://opensource.org/licenses/MIT)
Copyright (c) 2016 Nadav Matalon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


