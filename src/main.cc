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

#include "models/viewers.h"
#include "server/server.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "uv/loop.h"

#include <stdexcept>
#include <csignal>

// The server (As static because of signals)
static Server* server;

// The signal handler
static void handler(int)
{
    Meow("server")->info("Got SIGINT, stopping the server...");

    // Stop the server if we catch this signal
    server->stop();

    // And also stop all the loggers
    Logger::get_instance().stop();
}

// Main entry point
int main()
{
    // Register our signal callback first
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    try
    {
        // Load the viewers
        the_viewers.load();

        Meow("server")->info("Preparing the bot...");

        // Create the server
        server = new Server();

        // Initialize the server
        server->initialize();

        Meow("server")->info("Bot successfully prepared, now running");

        // Run the server
        server->start();

        // Run the main loop
        the_loop.run();
    }
    // Catch runtime errors
    catch (std::runtime_error e)
    {
        // Log these errors
        Meow("server")->error(e.what());
    }

    // If the server could initialize
    if (server)
    {
        // Log
        Meow("server")->info(Utils::string_format("Server stopping after %s of uptime", Utils::time_format(server->up_time()).c_str()));

        // Delete it
        delete server;
    }
    // Else
    else
        // Nope
        Meow("server")->error("Server could not initialize correctly!");

    // We should never exit this program, so when it really exits, there is a problem
    return 42;
}
