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

#include "irc/osu.h"

#include "utils/utils.h"
#include "irc/options.h"

#include <cstdlib>

OsuClient::OsuClient() : Irc::Client(), target_("")
{
}

OsuClient::~OsuClient()
{
}

void OsuClient::load(Config& config)
{
    Irc::Options options;

    // Get informations from the config file
    options.server = config.get("Osu", "Server", "");
    options.port = atoi(config.get("Osu", "Port", "").c_str());
    options.username = config.get("Osu", "Username", "");
    options.realname = options.username;
    options.password = config.get("Osu", "Password", "");
    std::string target = config.get("Osu", "Target", "");

    // Test for empty strings
    if (options.server.empty())
        Utils::throw_error("OsuClient", "load", "osu! server is not set");

    if (options.username.empty())
        Utils::throw_error("OsuClient", "load", "osu! username is not set");

    if (options.password.empty())
        Utils::throw_error("OsuClient", "load", "osu! password is not set");

    if (target.empty())
        Utils::throw_error("OsuClient", "load", "osu! target is not set");

    // Set options
    set_options(options);

    // Target
    set_target(target);
}

void OsuClient::on_connect()
{
    // Send some message (Yeah, the only reason this file SHOULD be in UTF-8. Damn accents)
    send(target_, "Paré à transmettre les requêtes!");
}

void OsuClient::on_message(const std::string& sender, const std::string& channel, const std::string& message)
{
    // Ignore any message
}