/*
    Copyright (c) 2018 Patrick Moffitt

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
//
// Created by Patrick Moffitt on 8/17/18.
//

#include "feather_m0_sleep_wake_pir.hpp"


void feather_m0_sleep_wake_pir::go_to_sleep(void) {
    Serial.println("Sleeping."); Serial.println(); Serial.flush();
    __WFI();   // (Wake From Interrupt).
    delay(1000);  // Yield to sleep.
}

volatile time_type feather_m0_sleep_wake_pir::get_interval(time_type start_millis,
                                                           time_type end_millis) {
    time_type interval{0U};
    if (end_millis < start_millis) {  // millis() has rolled over.
        interval = (ULONG_MAX - start_millis) + end_millis;
    } else {
        interval = end_millis - start_millis;
    }
    return interval;
}

void feather_m0_sleep_wake_pir::reset(void) {
    eic_isr_falling = 0U;
    eic_isr_rising = 0U;
    elapsed_ms = 0U;
    take_picture = false;
    taking_picture = false;
}