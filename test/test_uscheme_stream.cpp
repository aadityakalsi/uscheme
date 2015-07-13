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

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == strm.str() );
    }

    {
        std::stringstream strm;
        strm << "+127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "127" );
    }

    {
        std::stringstream strm;
        strm << "127   ";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "127" );
    }

    {
        std::stringstream strm;
        strm << ";Comment line\r\n 127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "127" );
    }
    
    {
        std::stringstream strm;
        strm << "\r\n\n;;\n;Comment line\r\n 127";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "127" );
    }

    {
        std::stringstream strm;
        strm << "\r\n\n;;\n;Comment line\r\n;Another Comment\n127;Trailing comment";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::FIXNUM );
        TEST_TRUE( p->is_fixnum() );
        TEST_TRUE( p->fixnum() == 127 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "127" );
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

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#t" );
    }

    {
        std::stringstream strm;
        strm << "#f";
        
        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::BOOLEAN );
        TEST_TRUE( p->is_boolean() );
        TEST_TRUE( !p->boolean() );
        TEST_TRUE( p.get() == uscheme::false_value().get() );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#f" );
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

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\tab" );
    }

    {
        std::stringstream strm;
        strm << "#\\tab";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\t' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\tab" );
    }

    {
        std::stringstream strm;
        strm << "#\\\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\n' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\newline" );
    }

    {
        std::stringstream strm;
        strm << "#\\newline";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == '\n' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\newline" );
    }

    {
        std::stringstream strm;
        strm << "#\\ ";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == ' ' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\space" );
    }

    {
        std::stringstream strm;
        strm << "#\\space";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == ' ' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\space" );
    }

    {
        std::stringstream strm;
        strm << "#\\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == 'n' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\n" );
    }

    {
        std::stringstream strm;
        strm << "#\\s";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::CHARACTER );
        TEST_TRUE( p->is_character() );
        TEST_TRUE( p->character() == 's' );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\s" );
    }

    {
        std::stringstream strm;
        strm << "#\\t";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::CHARACTER);
        TEST_TRUE(p->is_character());
        TEST_TRUE(p->character() == 't');

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "#\\t" );
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
        strm << "\"\\nr\"";

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
        strm << "\"\\sr\"";

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
        strm << "\"\\tr\"";

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

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo/def\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo/def") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo/def\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\n\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\n") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\n\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\t\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\t") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\t\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\b\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\b") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\b\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\v\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\v") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\v\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\a\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\a") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\a\"" );
    }

    {
        std::stringstream strm;
        strm << "\"foo\\\\\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\\") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\\\\"" );
    }    

    {
        std::stringstream strm;
        strm << "\"foo\\\\\"";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::STRING );
        TEST_TRUE( p->is_string() );
        TEST_TRUE( strcmp(p->string(), "foo\\") == 0 );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "\"foo\\\\\"" );
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
                std::string(ex.what()).find("EOS")
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
                std::string(ex.what()).find("List")
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

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "()" );
    }

    {
        std::stringstream strm;
        strm << "(\n\r\n)\n";

        auto p = uscheme::read_object(strm);
        TEST_TRUE( p->type() == uscheme::EMPTY_LIST );
        TEST_TRUE( p->is_empty_list() );
        TEST_TRUE( p.get() == uscheme::empty_list_value().get() );

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE( os.str() == "()" );
    }
}

CPP_TEST( read_object_pair )
{
    {
        std::stringstream strm;
        strm << "(0 . 1)";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::PAIR);
        TEST_TRUE(p->is_pair());
        TEST_TRUE(p->car()->fixnum() == 0);
        TEST_TRUE(p->cdr()->fixnum() == 1);

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE(os.str() == "(0 . 1)");
    }

    {
        std::stringstream strm;
        strm << "(0   1)";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::PAIR);
        TEST_TRUE(p->is_pair());
        TEST_TRUE(p->car()->fixnum() == 0);
        TEST_TRUE(p->cdr()->car()->fixnum() == 1);
        TEST_TRUE(p->cdr()->cdr()->is_empty_list());

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE(os.str() == "(0 1)");
    }

    {
        std::stringstream strm;
        strm << "(0 . (1 . ()))";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::PAIR);
        TEST_TRUE(p->is_pair());
        TEST_TRUE(p->car()->fixnum() == 0);
        TEST_TRUE(p->cdr()->car()->fixnum() == 1);
        TEST_TRUE(p->cdr()->cdr()->is_empty_list());

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE(os.str() == "(0 1)");
    }

    {
        std::stringstream strm;
        strm << "(0 . (1 . 2))";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::PAIR);
        TEST_TRUE(p->is_pair());
        TEST_TRUE(p->car()->fixnum() == 0);
        TEST_TRUE(p->cdr()->car()->fixnum() == 1);
        TEST_TRUE(p->cdr()->cdr()->fixnum() == 2);

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE(os.str() == "(0 1 . 2)");
    }
}

CPP_TEST( read_object_symbol )
{
    {
        std::stringstream strm;
        strm << "arf";

        auto p = uscheme::read_object(strm);
        TEST_TRUE(p->type() == uscheme::SYMBOL);
        TEST_TRUE(p->is_symbol());
        TEST_TRUE(strcmp(p->symbol(), "arf") == 0);

        std::stringstream strm1;
        strm1 << "arf";
        auto q = uscheme::read_object(strm1);
        TEST_TRUE(q->type() == uscheme::SYMBOL);
        TEST_TRUE(q->is_symbol());
        TEST_TRUE(strcmp(q->symbol(), "arf") == 0);

        TEST_TRUE(p->symbol() == q->symbol());

        std::stringstream os;
        uscheme::print_object(os, p);
        TEST_TRUE(os.str() == "arf");
    }
}
