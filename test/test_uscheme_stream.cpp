/**
 * \file test_uscheme_stream.cpp
 * \date 2015
 */


// LANG includes
#include <sstream>
#include <iostream>
#include <string>
#include <exception>

// TEST includes
#include "unittest.hpp"

// PKG includes
#include <uscheme/type/object.hpp>
#include <uscheme/stream/stream.hpp>

CPP_TEST( read_object_fixnum )
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

    {
        std::stringstream strm;
        strm << "\r\n\n;;\n;Comment line\r\n;Another Comment\n127;Trailing comment";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );
    }
}

CPP_TEST( read_object_boolean )
{
    {
        std::stringstream strm;
        strm << "#y";
        
        try {
            auto p = uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("Invalid boolean") != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "#t";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::BOOLEAN );
        TEST_TRUE( p->is_boolean() );
        TEST_TRUE( p->boolean() );
        TEST_TRUE( p.get() == uscheme::true_value().get() );
    }

    {
        std::stringstream strm;
        strm << "#f";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::BOOLEAN );
        TEST_TRUE( p->is_boolean() );
        TEST_TRUE( !p->boolean() );
        TEST_TRUE( p.get() == uscheme::false_value().get() );
    }
}

CPP_TEST( read_object_character )
{
    {
        std::stringstream strm;
        strm << "#\\\t";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\t' );
    }

    {
        std::stringstream strm;
        strm << "#\\tab";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\t' );
    }

    {
        std::stringstream strm;
        strm << "#\\\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\n' );
    }

    {
        std::stringstream strm;
        strm << "#\\newline";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\n' );
    }

    {
        std::stringstream strm;
        strm << "#\\ ";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == ' ' );
    }

    {
        std::stringstream strm;
        strm << "#\\space";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == ' ' );
    }

    {
        std::stringstream strm;
        strm << "#\\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == 'n' );
    }

    {
        std::stringstream strm;
        strm << "#\\s";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == 's' );
    }

    {
        std::stringstream strm;
        strm << "#\\t";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::CHARACTER);
        TEST_TRUE(p->is_character());
        TEST_TRUE(p->character() == 't');
    }
}

