/**
 * \file test_uscheme_stream.cpp
 * \date 2015
 */


// LANG includes
#include <cstring>
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

CPP_TEST( read_object_string )
{
    {
        std::stringstream strm;
        strm << "\"foo";

        try {
            uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("String literal")
                != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "\"foo\":";

        try {
            uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("String literal")
                != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "\"foo\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo/def\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo/def") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\n\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\n") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\t\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\t") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\b\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\b") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\v\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\v") == 0 );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\\\\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\\") == 0 );
    }
}

CPP_TEST( read_object_empty_list )
{
    {
        std::stringstream strm;
        strm << "(";

        try {
            uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("Empty list")
                != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "()gf";

        try {
            uscheme::read_object(strm);
        } catch (const std::exception& ex) {
            TEST_TRUE(
                std::string(ex.what()).find("Empty list")
                != std::string::npos);
        }
    }

    {
        std::stringstream strm;
        strm << "( )";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::EMPTY_LIST );
        TEST_TRUE( p->is_empty_list() );
        TEST_TRUE( p.get() == uscheme::empty_list_value().get() );
    }

    {
        std::stringstream strm;
        strm << "(\n\r\n)\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::EMPTY_LIST );
        TEST_TRUE( p->is_empty_list() );
        TEST_TRUE( p.get() == uscheme::empty_list_value().get() );
    }
}

