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

#include "server.h"

#include "config.h"
#include "players.h"
#include "player.h"
#include "osu_api.h"
#include "utils.h"

#include <sstream>

Server::Server() : connection_("127.0.0.1", 6379), twitch_(this), osu_(), current_streamer_(nullptr)
{
    initialize();
}

Server::~Server()
{

}

void Server::initialize()
{
    // Read the configuration file
    Config config("./config.ini");

    // Set the api key
    OsuApi.set_api_key(config.get("Api", "Key", ""));

    // If the api key is empty
    if (OsuApi.api_key().empty())
        // Error
        Utils::throw_error("Server", "Constructor", "Api key is empty !");

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
        // Add him
        PlayersDb.add(default_twitch_username, default_osu_username, "null", true);

    // Try to get the current streamer
    Redis::Reply reply = connection_.execute("GET current_streamer");

    // If the reply is a string reply
    if (reply.type == Redis::Reply::String)
        // Get the player associated with the username we cached and put him as the current streamer
        current_streamer_ = PlayersDb.get(reply.string);
    // Else
    else
        // Set the default player as the current streamer
        set_current_streamer(PlayersDb.get(default_twitch_username));

    // Load the two bots with this configuration
    twitch_.load(config);

    osu_.load(config);
}

void Server::set_current_streamer(Player* current_streamer)
{
    // Change the current streamer
    current_streamer_ = current_streamer;

    // Create a string stream
    std::stringstream stream;

    // Parse the redis command
    stream << "SET current_streamer " << current_streamer_->twitch_username();

    // Execute the command to store the current streamer in the db
    connection_.execute(stream.str());

    // Change the osu! client target
    osu_.set_target(current_streamer_->osu_username());
}

void Server::run()
{
    // Connect the Twitch session
    twitch_.connect();

    // Connect the osu! session
    osu_.connect();

    // Initialize sockets sets
    fd_set sockets, out_sockets;

    // Initialize sockets count
    int sockets_count;

    // Initialize timeout struct
    struct timeval timeout;

    // Set running as true
    running_ = true;

    // While the server is running (Which means always)
    while (running_)
    {
        // Twitch session has disconnected
        if (!twitch_.connected())
            // Reconnect it
            twitch_.connect();

        // osu! session has disconnected
        if (!osu_.connected())
            // Reconnect it
            osu_.connect();

        // Reset timeout values
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Reset sockets count
        sockets_count = 0;

        // Reset sockets and out sockets
        FD_ZERO(&sockets);
        FD_ZERO(&out_sockets);

        // Add sessions descriptors
        irc_add_select_descriptors(twitch_.session(), &sockets, &out_sockets, &sockets_count);
        irc_add_select_descriptors(osu_.session(), &sockets, &out_sockets, &sockets_count);

        // Select something. If it went wrong
        int available = select(sockets_count + 1, &sockets, &out_sockets, NULL, &timeout);

        printf("Got socket: %d\n", available);

        // Error
        if (available < 0)
            // Error
            Utils::throw_error("Server", "run", "Something went wrong when selecting a socket");

        // We have a socket
        if (available > 0)
        {
            // If there was something wrong when processing the osu! session
            if (irc_process_select_descriptors(twitch_.session(), &sockets, &out_sockets))
                // Error
                Utils::throw_error("Server", "run", Utils::string_format("Error with the Twitch session: %s", twitch_.get_error()));

            // If there was something wrong when processing the osu! session
            if (irc_process_select_descriptors(osu_.session(), &sockets, &out_sockets))
                // Error
                Utils::throw_error("Server", "run", Utils::string_format("Error with the osu! session: %s", osu_.get_error()));
        }
    }

    // Stop the twitch session
    twitch_.stop();

    // Stop the osu! session
    osu_.stop();
}

void Server::send_twitch(const std::string& message)
{
    twitch_.send(twitch_.target(), message);
}

void Server::send_osu(const std::string& message)
{
    osu_.send(osu_.target(), message);
}

void Server::stop()
{
    // Set running to false, the server will stop itself
    running_ = false;
}