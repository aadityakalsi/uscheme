/**
 * \file test_uscheme.cpp
 * \date 2015
 */

#include <string>
#include "unittest.hpp"

// TESTS

#include <uscheme/defs.hpp>

CPP_TEST( version )
{
    TEST_TRUE( strcmp(uscheme::version(), "0.1.1") == 0 );
}
