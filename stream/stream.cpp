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
#include <cstring>
#include <cstdlib>
#include <string>

// PKG includes
#include <uscheme/except.hpp>
#include <uscheme/stream/stream.hpp>

#define ERROR_IF(cond, id)        \
 if ((cond)) {                    \
    throw uscheme::exception(id); \
 }

namespace uscheme {

    bool is_delimiter(char ch)
    {
        return isspace(ch) || (ch == EOF) ||
            (ch == '(') || (ch == ')') ||
            (ch == '"') || (ch == ';');
    }

    bool is_whitespace(char ch)
    {
        return isspace(ch) || (ch == ';') || (ch == EOF);
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
                s.get();
                t = (s.peek() == '\\') ? CHARACTER : BOOLEAN;
                s.unget();
                break;
            }
            case '"': {
                t = STRING;
                break;
            }
            case '(': {
                t = PAIR;
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
                ERROR_IF(true, ERR_UNK_TYPE);
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

        ERROR_IF(!is_delimiter(ch), ERR_TERM_NUM);

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
                ERROR_IF(true, ERR_INV_BOOL);
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
                        ERROR_IF(true, ERR_CHAR_NL);
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
                        ERROR_IF(true, ERR_CHAR_TB);
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
                        ERROR_IF(true, ERR_CHAR_SP);
                    }
                }
            }
        }
        return p;
    }

    object_ptr read_string(std::istream& s)
    {
        static std::string BUFFER;
        BUFFER.resize(0);

        char ch = s.get(); /* skip the " */
        while ((ch = s.peek()) != EOF && (ch != '\0') && (ch != '"')) {
            if (ch == '\\') {
                s.get();
                switch (s.peek()) {
                    case '\\': {
                        ch = '\\';
                        break;
                    }
                    case 'n': {
                        ch = '\n';
                        break;
                    }
                    case 't': {
                        ch = '\t';
                        break;
                    }
                    case '"': {
                        ch = '"';
                        break;
                    }
                    case 'a': {
                        ch = '\a';
                        break;
                    }
                    case 'b': {
                        ch = '\b';
                        break;
                    }
                    case 'v': {
                        ch = '\v';
                        break;
                    }
                    default: {
                        continue;
                        // s.unget();
                        break;
                    }
                }
            }
            BUFFER.push_back(ch);
            s.get();
        }
        
        ERROR_IF((ch != '"'), ERR_STR_ABR);
        s.get();
        ERROR_IF(!is_whitespace(s.peek()), ERR_TERM_STR);

        return object::create_string(BUFFER.c_str());
    }

    object_ptr read_empty_list(std::istream& s)
    {
        skip_whitespace(s);
        ERROR_IF((s.peek() != ')'), ERR_TERM_LIST);
        s.get();
        ERROR_IF(!is_whitespace(s.peek()) && (s.peek() != ')'), ERR_TERM_LIST);
        return empty_list_value();
    }

    object_ptr read_pair(std::istream& s)
    {
        object_ptr car;
        object_ptr cdr;

        if (s.peek() == '(') {
            /* skip the '(' */
            s.get();
        }

        skip_whitespace(s);

        if (s.peek() == ')') { return read_empty_list(s); }

        car = read_object(s);
        skip_whitespace(s);

        object_ptr out;

        if (s.peek() == '.') {
            /* improper list */
            s.get();
            ERROR_IF(!is_whitespace(s.peek()), ERR_IMP_LIST_WS);
            cdr = read_object(s);
            skip_whitespace(s);
            ERROR_IF(s.peek() != ')', ERR_TERM_LIST);
            s.get();
            out = object::cons(car, cdr);
        } else {
            cdr = read_pair(s);
            out = object::cons(car, cdr);
        }

        //if (is_whitespace(s.peek())) {
        //    skip_whitespace(s);
        //}
        //ERROR_IF(s.peek() != ')', ERR_TERM_LIST);
        //s.get();
        //ERROR_IF(!is_whitespace(s.peek()), ERR_TERM_LIST);

        return out;
    }

    object_ptr read_object(std::istream& s)
    {
        skip_whitespace(s);
        ERROR_IF(s.eof(), ERR_EOS);

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
            case STRING:
                p = read_string(s);
                break;
            case PAIR:
                p = read_pair(s);
                break;
            case EMPTY_LIST:
                p = read_empty_list(s);
                break;
        }
        return p;
    }

    void print_pair(std::ostream& os, const object_ptr& p)
    {
        print_object(os, p->car());
        auto cdr = p->cdr();
        if (cdr->is_pair()) {
            os.put(' ');
            print_pair(os, cdr);
        } else if (cdr->is_empty_list()) {
            return;
        } else {/* improper list */
            os << " . ";
            print_object(os, cdr);
        }
    }

    void print_object(std::ostream& os, const object_ptr& p)
    {
        switch (p->type()) {
            case FIXNUM: {
                os << p->fixnum();
                break;
            }
            case BOOLEAN: {
                os.put('#').put(p->boolean() ? 't' : 'f');
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
            case STRING: {
                os.put('"');

                const char* str = p->string();
                char ch;
                while ((ch = *str)) {
                    ++str;
                    switch (ch) {
                        case '\\' : {
                            os.put('\\').put('\\');
                            break;
                        }
                        case '"' : {
                            os.put('\\').put('"');
                            break;
                        }
                        case '\n': {
                            os.put('\\').put('n');
                            break;
                        }
                        case '\t': {
                            os.put('\\').put('t');
                            break;
                        }
                        case '\a': {
                            os.put('\\').put('a');
                            break;
                        }
                        case '\b': {
                            os.put('\\').put('b');
                            break;
                        }
                        case '\v': {
                            os.put('\\').put('v');
                            break;
                        }
                        default: {
                            os.put(ch);
                            break;
                        }
                    }
                }

                os.put('"');
                break;
            }
            case EMPTY_LIST: {
                os << "()";
                break;
            }
            case PAIR: {
                os.put('(');
                print_pair(os, p);
                os.put(')');
                break;
            }
        }
    }

}//namespace uscheme
