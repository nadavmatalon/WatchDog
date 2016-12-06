
# WatchDog

[![Platform Badge](https://img.shields.io/badge/platform-Arduino-orange.svg)](https://www.arduino.cc/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![SemVer](https://img.shields.io/badge/SemVer-1.2.0-brightgreen.svg)](http://semver.org/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## INTRODUCTION

Many AVR's include an 'Enhanced Watchdog Timer' (WDT) which runs independently on a separate on-chip 128KHz oscilliator. The Watchdog Timer is typically used for pre-defined system reset duties (i.e. as a fail-safe or protection mechanism against system crashes). However, given that it has its own interrupt vector, the WatchDog Timer may also be used as a regular (albeit relatively restricted) time counter for managing a user-defined Interrupt Service Routine (ISR).

Taking advantage of the above characteristics of the WatchDog Timer, the present light-weight library contains a small collection of functions for controlling this Timer in its capacity as an interrupt generator. In other words, the user can select the overflow period (in mS) of the timer, as well as compose the content of the ISR (Interrupt Service Routine) which is set to run on each overflow event (e.g. getting a sensor reading every 500mS or blinking a LED at the rate of once per second).

Importantly, behind the scenes, the 'reset' capabilities of the WatchDog timer are entirely disabled by the library, leaving only the overflow time-counting mechanism and corresponding interrupt service routine to run as they normally do. Hence, no system reset is preformed when using any of the library's functions.

It is also worth mentioning that in order to keep the memory footprint of the library to a minimum, no objects are instantiated during its operation and only one variable is stored in the system (namely: the timer's selected overflow period).

For more details about the WatchDog Timer, see the relevant datasheet

## Repository Contents

- **WatchDog.h** - Library Header file.  
- **WatchDog.cpp** - Library Compilation file.  
- **/examples**   
  - **/WatchDog_Uno_Example**  
    - **WatchDog_Uno_Example.ino** - illustration code for using the WatchDog Timer library running on an Arduino Uno with a user-interface via the Serial Monitor.  
  - **/WatchDog_ATtiny84x_Example**  
    - **WatchDog_ATtiny84x_Example.ino** - illustration code for using the WatchDog Timer library running on an ATtiny441/841 or an ATtiny84A with feedback via a blinking LED.  
- **/extras**
  - **License.txt** - A cope of the end-user license agreement.  
- **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE.  
- **library.properties** - General library properties for the Arduino's IDE (>1.5) Library Package Manager.  
- **README.md** - The readme file for this library.  
- **library.json** - JSON file for the Arduino's IDE (>1.5) Package Manager.  


## GENERAL NOTES

1) __Conflict with Arduino's Delay Functions__

Like any other time-dependant element, use of [delay()](https://www.arduino.cc/en/Reference/Delay), [delayMicroseconds()](https://www.arduino.cc/en/Reference/DelayMicroseconds) and the like is fundamentally incompatible with the normal, on-going operation of the WatchDog Timer and should therefore be avoided (apart from minimal and necessay exceptions where the cost in terms of time-keeping accuracy can be reasonably tolerated).

2) __Timer's Limited Speed & Accuracy__

Although the WatchDog Timer, as operated by the current library, may have noumerous potential applications, its speed and accuracy are relatively limited and therefore it is not advisable to use this Timer for ultra-fast, time-critical missions.

3) __User-Defined Overflow Period__

The library ebables users to select a pre-defined overflow period lengths based on one of the WatchDog Timer's base pre-scaler values, or alternatively, to define a custom length overflow period. With regard to the latter, it is important to note that the custom length period (in mS) must consist of a value that is evenly divisible by at least one of the said pre-scaler base values. For example, a custom period of 750mS is valid as it is divisible by the base pre-scaler value 250mS without a reminder. However, a custom period of 117mS is invalid as it cannot be divided by any of the pre-scaler base values without a reminder. In this context, the user can use the WatchDog::checkPeriod() function in order to see if a custom value is indeed valid or not.


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include "WatchDog.h"
```

With the library installed & included in the sketch, the following functions can be used directly to manage the WatchDog Timer:

>Note that using these functions requires preceding them with the library name (see the [usage example](https://github.com/nadavmatalon/WatchDog/blob/master/examples/WatchDog_Example/WatchDog_Example.ino) sketch for a more detailed implementation):


## LIBRARY FUNCTIONS

__WatchDog::init();__  
Parameters:&nbsp;&nbsp;&nbsp;ISR_function_name, ovf_period_t (optional), ovf_status_t or unsigned int (optional, but must be a valid value). Points to note:    
1) There are no parentheses follwing the ISR_function_name  
2) ovf_period_t params (optional): OVF_16MS   / OVF_32MS   / OVF_64MS   / OVF_125MS  / OVF_250MS / OVF_500MS  / OVF_1000MS [default] / OVF_2000MS / OVF_4000MS / OVF_8000MS or user-defined custom overflow period (value must be divisible by one of the WatchDog Timer's base pre-scaler values (i.e. 16 / 32 / 64 / 125 / etc.)  
3) ovf_status_t params (optional): START [default] / STOP (determines whether the WatchDog Timer automatically starts running or not after the overflow period is set.  
Description:&nbsp;&nbsp;&nbsp;Initializes the WatchDog timer with a user-defined ISR function name and (optionally) overflow period [default: 1000mS] and (also optionally) start or stop status. Place this function in the setup() section of the sketch.   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__WatchDog::start();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Starts the WatchDog Timer (or Se-starts the latter if it was previously stopped).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None 

__WatchDog::stop();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Stops the WatchDog Timer.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None 

__WatchDog::status();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current status of the WatchDog Timer (0 = Stopped / 1 = Running).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte   

__WatchDog::getPeriod();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current overflow period of the WatchDog Timer (in mS). Returns 0 if period has not been defined (e.g. prior to initialization or if an attempt was made to define the overflow period with an invalid value.   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int   

__WatchDog::setPeriod();__  
Parameters:&nbsp;&nbsp;&nbsp;ovf_period_t (OVF_16MS / OVF_32MS / OVF_64MS / OVF_125MS / OVF_250MS / OVF_500MS  / OVF_1000MS / OVF_2000MS / OVF_4000MS / OVF_8000MS) or unsigned int (optional, must be a valid value)
Description:&nbsp;&nbsp;&nbsp;Sets the current overflow period of the WatchDog Timer.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None     

__WatchDog::checkPeriod();__  
Parameters:&nbsp;&nbsp;&nbsp;unsigned int  
Description:&nbsp;&nbsp;&nbsp;Determines if a custom value for the WatchDog Timer's overflow period is valid (0 = invalid / 1 = valid) 
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte     


## RUNNING THE EXAMPLE SKETCH

1) Start the Arduino IDE and open the example sketch  
2) Upload the sketch to the Arduino  
3) Open the Serial Communications Window (make sure the baud-rate is set to 9600 or change it in the sketch to match your Serial Port's buad-rate) and follow the on-screen instructions  

## BUG REPORTS

Please report any issues/bugs/suggestions at the [Issues](https://github.com/nadavmatalon/WatchDog/issues) section of this Github repository.

## TODO

## VERSION HISTORY

__Ver. 1.0.0__ - First release (17.11.16)  
__Ver. 1.1.0__ - Added user-definible overflow period (18.11.16)  
__Ver. 1.2.0__ - Added support for ATtiny441/841 (5.12.16)

## LICENSE

[The MIT License (MIT)](https://opensource.org/licenses/MIT)
Copyright (c) 2016 Nadav Matalon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


