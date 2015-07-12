/**
 * \file scheme.cpp
 * \date 2015
 */

#include <string>
#include <iostream>

#include <uscheme/defs.hpp>

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

int main(int argc, const char* argv[])
{
    if (argc != 2) { usage_and_die(); }

    std::string arg1(argv[1]);
    if (arg1 != "-h") {
        usage_and_die();
    } else {
        usage();
    }

    return 0;
}
