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

#include "logger.h"
#include "server.h"
#include "utils.h"

#if defined(WIN32) || defined(_WIN32)
// Include winsock2 on windows
#include <winsock2.h>
#endif

#include <cstdio>
#include <stdexcept>

// Main entry point
int main()
{
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
        Log.info("Preparing the bot...");

        // Create the server
        Server server;

        Log.info("Bot successfully prepared, now running");

        // Run the server
        server.run();
    }
    // Catch runtime errors
    catch (std::runtime_error e)
    {
        // Log these errors
        Log.error(e.what());
        fprintf(stderr, "Error: %s\n", e.what());
    }

    // We should never exit this program, so when it really exits, there is a problem
    return 42;
}