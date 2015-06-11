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
#include "irc/reply.h"

#include <string>
#include <vector>

namespace Irc
{
    // Irc parser, meant to inherit from
    class Parser
    {
    private:
        // The lexical scanner
        Scanner scanner_;

        // The reply
        Reply reply_;

        // Parse a prefix
        void parse_prefix();

        // Parse a nickname
        void parse_nick();

        // Parse a user
        void parse_user();

        // Parse a host
        void parse_host();

        // Parse a reply type
        void parse_type();

        // Parse paremeters of a name reply
        void parse_name();

        // Parse parameters of a message reply
        void parse_message();

        // Parse parameters of a join reply
        void parse_join();

        // Parse parameters of a part reply
        void parse_part();

    public:
        // Constructor
        Parser(const std::string& line);

        // Destructor
        ~Parser();

        // Parse the reply in the line and returns it
        Reply parse();
    };
}

#endif // IRC_PARSER_H_
