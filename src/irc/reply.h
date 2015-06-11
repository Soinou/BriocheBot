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

#ifndef IRC_REPLY_H_
#define IRC_REPLY_H_

#include <string>
#include <vector>

namespace Irc
{
    // Represents an irc reply
    struct Reply
    {
        // The different reply types
        enum Type
        {
            // Invalid reply (Or malformatted)
            kInvalid,

            // Connected (End of motd)
            kConnected,

            // Ping
            kPing,

            // Name reply (When joining channel/issuing list command)
            kName,

            // Join reply
            kJoin,

            // Part reply
            kPart,

            // Message
            kMessage,
        };

        // Raw string
        std::string raw;

        // The server
        std::string server;

        // The nickname
        std::string nickname;

        // The username
        std::string username;

        // The host name
        std::string host;

        // The reply type
        Type type;

        // The parameters (Depends of the command type)
        std::vector<std::string> parameters;
    };
}

#endif // IRC_REPLY_H_
