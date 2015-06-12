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
#include "models/viewers.h"
#include "models/moderator.h"
#include "osu/api.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "uv/work.h"

#include <chrono>

Server::Server()
// Redis server is on localhost at 6379
: connection_("127.0.0.1", 6379),
// Timer calls our update method every 5 seconds
timer_(5000, std::bind(&Server::update, this, std::placeholders::_1)),
// Initialize irc manager and clients
manager_(), twitch_(new TwitchClient(this)), osu_(new OsuClient()),
// No current streamer for now
current_streamer_(nullptr),
// Initialize times
start_time_(time(nullptr)), last_update_(time(nullptr)), change_time_(time(nullptr))
{
    // Add clients to the irc manager
    manager_.add_client(twitch_);
    manager_.add_client(osu_);
}

Server::~Server()
{
}

void Server::update(Uv::Timer* timer)
{
    // Get the new time
    time_t current_time = time(nullptr);

    // Get the difference between the last update and now
    time_t difference = current_time - last_update_;

    // Get the list of connected users in the twitch chat
    const std::vector<std::string>& user_list = twitch_->users_of(twitch_->target());

    // For each username
    for (auto i = user_list.begin(); i != user_list.end(); i++)
    {
        // Get the associated  viewer
        Viewer* viewer = the_viewers.get(*i);

        // If he doesn't exist
        if (viewer == nullptr)
        {
            // Create a new one
            viewer = new Viewer();

            // Change his twitch username
            viewer->set_twitch_username(*i);
        }

        // Increase the viewer online time
        viewer->set_online_time(viewer->online_time() + difference);

        // Save the viewer
        viewer->insert();
    }

    // If we have a current streamer
    if (current_streamer_ != nullptr)
    {
        // Copy the current streamer
        Streamer* streamer = current_streamer_;

        // Update the streamer time
        streamer->set_stream_time(streamer->stream_time() + difference);

        // Update him
        streamer->insert();
    }

    // Replace our last update with the current time
    last_update_ = current_time;
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
    if (!the_viewers.exists(default_twitch_username))
    {
        Meow("server")->info("Player does not exist, creating him...");

        // Create a new moderator
        Moderator* moderator = new Moderator();

        // Set some data
        moderator->set_id(the_viewers.max_id() + 1);
        moderator->set_twitch_username(default_twitch_username);
        moderator->set_osu_username(default_osu_username);
        moderator->set_osu_skin_link("null");
        moderator->set_privileges(1);

        // Add him
        moderator->insert();

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
        {
            try
            {
                // Get the player associated with the username we cached and put him as the current streamer
                current_streamer_ = dynamic_cast<Streamer*>(the_viewers.get(reply.string));
            }
            // On bad cast
            catch (std::bad_cast e)
            {
                // Print some error, but ignore it, we'll just have no streamer set
                Meow("server")->error("Current streamer in the database is not a valid streamer, can't set him");
            }
        }

        // Change the osu! client target
        osu_->set_target(reply.string);
    }
    // Else
    else
    {
        Meow("server")->info("Current streamer does not exist, falling back to default");

        // Get the default viewer
        Viewer* viewer = the_viewers.get(default_twitch_username);

        try
        {
            // Set the default player as the current streamer
            set_current_streamer(dynamic_cast<Streamer*>(the_viewers.get(default_twitch_username)));
        }
        // If the default player is not defined as a streamer
        catch (std::bad_cast e)
        {
            // Error, but don't crash, since we'll just have no streamer set
            Meow("server")->error("Default streamer is not a streamer, can't set him!");
        }
    }
}

void Server::set_current_streamer(Streamer* current_streamer)
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
    // Start the client manager
    manager_.start();

    // Set running as true
    running_ = true;

    // Start the timer
    timer_.start();
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
    // Stop the timer
    timer_.stop();

    // Set running to false
    running_ = false;

    // Stop the client manager
    manager_.stop();
}