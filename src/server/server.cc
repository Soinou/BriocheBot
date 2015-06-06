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

#include "server/server.h"

#include "utils/config.h"
#include "models/players.h"
#include "models/player.h"
#include "osu/api.h"
#include "utils/logger.h"
#include "utils/utils.h"

#include <chrono>
#include <thread>

#define SLEEP_TIME 200

Server::Server()
    : connection_("127.0.0.1", 6379), manager_(), twitch_(new TwitchClient(this)), osu_(new OsuClient()), current_streamer_(nullptr),
    start_time_(time(nullptr)), change_time_(time(nullptr))
{
    manager_.add_client(twitch_);
    manager_.add_client(osu_);
}

Server::~Server()
{
}

void Server::initialize()
{
    Meow("server")->info("Reading configuration file...");

    // Read the configuration file
    Config config("./config.ini");

    Meow("server")->info("Configuration file successfully read!");

    // Set the logger debug flag (Default to true)
    Meow("server")->set_debug(config.get("General", "Debug", "true") == "true");

    // Set the api key
    OsuApi.set_api_key(config.get("Api", "Key", ""));

    Meow("server")->info("Got osu! api key from the configuration");

    // If the api key is empty
    if (OsuApi.api_key().empty())
        // Error
        Utils::throw_error("Server", "Constructor", "Api key is empty !");

    Meow("server")->info("Loading the two irc clients...");

    // Load the two irc clients
    twitch_->load(config);
    osu_->load(config);

    Meow("server")->info("Successfully loaded the two irc clients!");

    Meow("server")->info("Reading default informations from the config file...");

    // Get the default twitch username (First streamer)
    std::string default_twitch_username = config.get("General", "DefaultTwitchUsername", "");

    // If the default twitch username is empty
    if (default_twitch_username.empty())
        // Error
        Utils::throw_error("Server", "initialize", "Default Twitch username is empty !");

    // Get the default osu! username
    std::string default_osu_username = config.get("General", "DefaultOsuUsername", "");

    // If the default osu! username is empty
    if (default_osu_username.empty())
        // Error
        Utils::throw_error("Server", "Initialize", "Default osu! username is empty !");

    // If the default twitch username does not exist
    if (!PlayersDb.exists(default_twitch_username))
    {
        Meow("server")->info("Player does not exist, creating him...");

        // Add him
        PlayersDb.add(default_twitch_username, default_osu_username, "null", true);

        Meow("server")->info("Player created!");
    }

    // Try to get the current streamer
    Redis::Reply reply = connection_.execute("GET current_streamer");

    Meow("server")->info("Searching the current streamer in the database");

    // If the reply is a string reply
    if (reply.type == Redis::Reply::String)
    {
        Meow("server")->info("Current streamer already exists, setting him");

        // We stored null in the db
        if (reply.string == "null")
            // No streamer
            current_streamer_ = nullptr;
        // We have a streamer
        else
            // Get the player associated with the username we cached and put him as the current streamer
            current_streamer_ = PlayersDb.get(reply.string);

        // Change the osu! client target
        osu_->set_target(reply.string);
    }
    // Else
    else
    {
        Meow("server")->info("Current streamer does not exist, falling back to default");

        // Set the default player as the current streamer
        set_current_streamer(PlayersDb.get(default_twitch_username));
    }
}

void Server::set_current_streamer(Player* current_streamer)
{
    Meow("server")->info("Changing the current streamer");

    // Change the current streamer
    current_streamer_ = current_streamer;

    // No streamer
    if (current_streamer == nullptr)
    {
        // Execute the command to store the current streamer in the db
        connection_.execute("SET current_streamer null");

        // Remove the osu! client target
        osu_->set_target("");
    }
    // A streamer
    else
    {
        // Execute the command to store the current streamer in the db
        connection_.execute(Utils::string_format("SET current_streamer %s", current_streamer_->twitch_username().c_str()));

        // Change the osu! client target
        osu_->set_target(current_streamer_->osu_username());
    }

    // Reinitialize the last time change
    change_time_ = time(nullptr);
}

int Server::up_time() const
{
    // Return the difference between the current time and the bot start time
    return time(nullptr) - start_time_;
}

int Server::stream_time() const
{
    // Return the difference between the current time and the last stream change
    return time(nullptr) - change_time_;
}

void Server::start()
{
    // Connect all the clients
    manager_.connect();

    // Set running as true
    running_ = true;

    // While the server is running (Which means always)
    while (running_)
    {
        // Update the irc clients
        manager_.update();

        // Sleep for a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }
}

void Server::send_twitch(const std::string& message)
{
    twitch_->send(twitch_->target(), message);
}

void Server::send_osu(const std::string& message)
{
    osu_->send(osu_->target(), message);
}

void Server::stop()
{
    // Set running to false, the server will stop itself
    running_ = false;
}