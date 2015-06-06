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

#include "irc/twitch.h"

#include "models/command.h"
#include "utils/utils.h"

#include <cstdlib>

TwitchClient::TwitchClient(Server* server) : Irc::Client(), server_(server), target_("")
{
}

TwitchClient::~TwitchClient()
{

}

void TwitchClient::load(Config& config)
{
    // Create a new irc options object
    Irc::Options options;

    // Get informations from the config file
    options.server = config.get("Twitch", "Server", "");
    options.port = atoi(config.get("Twitch", "Port", "").c_str());
    options.username = config.get("Twitch", "Username", "");
    options.realname = options.username;
    options.password = config.get("Twitch", "Password", "");
    std::string target = config.get("Twitch", "Target", "");

    // Test for empty strings
    if (options.server.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch server is not set");

    if (options.username.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch username is not set");

    if (options.password.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch password is not set");

    if (target.empty())
        Utils::throw_error("TwitchClient", "load", "Twitch target is not set");

    // Set options
    set_options(options);

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

void TwitchClient::on_message(const std::string& sender, const std::string& channel, const std::string& message)
{
    printf("Twitch: [%s] %s: (%s)\n", channel.c_str(), sender.c_str(), message.c_str());

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
