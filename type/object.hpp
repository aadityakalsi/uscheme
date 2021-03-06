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
 * \file object.hpp
 * \date 2015
 */

#ifndef USCHEME_TYPE_OBJECT_HPP
#define USCHEME_TYPE_OBJECT_HPP

// LANG includes
#include <cstdlib>
#include <memory>

// PKG includes
#include <uscheme/defs.hpp>
#include <uscheme/type/type.hpp>

namespace uscheme {

    struct object;

    /**
     *
     */
    typedef std::shared_ptr<object> object_ptr;

    /**
     *
     */
    struct object
    {

        //////////////////////////////////////////////////////////////////////////
        // Static methods
        //////////////////////////////////////////////////////////////////////////

        static USCHEME_INLINE
        object_ptr create_fixnum(long value)
        {
            object_ptr ptr(new object);
            ptr->type_ = FIXNUM;
            ptr->data_.fixnum.value = value;
            return ptr;
        }

        static USCHEME_INLINE
        object_ptr create_boolean(bool value)
        {
            object_ptr ptr(new object);
            ptr->type_ = BOOLEAN;
            ptr->data_.boolean.value = value ? 1 : 0;
            return ptr;
        }

        static USCHEME_INLINE
        object_ptr create_character(char value)
        {
            object_ptr ptr(new object);
            ptr->type_ = CHARACTER;
            ptr->data_.character.value = value;
            return ptr;
        }

        static USCHEME_INLINE
        object_ptr create_string(const char* value)
        {
            object_ptr ptr(new object);
            ptr->type_ = STRING;
            ptr->init_string(value);
            return ptr;
        }

        static USCHEME_INLINE
        object_ptr create_empty_list()
        {
            object_ptr ptr(new object);
            ptr->type_ = EMPTY_LIST;
            return ptr;
        }

        //////////////////////////////////////////////////////////////////////////
        // Instance methods
        //////////////////////////////////////////////////////////////////////////

        USCHEME_INLINE
        object_type type() const
        {
            return type_;
        }

        USCHEME_INLINE
        bool is_fixnum() const
        {
            return type_ == FIXNUM;
        }

        USCHEME_INLINE
        bool is_boolean() const
        {
            return type_ == BOOLEAN;
        }

        USCHEME_INLINE
        bool is_character() const
        {
            return type_ == CHARACTER;
        }

        USCHEME_INLINE
        bool is_string() const
        {
            return type_ == STRING;
        }

        USCHEME_INLINE
        bool is_empty_list() const
        {
            return type_ == EMPTY_LIST;
        }

        USCHEME_INLINE
        long fixnum() const
        {
            return data_.fixnum.value;
        }

        USCHEME_INLINE
        bool boolean() const
        {
            return data_.boolean.value != 0;
        }

        USCHEME_INLINE
        char character() const
        {
            return data_.character.value;
        }

        USCHEME_INLINE
        const char* string() const
        {
            return data_.string.value;
        }

        ~object()
        {
            destroy();
        }

      private:
        object()
          : type_(FIXNUM)
          , data_()
        {
            data_.fixnum.value = 0;
        }

        enum object_type type_;
        union {
            struct {
                long value;
            } fixnum;
            struct {
                char value;
            } boolean;
            struct {
                char value;
            } character;
            struct {
                const char* value;
            } string;
        } data_;

        void init_string(const char* val);

        void destroy();
    };

    USCHEME_API
    /**
     *
     */
    object_ptr true_value(void);

    USCHEME_API
    /**
     *
     */
    object_ptr false_value(void);

    USCHEME_API
    /**
     *
     */
    object_ptr empty_list_value(void);

}//namespace uscheme

#endif//USCHEME_TYPE_OBJECT_HPP
