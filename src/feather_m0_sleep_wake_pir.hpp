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

#ifndef FEATHER_M0_SLEEP_WAKE_PIR_HPP
#define FEATHER_M0_SLEEP_WAKE_PIR_HPP
#include <Arduino.h>
#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.
#include <functional>

using time_type = decltype(millis());

#define MOTION_PIN PIN_A1

/*
 * Ambient temperatures within the PIR sensor's target range sometimes trigger
 * brief pulses of about 4200ms. MIN_PIR_PULSE_MS sets the threshold for activation
 * of the camera routine above such false alarms.
 */
#define MIN_PIR_PULSE_MS 4300

/*
 * A class that controls the sleep/wake functions of the Adafruit Feather M0 using a motion sensor.
 *
 * Abbreviations used in this class:
 *   - Passive Infra-Red (PIR)
 *   - External Interrupt Controller (EIC)
 *   - Interrupt Service Routine (ISR)
 */
struct feather_m0_sleep_wake_pir {
    volatile time_type eic_isr_rising{0U};
    volatile time_type eic_isr_falling{0U};
    volatile time_type elapsed_ms{0U};
    volatile bool take_picture{false};
    volatile bool taking_picture{false};
    void go_to_sleep(void);
    volatile time_type get_interval(time_type start_millis, time_type end_millis);
    void reset(void);
};


#endif //FEATHER_M0_SLEEP_WAKE_PIR_HPP
