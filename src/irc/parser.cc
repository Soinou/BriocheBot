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
    Parser::Parser(const std::string& line) : scanner_(line), reply_()
    {
    }

    Parser::~Parser()
    {

    }

    Reply Parser::parse()
    {
        // Get the raw line into our reply
        reply_.raw = scanner_.line();

        // Next token
        scanner_.advance();

        // First token is a semi colon
        if (scanner_.next().type == Token::Type::kSemiColon)
            // Parse a prefix
            parse_prefix();
        // Else
        else
            // Directly parse a command
            parse_type();

        // Return the reply
        return reply_;
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
            reply_.nickname = value;

            // Parse the username
            parse_user();
        }
        // Token is a "@"
        else if (scanner_.next().type == Token::Type::kAt)
        {
            // Format is [nickname@host]

            // The string we had is the nickname, so store it
            reply_.nickname = value;

            // Parse the host
            parse_host();
        }
        // Else if the token is a word
        else if (scanner_.next().type == Token::Type::kWord)
        {
            // The string we had was the server, so store it
            reply_.server = value;

            // Parse the command
            parse_type();
        }
        // Else
        else
            // Reply is invalid
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_user()
    {
        // Skip the "!"
        scanner_.advance();

        // If the next token is a word
        if (scanner_.next().type == Token::kWord)
        {
            // Get the username
            reply_.username = scanner_.next().representation;

            // Advance
            scanner_.advance();

            // Parse host
            parse_host();
        }
        // Else
        else
            // Reply is invalid
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_host()
    {
        // If the token is an @
        if (scanner_.next().type == Token::kAt)
        {
            // Skip the "@"
            scanner_.advance();

            // If the next token is a word
            if (scanner_.next().type == Token::kWord)
            {
                // Get the host
                reply_.host = scanner_.next().representation;

                // Advance
                scanner_.advance();

                // Parse command
                parse_type();
            }
            // Else
            else
                // Reply is invalid
                reply_.type = Reply::kInvalid;
        }
        // Else
        else
            // Reply is invalid
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_type()
    {
        // Get the token as a string
        std::string command = scanner_.next().representation;

        // RPL_ENDMOTD
        if (command == "376")
            reply_.type = Reply::kConnected;
        // PING
        else if (command == "PING")
            reply_.type = Reply::kPing;
        // RPL_NAMREPLY
        else if (command == "353")
        {
            reply_.type = Reply::kName;

            // Advance
            scanner_.advance();

            // Parse parameters
            parse_name();
        }
        // PRIVMSG
        else if (command == "PRIVMSG")
        {
            reply_.type = Reply::kMessage;

            // Advance
            scanner_.advance();

            // Parse parameters
            parse_message();
        }
        // JOIN
        else if (command == "JOIN")
        {
            reply_.type = Reply::kJoin;

            // Advance
            scanner_.advance();

            // Parse parameters
            parse_join();
        }
        // PART
        else if (command == "PART")
        {
            reply_.type = Reply::kPart;

            // Advance
            scanner_.advance();

            // Parse parameters
            parse_part();
        }
        // Invalid
        else
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_name()
    {
        // If we have a word
        if (scanner_.next().type == Token::kWord)
        {
            // Skip it, this is our username, we don't want it
            scanner_.advance();

            // Skip the */=/@ character, we don't really care about it
            if (scanner_.next().representation == "*"
                || scanner_.next().representation == "@"
                || scanner_.next().representation == "=")
                scanner_.advance();

            // If we now have a word
            if (scanner_.next().type == Token::kWord)
            {
                // Push it, this is the channel name
                reply_.parameters.push_back(scanner_.next().representation);

                // Advance
                scanner_.advance();

                // If we have a semi colon, skip it
                if (scanner_.next().type == Token::kSemiColon)
                    scanner_.advance();

                // While we have a token
                while (scanner_.next().type != Token::kEnd)
                {
                    // If this token is a word
                    if (scanner_.next().type == Token::kWord)
                    {
                        // This is an username, push it to our parameters
                        reply_.parameters.push_back(scanner_.next().representation);

                        // Then advance
                        scanner_.advance();
                    }
                    // Else
                    else
                    {
                        // Invalid reply
                        reply_.type = Reply::kInvalid;
                        // Stop here
                        break;
                    }
                }
            }
            // Else
            else
                // Invalid reply
                reply_.type = Reply::kInvalid;
        }
        // Else
        else
            // Invalid reply
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_message()
    {
        // If we have a word
        if (scanner_.next().type == Token::kWord)
        {
            // This is the target, get it
            reply_.parameters.push_back(scanner_.next().representation);

            // Advance
            scanner_.advance();

            // If the token is a semi colon, skip it
            if (scanner_.next().type == Token::kSemiColon)
                scanner_.advance();

            // Then get everything we got left and push it
            reply_.parameters.push_back(scanner_.remaining());
        }
        // Else
        else
            // Reply is invalid
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_join()
    {
        // If we have a word
        if (scanner_.next().type == Token::kWord)
            // This is the channel joined, get it
            reply_.parameters.push_back(scanner_.next().representation);
        // Else
        else
            // Invalid reply
            reply_.type = Reply::kInvalid;
    }

    void Parser::parse_part()
    {
        // If we have a word
        if (scanner_.next().type == Token::kWord)
            // This is the channel joined, get it
            reply_.parameters.push_back(scanner_.next().representation);
        // Else
        else
            // Invalid reply
            reply_.type = Reply::kInvalid;
    }
}
