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
 * \file object.cpp
 * \date 2015
 */

// LANG includes
#include <cstring>

// PKG includes
#include <uscheme/type/type.hpp>
#include <uscheme/type/object.hpp>

#if defined(_WIN32)
#  define STRDUP _strdup
#else
#  define STRDUP strdup
#endif//defined(_WIN32)

namespace uscheme {

    static const object_ptr TRUE  = object::create_boolean(true);
    static const object_ptr FALSE = object::create_boolean(false);

    object_ptr true_value(void)
    {
        return TRUE;
    }

    object_ptr false_value(void)
    {
        return FALSE;
    }

    void object::init_string(const char* value)
    {
        data_.string.value = STRDUP(value);
    }

    void object::destroy()
    {
        switch (type_) {
            case STRING: {
                free((void*)data_.string.value);
                break;
            }
            default: {
                break;
            }
        }
    }

}//namespace uscheme
