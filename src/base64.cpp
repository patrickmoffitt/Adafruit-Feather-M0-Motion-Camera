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

/*
    Copyright (c) 2001 Bob Trower, Trantor Standard Systems Inc.

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the
    Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall
    be included in all copies or substantial portions of the
    Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
    KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
    OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
    OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "base64.hpp"

using std::memcpy;

/*
 * Translation Table as described in RFC1113
*/
const char Base64::codec[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * encode_block
 *
 * encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void Base64::encode_block(unsigned char *in, unsigned char *out, int len) {
    out[0] = (unsigned char) codec[(int) (in[0] >> 2U)];
    out[1] = (unsigned char) codec[(((in[0] & 3U) << 4U) | ((in[1] & 240U) >> 4U))];
    out[2] = (unsigned char) (len > 1 ? codec[(((in[1] & 15U) << 2U) | ((in[2] & 192U) >> 6U))]
            : '=');
    out[3] = (unsigned char) (len > 2 ? codec[(in[2] & 63U)] : '=');
}

/*
 * encode
 *
 * base64 encode a stream adding padding and line breaks as per spec.
*/
void Base64::encode(char *source_file, char *target_file) {
    unsigned char in[in_bytes];
    unsigned char out[out_chars];
    uint8_t i{0};
    int len{0};

    std::ifstream ifs{source_file, std::ios::in};
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << *source_file << " for reading." << std::endl;
        return;
    }

    std::ofstream ofs{target_file, std::ios::out|std::ios::trunc};
    if (!ofs.is_open()) {
        std::cerr << "Failed to open " << *target_file << " for writing." << std::endl;
        return;
    }

    *in = (unsigned char) 0;
    *out = (unsigned char) 0;
    char ch;
    while(!ifs.eof()) {
        len = 0;
        for (i = 0; i < in_bytes; i++) {
            ifs.get(ch);
            in[i] = (unsigned char) ch;

            if (!ifs.eof()) {
                len++;
            } else {
                in[i] = (unsigned char) 0;
            }
        }
        if (len > 0) {
            encode_block(in, out, len);
            for (i = 0; i < out_chars; i++) {
                ofs << out[i];
            }
        }
    }
    ifs.close();
    ofs.close();
}

/*
 * encode
 *
 * base64 encode a buffer adding padding and line breaks as per spec.
*/
void Base64::encode(const uint8_t *in_buffer, char *out_buffer, size_t in_length) {
    int len{0};
    size_t encoded_bytes{0};
    unsigned char in[in_bytes];
    unsigned char out[out_chars];
    unsigned int i{0};
    unsigned int j{0};
    unsigned char zero{0};
    while (i < in_length) {
        len = 0;
        memcpy(&in, &zero, in_bytes);
        for (j = 0; j < in_bytes; j++) {
            if (i < in_length) {
                in[j] = (unsigned char) in_buffer[i];
                i++;
                len++;
            }
        }
        if (len > 0) {
            encode_block(in, out, len);
            for (j = 0; j < out_chars; j++) {
                out_buffer[encoded_bytes] = out[j];
                encoded_bytes++;
            }
        }
    }
    out_buffer[encoded_bytes] = 0; // End it.
}

string Base64::encode(const uint8_t *in_buffer, size_t in_length) {
    std::stringstream out_buff;
    int len{0};
    size_t encoded_bytes{0};
    unsigned char in[in_bytes];
    unsigned char out[out_chars];
    unsigned int i{0};
    unsigned int j{0};
    unsigned char zero{0};
    while (i < in_length) {
        len = 0;
        memcpy(&in, &zero, in_bytes);
        for (j = 0; j < in_bytes; j++) {
            if (i < in_length) {
                in[j] = (unsigned char) in_buffer[i];
                i++;
                len++;
            }
        }
        if (len > 0) {
            encode_block(in, out, len);
            for (j = 0; j < out_chars; j++) {
                out_buff << out[j];
                encoded_bytes++;
            }
        }
    }
    return out_buff.str();
}

size_t Base64::encoded_size(const size_t un_encoded_size) {
    // 3 becomes 4.
    size_t encoded_size{1};
    encoded_size = (un_encoded_size / 3) * 4;
    if (un_encoded_size % 3 != 0) {
        encoded_size += 4;
    }
    return encoded_size;
}

