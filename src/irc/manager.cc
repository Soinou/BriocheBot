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

#include "irc/manager.h"

#include "irc/client.h"
#include "utils/utils.h"

#include <cassert>

namespace Irc
{
    Manager::Manager() : clients_()
    {

    }

    Manager::~Manager()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
        {
            // We should have one
            assert(*i);

            // Delete him
            delete (*i);
        }
    }

    void Manager::add_client(Client* client)
    {
        // We should have a client
        assert(client);

        // Push him to our list
        clients_.push_back(client);
    }

    void Manager::connect()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
        {
            // We should have one
            assert(*i);

            // Connect him
            (*i)->connect();
        }
    }

    void Manager::update()
    {
        // Reset the timeout
        timeout_.tv_sec = 1;
        timeout_.tv_usec = 0;

        // Reset the descriptors sets
        FD_ZERO(&sockets_in_);
        FD_ZERO(&sockets_out_);

        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
        {
            // If the connection was somehow dropped
            if (!(*i)->connected())
                // Reconnect it
                (*i)->connect();

            // Add his descriptors
            (*i)->add_select_descriptors(&sockets_in_, &sockets_out_, &max_socket_);
        }

        // Select the existing sockets. If error
        if (select(max_socket_ + 1, &sockets_in_, &sockets_out_, nullptr, &timeout_) < 0)
            // Return false
            Utils::throw_error("Manager", "update", "Impossible to select a valid socket");

        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
            // Process the descriptors
            (*i)->process_select_descriptors(&sockets_in_, &sockets_out_);
    }

    void Manager::stop()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
        {
            // We should have one
            assert(*i);

            // Stop him
            (*i)->disconnect();
        }
    }
}
