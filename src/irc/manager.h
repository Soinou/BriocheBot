// irc
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

#ifndef IRC_MANAGER_H_
#define IRC_MANAGER_H_

#include "irc/sockets.h"

#include <vector>

// Irc namespace
namespace Irc
{
    // Forward declaration of the client class
    class Client;

    // An irc client manager
    class Manager
    {
    public:
        // Little typedef
        typedef struct timeval time_value;

    private:
        // The list of clients
        std::vector<Client*> clients_;

        // Input sockets
        fd_set sockets_in_;

        // Output sockets
        fd_set sockets_out_;

        // Max socket
        int max_socket_;

        // Timeout
        time_value timeout_;

    public:
        // Constructor
        Manager();

        // Destructor
        ~Manager();

        // Adds a client (Clients deletion will be handled by the manager)
        void add_client(Client* client);

        // Connects all the clients
        void connect();

        // Updates all the clients (Returns false on failure)
        bool update();

        // Stops all the clients
        void stop();
    };
}

#endif // IRC_MANAGER_H_
