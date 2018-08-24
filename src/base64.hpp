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

#ifndef BASE64_HPP_
#define BASE64_HPP_

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

class Base64
{
public:
    static const size_t in_bytes{3};
    static const size_t out_chars{4};
    static const char codec[];
    static void encode_block(unsigned char *in, unsigned char *out, int len);
    static void encode(char *source_file, char *target_file);
    static void encode(const uint8_t *in_buffer, char *out_buffer, size_t in_length);
    static string encode(const uint8_t *in_buffer, size_t in_length);
    static size_t encoded_size(size_t un_encoded_size);
};
#endif //BASE64_HPP_
