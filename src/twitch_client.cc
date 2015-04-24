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

#include "twitch_client.h"

#include "command.h"
#include "utils.h"

#include <cstdlib>

TwitchClient::TwitchClient(Server* server) : IrcClient(), server_(server), target_("")
{
    // No ssl
    set_ssl(false);

    // No ssl check
    set_ssl_verify(false);
}

TwitchClient::~TwitchClient()
{

}

void TwitchClient::load(Config& config)
{
    // Get informations from the config file
    std::string server = config.get("Twitch", "Server", "");
    int port = atoi(config.get("Twitch", "Port", "").c_str());
    std::string username = config.get("Twitch", "Username", "");
    std::string password = config.get("Twitch", "Password", "");
    std::string target = config.get("Twitch", "Target", "");

    // Test for empty strings
    if (server.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch server is not set");

    if (username.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch username is not set");

    if (password.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch password is not set");

    if (target.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch target is not set");

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

    // Load the scripts
    manager_.load(server_);
}

void TwitchClient::on_connect()
{
    // Join the default channel
    join(target_);

    // Send a message
    send(target_, "Hey!");
}

void TwitchClient::on_channel(const std::string& sender, const std::string& channel, const std::string& message)
{
    // Trigger a message event first (Requests/Logs)
    manager_.on_message(sender, message);

    // Try to parse a command from the message
    Command* command = Command::parse(message);

    // If we have a command
    if (command)
    {
        // Trigger a command event
        manager_.on_command(sender, command->command(), command->arguments());

        // Delete this command
        delete command;
    }
}
