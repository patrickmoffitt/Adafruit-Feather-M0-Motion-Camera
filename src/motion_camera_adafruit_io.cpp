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
// Created by Patrick Moffitt on 8/4/18.
//

#include "motion_camera_adafruit_io.hpp"


Motion_camera::Motion_camera() {
    setup_camera();
}

Motion_camera::~Motion_camera() {
    camera_off();
}

void Motion_camera::setup_camera(void) {
    pinMode(CAMERA_ENABLE_PIN, OUTPUT);
}

void Motion_camera::camera_on(void) {
    digitalWrite(CAMERA_ENABLE_PIN, LOW);
    delay(3000);
    // Try to locate the camera
    if (camera.begin(CAMERA_BAUD) == 1) {
        ERROR_PRINTLN(F("Camera found."));
    } else {
        ERROR_PRINTLN(F("No camera found."));
        return;
    }
    delay(1000);

    // Turn off NTSC video streaming.
    camera.TVoff(); delay(200);

    // Set the image dimensions; width x height.
    uint8_t img_size = camera.getImageSize(); delay(200);
    if (img_size != CAMERA_IMAGE_DIMENSIONS) {
        camera.setImageSize(CAMERA_IMAGE_DIMENSIONS); delay(200);
    }
    ERROR_PRINT(F("Image size: "));
    String resolution;
    switch (img_size) {
        case VC0706_640x480 : resolution = F("640x480");
            break;
        case VC0706_320x240 : resolution = F("320x240");
            break;
        case VC0706_160x120 : resolution = F("160x120");
            break;
        default : resolution = F("request failed.");
            break;
    }
    ERROR_PRINTLN(resolution);

    if (camera.getCompression() != CAMERA_COMPRESSION) {
        ERROR_PRINT(F("Set camera JPEG compression to "));
        ERROR_PRINT(String(CAMERA_COMPRESSION, DEC)); ERROR_PRINTLN(F("%."));
        camera.setCompression(CAMERA_COMPRESSION); delay(200);
    }

#ifdef MC_DEBUG
    // Print out the camera version information (optional)
    char *reply = camera.getVersion();
    if (reply == 0) {
        DEBUG_PRINTLN(F("Failed to get version."));
    } else {
        DEBUG_PRINTLN(F("-----------------"));
        DEBUG_PRINTLN(reply);
        DEBUG_PRINTLN(F("-----------------"));
    }
#endif
}

void Motion_camera::camera_off(void) {
    digitalWrite(CAMERA_ENABLE_PIN, HIGH);
}

const char *Motion_camera::aio_version = ADAFRUIT_IO_API_VERSION;
const char *Motion_camera::server      = ADAFRUIT_IO_SERVER_NAME;
const char *Motion_camera::username    = AIO_USERNAME;
const char *Motion_camera::password    = AIO_KEY;
const char *Motion_camera::group_key   = ADAFRUIT_IO_GROUP_KEY;
const char *Motion_camera::feed_name   = ADAFRUIT_IO_FEDD_NAME;


std::array<const char *, 4> Motion_camera::http_responses =
        {"The HTTPS POST request succeeded.",
         "The HTTPS client was unable to connect to the server. Check WiFi?",
         "The camera reported that the frame buffer is empty.",
         "The web server did not respond within the timeout required."
        };


const string Motion_camera::url = "/api/v" + string(aio_version) + "/" +
                                  string(username) +
                                  "/feeds/" +
                                  string(group_key) + "." + string(feed_name) +
                                  "/data";

