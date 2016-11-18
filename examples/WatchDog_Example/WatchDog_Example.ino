#include "WatchDog.h"

int cmd;
unsigned long timeNow = 0;

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    WatchDog::init(blinkISR, OVF_500MS);
    Serial.print(F("\n\nWatchDog Timer Functions:                                      \
                      \nSend 0 to Stop WatchDog Timer                                  \
                      \nSend 1 to (Re-)Start WatchDog Timer                            \
                      \nSend 2 for WatchDog Timer Status                               \
                      \nSend 3 for WatchDog Timer Overflow Period                      \
                      \nSend 4 to Set WatchDog Timer Overflow Period to  250mS         \
                      \nSend 5 to Set WatchDog Timer Overflow Period to  500mS         \
                      \nSend 6 to Set WatchDog Timer Overflow Period to 1000mS         \
                      \n(Running numbers show elapsed time in mS to indicate no reset) \
                      \n\n"));
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
            if (WatchDog::status()) {
                Serial.print(F("\n\nwatchdog already running\n\n"));
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
        }
    }
}
  
void blinkISR() {                                         // watchdog timer interrupt service routine    
    digitalWrite(13, !digitalRead(13));
}

