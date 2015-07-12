/**
 * \file test_uscheme.cpp
 * \date 2015
 */

// LANG includes
#include <cstring>

// TEST includes
#include "unittest.hpp"

// TESTS

#include <uscheme/defs.hpp>

CPP_TEST( version )
{
    TEST_TRUE( strcmp(uscheme::version(), "0.1.1") == 0 );
}
