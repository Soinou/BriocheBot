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

namespace Irc
{
    Manager::Manager() : clients_()
    {

    }

    Manager::~Manager()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
            // Delete him
            delete (*i);
    }

    void Manager::add_client(Client* client)
    {
        // Push him to our list
        clients_.push_back(client);
    }

    void Manager::start()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
            // Start him
            (*i)->start();
    }

    void Manager::stop()
    {
        // For each client
        for (auto i = clients_.begin(); i != clients_.end(); i++)
            // Stop him
            (*i)->stop();
    }
}
