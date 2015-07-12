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
 * \file except.cpp
 * \date 2015
 */

#include <uscheme/except.hpp>

namespace uscheme {

    const char* error_string(except_id id)
    {

        switch (id) {
            case ERR_EOS:
                return "Reached EOS.";
            case ERR_UNK_TYPE:
                return "Invalid expression. Could not determine type.";
            case ERR_TERM_NUM:
                return "Number did not end with delimiter or whitespace.";
            case ERR_INV_BOOL:
                return "Invalid boolean value.";
            case ERR_CHAR_NL:
                return "Character literal did not match '#\\newline.'";
            case ERR_CHAR_SP:
                return "Character literal did not match '#\\space'.";
            case ERR_CHAR_TB:
                return "Character literal did not match '#\\tab'.";
            case ERR_STR_ABR:
                return "String literal terminated abruptly.";
            case ERR_TERM_STR:
                return "String literal not followed by delimiter.";
            case ERR_TERM_EMPTY:
                return "Empty list not terminated with ')', or no whitespace after.";
            default:
                return "Unknown error.";
        }

    }

}//namespace uscheme
