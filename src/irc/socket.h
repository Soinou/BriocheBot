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

#ifndef IRC_SOCKET_H_
#define IRC_SOCKET_H_

// On Windows
#if defined(WIN32) || defined(_WIN32)

// Include windows sockets library
#include <winsock2.h>

// Windows socket holder is a SOCKET object
typedef SOCKET socket_t;

// On Unix
#else

// Include select header
#include <sys/select.h>

// Unix socket type is a simple int
typedef int socket_t;

#endif

#include <string>

namespace Irc
{
    // Portable socket class
    class Socket
    {
    private:
        // The socket object
        socket_t socket_;

    public:
        // Constructor
        Socket();

        // Destructor
        ~Socket();

        // Socket getter
        inline const socket_t& socket() const
        {
            return socket_;
        }

        // If the socket is correctly connected
        bool is_valid();

        // Checks if the socket is in the given set
        bool is_in(fd_set* sockets);

        // Get the current socket error
        int socket_error() const;

        // Creates the socket
        void create();

        // Connects the socket
        void connect(const std::string& address, int port);

        // Disconnects the socket
        void disconnect();

        // Adds the socket to the given file descriptors set
        void add_select_descriptors(fd_set* sockets, int* max_socket);

        // Receive a string
        std::string receive();

        // Send something
        void send(const std::string& message);
    };
}

#endif // IRC_SOCKET_H_
