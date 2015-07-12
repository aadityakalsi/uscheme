/**
 * \file test_uscheme_stream.cpp
 * \date 2015
 */

#include <string>
#include "unittest.hpp"

// LANG includes
#include <sstream>
#include <exception>

// TESTS

#include <uscheme/stream/stream.hpp>

CPP_TEST( read_object )
{
    {
        std::stringstream strm;
        
        try {
            auto p = uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("EOS") != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        
        strm << "127.";
        try {
            auto p = uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("delimiter or whitespace")
                != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "-127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == -127 );
    }

    {
        std::stringstream strm;
        strm << "+127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );
    }

    {
        std::stringstream strm;
        strm << "127   ";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );
    }

    {
        std::stringstream strm;
        strm << ";Comment line\r\n 127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );
    }
    
    {
        std::stringstream strm;
        strm << "\r\n\n;;\n;Comment line\r\n 127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );
    }
}
