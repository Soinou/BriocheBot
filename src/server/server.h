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

#ifndef SERVER_H_
#define SERVER_H_

#include "irc/manager.h"
#include "irc/osu.h"
#include "irc/twitch.h"
#include "utils/redis.h"

#include <time.h>

// Forward declaration of the Viewer class
class Streamer;

// Represents a server
class Server
{
private:
    // The connection to the redis server
    Redis::Connection connection_;

    // The internal timer
    Uv::Timer timer_;

    // The irc manager
    Irc::Manager manager_;

    // The twitch client
    TwitchClient* twitch_;

    // The osu! client
    OsuClient* osu_;

    // If the server is running
    bool running_;

    // The current streamer
    Streamer* current_streamer_;

    // The bot start time
    time_t start_time_;

    // The last time the update method was called
    time_t last_update_;

    // The last change time
    time_t change_time_;

    // Internal method called by the timer
    void update(Uv::Timer* timer);

public:
    // Constructor
    Server();

    // Destructor
    ~Server();

    // Initializes the server
    void initialize();

    // Current streamer getter
    inline Streamer* current_streamer()
    {
        return current_streamer_;
    }

    // Current streamer setter
    void set_current_streamer(Streamer* current_streamer);

    // Running getter
    inline bool running()
    {
        return running_;
    }

    // Get the total bot uptime
    int up_time() const;

    // Get the total stream time of the last streamer
    int stream_time() const;

    // Starts the server
    void start();

    // Sends a message to the twitch client
    void send_twitch(const std::string& message);

    // Sends a message to the osu! client
    void send_osu(const std::string& message);

    // Forcefully stop the server
    void stop();
};

#endif // SERVER_H_
