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
        object_type t;
        switch (s.peek()) {
            case '#': {
                char ch = s.get();
                t = (s.peek() == '\\') ? CHARACTER : BOOLEAN;
                s.unget();
                break;
            }
            /* number */
            case '+': /* fall through */
            case '-': /* fall through */
            case '0': /* fall through */
            case '1': /* fall through */
            case '2': /* fall through */
            case '3': /* fall through */
            case '4': /* fall through */
            case '5': /* fall through */
            case '6': /* fall through */
            case '7': /* fall through */
            case '8': /* fall through */
            case '9': {
                t = FIXNUM;
                break;
            }
            default: {
                t = FIXNUM;
                break;
            }
        }
        return t;
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

    object_ptr read_boolean(std::istream& s)
    {
        char ch = s.get();
        ch = s.get();

        bool value = false;

        switch (ch) {
            case 't': value = true; break;
            case 'f': break;
            default :
                ERROR_IF(true, "Invalid boolean value.");
        }

        return value ? true_value() : false_value();
    }

    object_ptr read_character(std::istream& s)
    {
        char ch = s.get(); /* get # */
        ch = s.get();      /* get \ */
        ch = s.get();

        object_ptr p;

        if (ch != 'n' && ch != 't' && ch != 's') {
            p = object::create_character(ch);
        } else {
            /* could be newline or tab or space or just n or t or s */
            char characters[8];
            size_t numpushed = 0;
            if (ch == 'n') {
                if (is_delimiter(s.peek())) {
                    p = object::create_character('n');
                } else {
                    /* better match ewline */
                    for (; numpushed != 6; ++numpushed) {
                        characters[numpushed] = s.get();
                    }
                    characters[numpushed] = '\0';
                    if (strcmp(characters, "ewline") == 0) {
                        p = object::create_character('\n');
                    } else {
                        for (size_t nchar = 0; nchar != numpushed; ++nchar) {
                            s.unget();
                        }
                        ERROR_IF(true, "Character literal did not match \\newline.")
                    }
                }
            }
            if (ch == 't') {
                if (is_delimiter(s.peek())) {
                    p = object::create_character('t');
                } else {
                    /* better match ab */
                    for (; numpushed != 2; ++numpushed) {
                        characters[numpushed] = s.get();
                    }
                    characters[numpushed] = '\0';
                    if (strcmp(characters, "ab") == 0) {
                        p = object::create_character('\t');
                    } else {
                        for (size_t nchar = 0; nchar != numpushed; ++nchar) {
                            s.unget();
                        }
                        ERROR_IF(true, "Character literal did not match \\tab.")
                    }
                }
            }
            if (ch == 's') {
                if (is_delimiter(s.peek())) {
                    p = object::create_character('s');
                } else {
                    /* better match pace */
                    for (; numpushed != 4; ++numpushed) {
                        characters[numpushed] = s.get();
                    }
                    characters[numpushed] = '\0';
                    if (strcmp(characters, "pace") == 0) {
                        p = object::create_character(' ');
                    } else {
                        for (size_t nchar = 0; nchar != numpushed; ++nchar) {
                            s.unget();
                        }
                        ERROR_IF(true, "Character literal did not match \\space.")
                    }
                }
            }
        }
        return  p;
    }

    object_ptr read_object(std::istream& s)
    {
        skip_whitespace(s);
        ERROR_IF(s.eof(), "Reached EOS.");

        const auto t = determine_type(s);
        object_ptr p;
        switch (t) {
            case FIXNUM:
                p = read_fixnum(s);
                break;
            case BOOLEAN:
                p = read_boolean(s);
                break;
            case CHARACTER:
                p = read_character(s);
                break;
        }
        return p;
    }

    void print_object(std::ostream& os, const object_ptr& p)
    {
        switch (p->type()) {
            case FIXNUM: {
                os << p->fixnum();
                break;
            }
            case BOOLEAN: {
                os << '#' << (p->boolean() ? 't' : 'f');
                break;
            }
            case CHARACTER: {
                os << "#\\";
                char ch = p->character();
                switch (ch) {
                    case '\n': os << "newline"; break;
                    case ' ' : os << "space"; break;
                    case '\t': os << "tab"; break;
                    default  : os << ch; break;
                }
                break;
            }
        }
    }

}//namespace uscheme
