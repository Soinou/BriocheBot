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
#include "utils/http.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "uv/work.h"

#include <json/json.h>

#include <cstdlib>

TwitchClient::TwitchClient(Server* server) : Irc::Client(), server_(server), target_("")
{
    // Register irc events
    on_connect += [this]() { handle_on_connect(); };
    on_message += [this](const std::string& channel, const std::string& username, const std::string& message) { handle_on_message(channel, username, message); };
}

TwitchClient::~TwitchClient()
{

}

static std::vector<std::string> parse(const Json::Value& json)
{
    // The list we'll return
    std::vector<std::string> list;

    // If the value passed is a correct array
    if (json.isArray())
        // For each value in this array
        for (auto i = json.begin(); i != json.end(); i++)
            // If the value is a string
            if (i->isString())
                // Push it
                list.push_back(i->asString());

    // Return the list
    return list;
}

void TwitchClient::get_user_list()
{
    // Get json from the only thing that works correctly
    std::string string = Bringz.get("tmi.twitch.tv", 80, Utils::string_format("/group/user/%s/chatters", target_.substr(1).c_str()));

    // Trim the string we got 
    Utils::trim(string);

    // Create a json holder
    Json::Value json;

    // Create a json reader
    Json::Reader reader;

    // If we can parse the json
    if (reader.parse(string, json))
    {
        // If the json we parsed is an object
        if (json.isObject())
        {
            // Get the chatters object
            Json::Value chatters = json.get("chatters", 0);

            // If the chatters object is a valid object
            if (chatters.isObject())
            {
                // Get all the arrays and add them to our user list
                add_users(target_, parse(chatters.get("moderators", 0)));
                add_users(target_, parse(chatters.get("staff", 0)));
                add_users(target_, parse(chatters.get("admins", 0)));
                add_users(target_, parse(chatters.get("global_mods", 0)));
                add_users(target_, parse(chatters.get("viewers", 0)));
            }
            // Else
            else
                // Error
                Meow("server")->error("Impossible to get json from the twitch user list");
        }
        // Else
        else
            // Error
            Meow("server")->error("Impossible to get json from the twitch user list");
    }
    // Else
    else
        // Error
        Meow("server")->error("Impossible to get json from the twitch user list");
}

void TwitchClient::handle_on_connect()
{
    // Run the get user list method
    Uv::run([this]() { get_user_list(); });

    // Join the default channel
    join(target_);

    // Send a message
    send(target_, "Hey!");
}

void TwitchClient::handle_on_message(const std::string& channel, const std::string& username, const std::string& message)
{
    // Trigger a message event first (Requests/Logs)
    manager_.on_message(username, message);

    // Try to parse a command from the message
    Command* command = Command::parse(message);

    // If we have a command
    if (command)
    {
        // Trigger a command event
        manager_.on_command(username, command->command(), command->arguments());

        // Delete this command
        delete command;
    }
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
