// ircbot2
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

#include "osu_client.h"

#include "utils.h"

#include <cstdlib>

OsuClient::OsuClient() : IrcClient(), target_("")
{
    // No ssl
    set_ssl(false);
}

OsuClient::~OsuClient()
{

}

void OsuClient::load(Config& config)
{
    // Get informations from the config file
    std::string server = config.get("Osu", "Server", "");
    int port = atoi(config.get("Osu", "Port", "").c_str());
    std::string username = config.get("Osu", "Username", "");
    std::string password = config.get("Osu", "Password", "");
    std::string target = config.get("Osu", "Target", "");

    // Test for empty strings
    if (server.empty())
        Utils::throw_error("OsuClient", "load", "osu! server is not set");

    if (username.empty())
        Utils::throw_error("OsuClient", "load", "osu! username is not set");

    if (password.empty())
        Utils::throw_error("OsuClient", "load", "osu! password is not set");

    if (target.empty())
        Utils::throw_error("OsuClient", "load", "osu! target is not set");

    // Server
    set_server(server);

    // Port
    set_port(port);

    // Nickname, username and realname
    set_nick(username);
    set_username(username);
    set_realname(username);

    // Password
    set_password(password);

    // Target
    set_target(target);
}

void OsuClient::on_connect()
{
    // Join the linked channel
    join(target_);

    // Send some message (Yeah, the only reason this file SHOULD be in UTF-8. Damn accents)
    send(target_, "Paré à transmettre les requêtes!");
}

void OsuClient::on_channel(const std::string& sender, const std::string& channel, const std::string& message)
{
    // Ignore any message
}
