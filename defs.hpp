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
 * \file defs.hpp
 * \date 2015
 */

#ifndef USCHEME_DEFS_HPP
#define USCHEME_DEFS_HPP

#if defined(USCHEME_BUILD)
#  ifdef _MSC_VER
#    define USCHEME_API __declspec(dllexport)
#  else
#    define USCHEME_API __attribute__((__visibility__("default")))
#  endif
#else
#  ifdef _MSC_VER
#    define USCHEME_API __declspec(dllimport)
#  else
#    define USCHEME_API
#  endif
#endif

#if !defined(USCHEME_INLINE)
#  if defined(_MSC_VER)
#    define USCHEME_INLINE __forceinline
#  else/*GCC like compiler*/
#    define USCHEME_INLINE inline __attribute__((always_inline))
#  endif/*defined(_MSC_VER)*/
#endif/*defined(USCHEME_INLINE)*/

#if !defined(USCHEME_PRIVATE)
#  define USCHEME_PRIVATE static
#endif/*defined(USCHEME_PRIVATE)*/

#if !defined(USCHEME_DEBUG)
#  if !defined(NDEBUG)
#    define USCHEME_DEBUG 1
#  else
#    define USCHEME_DEBUG 0
#  endif//!defined(NDEBUG)
#endif/*defined(USCHEME_DEBUG)*/

namespace uscheme {

    USCHEME_API
    /**
     * Get the current version of this library as a string.
     */
    const char* version(void);

}//namespace uscheme

#endif//USCHEME_DEFS_HPP

