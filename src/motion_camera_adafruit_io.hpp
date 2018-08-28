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

#ifndef MOTION_CAMERA_ADAFRUIT_IO_HPP
#define MOTION_CAMERA_ADAFRUIT_IO_HPP

// Uncomment/comment to turn on/off debug output messages.
// #define MC_DEBUG
// Uncomment/comment to turn on/off error output messages.
// #define MC_ERROR

// Set where debug messages will be printed.
#define DEBUG_PRINTER Serial

// Define actual debug output functions when necessary.
#ifdef MC_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
  #define DEBUG_PRINTBUFFER(buffer, len) { printBuffer(buffer, len); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#define DEBUG_PRINTBUFFER(buffer, len) {}
#endif

#ifdef MC_ERROR
#define ERROR_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define ERROR_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#define ERROR_PRINTBUFFER(buffer, len) { printBuffer(buffer, len); }
#else
#define ERROR_PRINT(...) {}
  #define ERROR_PRINTLN(...) {}
  #define ERROR_PRINTBUFFER(buffer, len) {}
#endif

#include <WiFiSSLClient.h>
#include "./Adafruit_VC0706.h"
#undef min  // @ToDo remove when library? bug fixed.
#undef max  // @ToDo remove when library? bug fixed.
#include <algorithm>
#include <sstream>
#include <string>
#include "base64.hpp"


// Settings
#define CAMERA_ENABLE_PIN PIN_A0
#define CAMERA_COMPRESSION 0x00
#define CAMERA_AUTO_EXPOSURE_DELAY_MS 5000
#define CAMERA_IMAGE_DIMENSIONS VC0706_640x480
#define CAMERA_BAUD ((unsigned long)115200)
#define CAMERA_BUFFER 64  // Do not change. Use 64 or base64 encoding will break.
#define HTTPS_SERVER_RESPONSE_DELAY_SECONDS 12
#define ADAFRUIT_IO_API_VERSION "2"
#define ADAFRUIT_IO_SERVER_NAME "io.adafruit.com"
#define ADAFRUIT_IO_GROUP_KEY "motion-camera"
#define ADAFRUIT_IO_FEED_NAME "images"

// Error codes.
#define HTTPS_NO_CONNECTION_TO_HOST 1
#define CAMERA_INVALID_FRAME_LENGTH 2
#define HTTPS_POST_NO_RESPONSE 3

using std::string;

class Motion_camera {
    static std::array<const char*, 4> http_responses;
public:
    Motion_camera();
    ~Motion_camera();
    void setup_camera(void);
    void camera_on(void);
    void camera_off(void);

    // Store the Adafruit.io server, username, and password in flash memory.
    static const char *aio_version;
    static const char *server;
    static const int port{443};
    static const char *username;
    static const char *password;
    static const char *group_key;
    static const char *feed_name;


    // Define feed.
    static const string url;

    int post(void);

    void get_http_response_code(const char *response_body, long *code);
    static const char *get_http_response(int code);

    void printBuffer(uint8_t *buffer, uint16_t len);

    Adafruit_VC0706 camera = Adafruit_VC0706(&Serial1);
    // Create a WiFiClient class to connect to the Adafruit.io server.
    WiFiSSLClient https;
};

extern Motion_camera mc;

#endif //MOTION_CAMERA_ADAFRUIT_IO_HPP
