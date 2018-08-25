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
#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.

#include "./src/motion_camera_secrets.hpp"
#include "./src/motion_camera_adafruit_io.hpp" // Defines mc object.
#include "./src/feather_m0_sleep_wake_pir.hpp"




#define WIFI_TIMEOUT_TENTH_SECONDS 300
feather_m0_sleep_wake_pir feather;

void EIC_ISR(void) {
    if (feather.eic_isr_rising == 0U) {
        feather.eic_isr_rising = millis();
    } else {
        feather.eic_isr_falling = millis();
    }
    if (feather.eic_isr_rising != 0U and feather.eic_isr_falling != 0U) {
        feather.elapsed_ms = feather.get_interval(feather.eic_isr_rising, feather.eic_isr_falling);
    }
    // Motion is below threshold; reset timers.
    if (feather.elapsed_ms > 0U and feather.elapsed_ms < MIN_PIR_PULSE_MS) feather.reset();

    // Motion is above threshold; take a picture.
    if (feather.elapsed_ms > MIN_PIR_PULSE_MS and not feather.taking_picture) {
        feather.take_picture = true;
    }
}


void setup(){
    pinMode(MOTION_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(MOTION_PIN), EIC_ISR, CHANGE);

    // set external 32k oscillator to run when idle or sleep mode is chosen
    SYSCTRL->XOSC32K.reg |= SYSCTRL_XOSC32K_RUNSTDBY | SYSCTRL_XOSC32K_ONDEMAND;

    // generic clock multiplexer id for the external interrupt controller
    REG_GCLK_CLKCTRL |= GCLK_CLKCTRL_ID(GCM_EIC) |
                        GCLK_CLKCTRL_GEN_GCLK1 |  // generic clock 1 which is xosc32k
                        GCLK_CLKCTRL_CLKEN;       // enable it
    while (GCLK->STATUS.bit.SYNCBUSY);             // write protected, wait for sync

    // Set External Interrupt Controller (EIC) to use channel 4 (pin 6)
    EIC->WAKEUP.reg |= EIC_WAKEUP_WAKEUPEN4;

    PM->SLEEP.reg |= PM_SLEEP_IDLE_CPU;  // Enable Idle0 mode - sleep CPU clock only
    //PM->SLEEP.reg |= PM_SLEEP_IDLE_AHB; // Idle1 - sleep CPU and AHB clocks
    //PM->SLEEP.reg |= PM_SLEEP_IDLE_APB; // Idle2 - sleep CPU, AHB, and APB clocks

    // It is either Idle mode or Standby mode, not both.
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Enable Standby or "deep sleep" mode
    __DSB(); /* Ensure effect of last store takes effect */

    Serial.begin(115200);
    WiFi.setPins(8, 7, 4, 2);
    WiFi.maxLowPowerMode();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Turn the camera on and show the boot message.
    mc.camera_on();
    char *boot_msg = mc.camera.resetMessage();
    DEBUG_PRINTLN(F("-----------------"));
    DEBUG_PRINT(boot_msg);
    DEBUG_PRINTLN(F("-----------------"));
    DEBUG_PRINTLN();
    mc.camera_off();

    delay(10000);  // Allow the PIR sensor to settle and then reset the timers.
    feather.reset();
}

void loop() {

    ERROR_PRINTLN(F("eic_isr_rising, eic_isr_falling, elapsed_ms"));
    ERROR_PRINT(feather.eic_isr_rising); ERROR_PRINT(F(", "));
    ERROR_PRINT(feather.eic_isr_falling); ERROR_PRINT(F(", "));
    ERROR_PRINTLN(feather.elapsed_ms);

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        uint8_t delay_count{0};
        while (delay_count < WIFI_TIMEOUT_TENTH_SECONDS and WiFi.localIP() == 0) {
            delay(100);
            delay_count++;
        }
        if (delay_count == WIFI_TIMEOUT_TENTH_SECONDS) {
            ERROR_PRINTLN(F("No WiFi connection!"));
            feather.go_to_sleep(); // No WiFi; no picture.
        }
    }

    if (feather.take_picture == 1) {
        DEBUG_PRINTLN(F("EIC ISR Called."));
        mc.camera_on();
        mc.camera.resetMessage();
        if (mc.camera.takePicture() == 1) {
            Serial.println("Picture taken.");
            feather.taking_picture = true;
            auto https_response = mc.post();
            ERROR_PRINT(F("elapsed_ms: ")); ERROR_PRINTLN(feather.elapsed_ms);
            feather.reset();
            if (https_response == 200) {
                Serial.print("Picture posted to "); Serial.println(ADAFRUIT_IO_SERVER_NAME);
            } else {
                Serial.println(mc.get_http_response(https_response));
            }
        } else {
            feather.reset();
            ERROR_PRINTLN(F("No picture taken. Check camera."));
        }
    }

    if (feather.eic_isr_rising == 0U){
        mc.camera_off();
        WiFi.end(); delay(200);
        feather.go_to_sleep();
    } else {
        delay(2 * MIN_PIR_PULSE_MS);
    }
}
