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

#include "utils/logger.h"
#include "server/server.h"
#include "utils/utils.h"
#include "irc/client.h"

#if defined(WIN32) || defined(_WIN32)
// Include winsock2 on windows
#include <winsock2.h>
#endif

#include <cstdio>
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
}

// Main entry point
int main()
{
    // Register our signal callback first
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    try
    {
        // If under windows
#if defined(WIN32) || defined(_WIN32)
        // Initialize the winsocket data
        WSADATA socket_data;

        // If there is an error when initializing winsocket
        if (WSAStartup(MAKEWORD(2, 2), &socket_data) != 0)
            // Error
            Utils::throw_error("Server", "run", "Impossible to initialize winsock");
#endif

        Meow("server")->info("Preparing the bot...");

        // Create the server
        server = new Server();

        // Initialize the server
        server->initialize();

        Meow("server")->info("Bot successfully prepared, now running");

        // Run the server
        server->start();
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

    Meow("server")->info("Server stopped, waiting for the loggers to terminate...");

    // Wait for the logger to terminate
    Logger::get_instance().wait();

    // If we are on windows
#ifdef WIN32
    // Cleanup the sockets
    WSACleanup();
#endif

    // We should never exit this program, so when it really exits, there is a problem
    return 42;
}