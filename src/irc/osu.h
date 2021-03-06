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

#ifndef IRC_OSU_H_
#define IRC_OSU_H_

#include "utils/config.h"
#include "irc/client.h"

#include <string>

// Represents a twitch irc client
class OsuClient : public Irc::Client
{
private:
    // The default target
    std::string target_;

    // Called on successful connection
    void handle_on_connect();

public:
    // Constructor
    OsuClient();

    // Destructor
    ~OsuClient();

    // Target getter
    inline const std::string& target() const
    {
        return target_;
    }

    // Target setter
    inline void set_target(const std::string& target)
    {
        target_ = target;
    }

    // Loads the bot
    void load(Config& config);
};

#endif // IRC_OSU_H_
