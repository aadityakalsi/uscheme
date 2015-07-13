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
#include <unordered_set>

// PKG includes
#include <uscheme/type/type.hpp>
#include <uscheme/type/object.hpp>

#if defined(_WIN32)
#  define STRDUP _strdup
#else
#  define STRDUP strdup
#endif//defined(_WIN32)

typedef std::unique_ptr<char[]> safe_c_str;

struct string_hash
{
    USCHEME_INLINE
    size_t operator()(const safe_c_str& s) const
    {
        const char* str = s.get();
        size_t h = 0;
        for (; *str; ++str) {
            h = (h * 31) + (*str);
        }
        return h;
    }
};

struct string_equal
{
    USCHEME_INLINE
    bool operator()(const safe_c_str& s1, const safe_c_str& s2) const
    {
        return strcmp(s1.get(), s2.get()) == 0;
    }
};

namespace uscheme {


    typedef std::unordered_set<safe_c_str, string_hash, string_equal> set_type;

    static const object_ptr TRUE    = object::create_boolean(true);
    static const object_ptr FALSE   = object::create_boolean(false);
    static const object_ptr EMPTY   = object::create_empty_list();

    static set_type SYM_TAB;

    object_ptr true_value(void)
    {
        return TRUE;
    }

    object_ptr false_value(void)
    {
        return FALSE;
    }

    object_ptr empty_list_value(void)
    {
        return EMPTY;
    }

    void object::init_string(const char* value)
    {
        data_.string.value = STRDUP(value);
    }

    void object::init_symbol(const char* value)
    {
        size_t len = strlen(value);
        char* newstr = new char[len + 1];
        memcpy(newstr, value, len + 1);

        safe_c_str s(newstr);

        auto pr = SYM_TAB.insert(std::move(s));
        data_.string.value = (*(pr.first)).get();
    }

    void object::destroy()
    {
        switch (type_) {
            case STRING: {
                free((void*)data_.string.value);
                break;
            }
            case PAIR: {
                delete data_.pair.car;
                delete data_.pair.cdr;
                break;
            }
            default: {
                break;
            }
        }
    }

}//namespace uscheme
