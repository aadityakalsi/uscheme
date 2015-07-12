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
 * \file scheme.cpp
 * \date 2015
 */

#include <string>
#include <iostream>

#include <uscheme/defs.hpp>
#include <uscheme/stream/stream.hpp>
#include <uscheme/exec/exec.hpp>

void usage(void)
{
    std::cout <<
    "\n"
    "usage: scheme [-h]\n"
    "\n"
    "Scheme interpreter using libuscheme.\n"
    "\n"
    "libuscheme version: " << uscheme::version() << "\n" <<
    "\n";
}

void usage_and_die(void)
{
    usage();
    exit(1);
}

template <bool print = true>
void repl(std::istream& strm)
{
    std::cout << "Welcome to uscheme. Use Ctrl-C to exit.\n";
    for (;;) {
      if (print) {
        std::cout << "> ";
      }

        uscheme::object_ptr p;
      try {
        p = uscheme::read_object(strm);
      } catch (const uscheme::exception& ex) {
        if (ex.id() == uscheme::ERR_EOS) {
            exit(0);
        } else {
            uscheme::skip_line(strm);
        }
        std::cerr << "ERROR: " << ex.what() << '\n';
        continue;
      }

        p = uscheme::eval_object(p);

      if (print) {
        uscheme::print_object(std::cout, p);
        std::cout << '\n';
      }
    }
}

int main(int argc, const char* argv[])
{
    switch (argc) {
        case 1: {
            repl<true>(std::cin);
            break;
        }
        case 2: {
            std::string arg1(argv[1]);
            if (arg1 != "-h") {
                usage_and_die();
            } else {
                usage();
            }
            break;
        }
        default:{
            usage_and_die();
            break;
        }
    }

    return 0;
}
