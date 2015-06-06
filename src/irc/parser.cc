// BriocheBot
// The MIT License(MIT)
//
// Copyright(c) 2015 Abricot Soinou <abricot.soinou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "irc/parser.h"

namespace Irc
{
    Parser::Parser(const std::string& line) : scanner_(line), message_()
    {
    }

    Parser::~Parser()
    {

    }

    Message Parser::parse()
    {
        // Next token
        scanner_.advance();

        // First token is a semi colon
        if (scanner_.next().type == Token::Type::kSemiColon)
            // Parse a prefix
            parse_prefix();
        // Else
        else
            // Directly parse a command
            parse_command();

        // Return the message
        return message_;
    }

    void Parser::parse_prefix()
    {
        // Prefix [":" prefix space]

        // Skip the ":"
        scanner_.advance();

        // Get the new token representation
        std::string value = scanner_.next().representation;

        // Go to the next token
        scanner_.advance();

        // Token is a "!"
        if (scanner_.next().type == Token::Type::kExclamation)
        {
            // Format is [nickname!user@host]

            // The string we had is the nickname, so store it
            message_.nick = value;

            // Parse the username
            parse_user();
        }
        // Token is a "@"
        else if (scanner_.next().type == Token::Type::kAt)
        {
            // Format is [nickname@host]

            // The string we had is the nickname, so store it
            message_.nick = value;

            // Parse the host
            parse_host();
        }
        // Else if the token is a word
        else if (scanner_.next().type == Token::Type::kWord)
        {
            // The string we had was the server, so store it
            message_.server = value;

            // Parse the command
            parse_command();
        }
        else
        {
            // Throw error
        }
    }

    void Parser::parse_user()
    {
        // Skip the "!"
        scanner_.advance();

        // Get the username
        message_.user = scanner_.next().representation;

        // Advance
        scanner_.advance();

        // Parse host
        parse_host();
    }

    void Parser::parse_host()
    {
        // Skip the "@"
        scanner_.advance();

        // Get the host
        message_.host = scanner_.next().representation;

        // Advance
        scanner_.advance();

        // Parse command
        parse_command();
    }

    void Parser::parse_command()
    {
        // Get the command
        message_.command = scanner_.next().representation;

        // Advance
        scanner_.advance();

        // Parse parameters
        parse_parameters();
    }

    void Parser::parse_parameters()
    {
        // Get the middle parameter
        message_.middle = scanner_.next().representation;

        // Advance
        scanner_.advance();

        // Skip eventual ":"
        if (scanner_.next().type == Token::Type::kSemiColon)
            scanner_.advance();

        // Add the next token to our trailing message
        message_.trailing = scanner_.next().representation;

        // Get all the remaining characters and put it into our trailing message
        message_.trailing += scanner_.remaining();
    }
}