int Motion_camera::post(void) {
    using b64 = Base64;
    using std::memcpy;

    DEBUG_PRINT(F("Server: ")); DEBUG_PRINTLN(server);
    DEBUG_PRINT(F("Port: ")); DEBUG_PRINTLN(port);
    if (!https.connectSSL(server, port)) {
        https.stop();
        camera_off();
        return HTTPS_NO_CONNECTION_TO_HOST;
    }
    // Get the size of the image (frame) taken
    uint32_t jpeg_len = mc.camera.frameLength(); delay(200);
    DEBUG_PRINT(F("JPEG Length: ")); DEBUG_PRINTLN(jpeg_len);

    if (jpeg_len == 0) {
        https.stop();
        camera_off();
        return CAMERA_INVALID_FRAME_LENGTH;
    }

    std::stringstream buf;
    static constexpr char rtn_nl[] = "\r\n";
    string header1;
    buf << "POST " << url << " HTTP/1.1" << rtn_nl
        << "Host: " << server << rtn_nl
        << "X-AIO-Key: " << password << rtn_nl
        << "User-Agent: MotionCamera/0.0.1" << rtn_nl
        << "Accept: text/plain" << rtn_nl
        << "Content-Type: application/json" << rtn_nl;
    header1 = buf.str();
    buf.str("");

    const char* json = "{\"value\": \"";
    size_t content_len = strlen(json) + b64::encoded_size(jpeg_len);
    content_len += 2; // 2 for the "}

    buf << "Content-Length: " << content_len << rtn_nl << rtn_nl;

    string message{header1 + buf.str()};
    buf.str("");

    https.write(message.c_str());
    ERROR_PRINTLN(message.c_str());

    // Read all the data from the camera and write it to the HTTPS host.
    const size_t camera_read{CAMERA_BUFFER};
    uint8_t *camera_buffer;

    // Keep b64 working on multiples of 3 until the very last chunk.
    // Only the last chunk may contain a pad. See RFC1113.
    size_t b64_full_chunk{63};
    size_t b64_half_chunk{33};
    uint8_t b64_buffer[b64_full_chunk + b64_half_chunk];
    size_t b64_buffer_end{0};
    uint8_t b64_in[b64_full_chunk];
    string b64_out;
    size_t camera_chunk{0};
    size_t processed_bytes{0};
    size_t chunk{0};

    DEBUG_PRINT(json);
    https.write(json, strlen(json));
    https.flush();

    while (processed_bytes < jpeg_len) {
        camera_chunk = std::min(((size_t)jpeg_len - processed_bytes), camera_read);
        camera_buffer = camera.readPicture((uint8_t)camera_chunk);
        processed_bytes += camera_chunk;

        // Move the data from the camera buffer to the b64 buffer.
        memmove(b64_buffer+b64_buffer_end, camera_buffer, camera_chunk);

        // Process full chunks.
        chunk = std::min(camera_chunk, b64_full_chunk);
        if (chunk == b64_full_chunk) {
            memcpy(b64_in, b64_buffer, chunk);
            b64_out = b64::encode(b64_in, chunk);
            https.write(b64_out.c_str(), b64_out.length());
            https.flush();
            DEBUG_PRINT(b64_out.c_str());
            // Realign b64_buffer
            b64_buffer_end += camera_chunk - chunk;
            memmove(b64_buffer, b64_buffer + chunk, b64_buffer_end);
        } else {
            b64_buffer_end += chunk;
        }

        // Periodically process half chunks so that the b64_buffer doesn't overflow.
        while (b64_buffer_end > b64_half_chunk) {
            memcpy(b64_in, b64_buffer, b64_half_chunk);
            b64_out = b64::encode(b64_in, b64_half_chunk);
            https.write(b64_out.c_str(), b64_out.length());
            https.flush();
            DEBUG_PRINT(b64_out.c_str());
            // Realign b64_buffer
            b64_buffer_end -= b64_half_chunk;
            memmove(b64_buffer, b64_buffer+b64_half_chunk, b64_buffer_end);
        }

        // At EOF jpeg, encode the remaining b64_buffer, possibly with a pad.
        if (camera_chunk < b64_full_chunk) {
            memcpy(b64_in, b64_buffer, b64_buffer_end);
            b64_out = b64::encode(b64_in, b64_buffer_end);
            https.write(b64_out.c_str(), b64_out.length());
            https.flush();
            DEBUG_PRINT(b64_out.c_str());
            // Realign b64_buffer
            b64_buffer_end -= b64_buffer_end;
            // No memory to move.
        }
    }

    string end;
    // buf << rtn_nl << boundary_mark_eot << rtn_nl;
    buf << "\"}";
    end = buf.str();
    https.write(end.c_str());
    https.flush();
    DEBUG_PRINTLN(end.c_str());
    buf.str("");
    camera.resumeVideo(); delay(200);
    camera_off();

    int delay_count{0};
    size_t response_size{0};
    // Since we lack callbacks or async io, poll for the HTTP response while yielding.
    while (delay_count < HTTPS_SERVER_RESPONSE_DELAY_SECONDS and response_size == 0) {
        response_size = static_cast<size_t>(https.available());
        delay_count++;
        delay(100);
    }
    if (delay_count >= HTTPS_SERVER_RESPONSE_DELAY_SECONDS or response_size == 0) {
        ERROR_PRINTLN(F("No response from HTTP server."));
        https.stop();
        return HTTPS_POST_NO_RESPONSE;
    }

    long http_response_code{0};
    DEBUG_PRINT(F("Response size: "));
    DEBUG_PRINT(response_size);
    DEBUG_PRINTLN(F(" bytes."));
    uint8_t response[response_size];
    if (https.read(response, response_size) != -1 and strlen((char *)response) > 0) {
        get_http_response_code((char *) response, &http_response_code);
        ERROR_PRINT(F("Response http_code: "));
        ERROR_PRINTLN(http_response_code);
        if (http_response_code != 200) {
            ERROR_PRINT((char *) response);
            int bytes_read{0};
            do {
                response_size = static_cast<size_t>(https.available());
                bytes_read = https.read(response, response_size);
                ERROR_PRINT((char *) response);
                delay(500);
            } while (bytes_read != -1);
        }
    } else {
        ERROR_PRINTLN(F("ERROR: Unable to read HTTPS server response."));
    }

    https.stop();
    return http_response_code;
}

