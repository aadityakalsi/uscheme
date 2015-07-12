/*
Copyright (c) 2015, Aaditya Kalsi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file stream.cpp
 * \date 2015
 */

// LANG includes
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

// PKG includes
#include <uscheme/stream/stream.hpp>

#define ERROR_IF(cond, msg)        \
 if ((cond)) {                     \
    throw std::runtime_error(msg); \
 }

namespace uscheme {

    bool is_delimiter(char ch)
    {
        return isspace(ch) || (ch == EOF) ||
               (ch == '(') || (ch == ')') ||
               (ch == '"') || (ch == ';');
    }

    void skip_line(std::istream& s)
    {
        char ch = s.peek();
        while ((ch != '\r') && (ch != '\n') && (ch != EOF)) {
            s.get();
            ch = s.peek();
        }
        switch (ch) {
            case '\r':
                s.get();
                if (s.peek() == '\n') { s.get(); }
                break;
            case '\n':
                s.get();
                break;
            default:
                break;
        }
        return;
    }

    void skip_whitespace(std::istream& s)
    {
        char ch;
        while (true) {
            ch = s.peek();
            if (isspace(ch)) {
                s.get();
                continue;
            }
            if (ch == ';') {
                // comments are whitespace too
                s.get();
                skip_line(s);
                continue;
            }
            break;
        }
    }

    object_type determine_type(std::istream& s)
    {
        return FIXNUM;
    }

    object_ptr read_fixnum(std::istream& s)
    {
        char ch = s.peek();
        
        long sign = (ch == '-') ? -1 : 1;
        long num = 0;
        
        if (ch == '-' || ch == '+') { s.get(); }

        ch = s.peek();
        while (isdigit(ch)) {
            num = (num * 10) + (ch - '0');
            s.get();
            ch = s.peek();
        }

        num *= sign;

        ERROR_IF(
            !is_delimiter(ch),
            "Number did not end with delimiter or whitespace.");

        return object::create_fixnum(num);
    }

    object_ptr read_object(std::istream& s)
    {
        skip_whitespace(s);
        ERROR_IF(s.eof(), "Reached EOS.");

        const auto t = determine_type(s);
        object_ptr p;
        switch (t) {
            case FIXNUM:
                p = std::move(read_fixnum(s));
        }
        return p;
    }

}//namespace uscheme
