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

#ifndef IRC_PARSER_H_
#define IRC_PARSER_H_

#include "irc/scanner.h"

#include <string>
#include <vector>

namespace Irc
{
    // Represents a 
    struct Message
    {
        // Server
        std::string server;

        // Nickname
        std::string nick;

        // User
        std::string user;

        // Host
        std::string host;

        // Command
        std::string command;

        // Middle message
        std::string middle;

        // Trailing message
        std::string trailing;
    };

    // Irc parser, meant to inherit from
    class Parser
    {
    private:
        // The lexical scanner
        Scanner scanner_;

        // The message
        Message message_;

        void parse_prefix();
        void parse_nick();
        void parse_user();
        void parse_host();
        void parse_command();
        void parse_parameters();

    public:
        // Constructor
        Parser(const std::string& line);

        // Destructor
        ~Parser();

        // Parse the message in the line and returns it
        Message parse();
    };
}

#endif // IRC_PARSER_H_