void Motion_camera::get_http_response_code(const char *response_body, long *code) {
    char response[15];
    memcpy(response, response_body, 15);
    string needle{"HTTP/1.1 "};
    string response_str{string(response)};
    size_t found = response_str.find(needle);
    if (found == string::npos) {
        *code = -1;
    } else {
        size_t code_begin = found + needle.length();
        char *end;
        *code = strtol(response_str.substr(code_begin, 3).c_str(), &end, 10);
    }
}

void Motion_camera::printBuffer(uint8_t *buffer, uint16_t len) {
    DEBUG_PRINTER.print('\t');
    for (uint16_t i=0; i<len; i++) {
        if (isprint(buffer[i]))
            DEBUG_PRINTER.write(buffer[i]);
        else
            DEBUG_PRINTER.print(" ");
        DEBUG_PRINTER.print(F(" [0x"));
        if (buffer[i] < 0x10)
            DEBUG_PRINTER.print("0");
        DEBUG_PRINTER.print(buffer[i],HEX);
        DEBUG_PRINTER.print("], ");
        if (i % 8 == 7) {
            DEBUG_PRINTER.print("\n\t");
        }
    }
    DEBUG_PRINTER.println();
}

/*
 * Gets the message associated with local status codes or returns the code.
 *
 * See the IANA Hypertext Transfer Protocol (HTTP) Status Code Registry
 * https://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml
 */
const char *Motion_camera::get_http_response(int code) {
    if (code > 0 and code < (int) http_responses.size()) {
        return http_responses[code];
    } else {
        String response{"HTTP Response code: "};
        response.concat(code);
        const char *code_char{response.c_str()};
        return code_char;
    }
}

Motion_camera mc;