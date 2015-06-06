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

#include "socket.h"

#include "utils/utils.h"

// The default buffer size
#define BUFFER_SIZE 1024

// A bit of multi-platform defines
#if defined(WIN32) || defined(_WIN32)
#define LAST_SOCKET_ERROR WSAGetLastError()
#define IS_SOCKET_ERROR(socket) (socket == SOCKET_ERROR)
#define CLOSE_SOCKET(socket) closesocket(socket)
#else
#include <sys/types.h> // fd_set
#include <sys/socket.h> // sockets
#include <unistd.h> // close
#include <netinet/in.h> // ???
#include <fcntl.h> // fcntl
#include <cstring> // memcpy
#include <netdb.h> // gethostbyname
#define LAST_SOCKET_ERROR errno
#define IS_SOCKET_ERROR(socket) (socket < 0)
#define CLOSE_SOCKET(socket) close(socket)
#endif

namespace Irc
{
    Socket::Socket() : socket_(0)
    {
    }

    Socket::~Socket()
    {
        disconnect();
    }

    bool Socket::is_valid()
    {
        return socket_ != 0 && !IS_SOCKET_ERROR(socket_);
    }

    bool Socket::is_in(fd_set* sockets)
    {
        return FD_ISSET(socket_, sockets);
    }

    int Socket::socket_error() const
    {
        return LAST_SOCKET_ERROR;
    }

    void Socket::create()
    {
        // Create the socket
        socket_ = ::socket(PF_INET, SOCK_STREAM, 0);

        // If we have an error
        if (IS_SOCKET_ERROR(socket_))
            // Throw it
            Utils::throw_error("Socket", "Constructor", Utils::string_format("Impossible to create the socket: %d", socket_error()));

        // Activate non-blocking mode on the socket (With the correct platform function)
#if !defined (_WIN32)
        if (fcntl(socket_, F_SETFL, fcntl(socket_, F_GETFL, 0) | O_NONBLOCK) != 0)
#else
        unsigned long mode = 0;

        if (ioctlsocket(socket_, FIONBIO, &mode) == SOCKET_ERROR)
#endif
            // Throw on errors
            Utils::throw_error("Socket", "create", Utils::string_format("Impossible to make the socket non blocking: %d", socket_error()));
    }

    void Socket::connect(const std::string& address, int port)
    {
        // Address holder
        struct sockaddr_in socket_address;

        // Host
        struct hostent* host;

        // Set address family
        socket_address.sin_family = AF_INET;

        // Set address port
        socket_address.sin_port = htons(port);

        // Get the host
        host = gethostbyname(address.c_str());

        // No host
        if (host == nullptr)
            // Error
            Utils::throw_error("Socket", "connect", Utils::string_format("Host '%s' does not exist or is not reachable!", address.c_str()));

        // Copy the host to the address holder
        memcpy(&socket_address.sin_addr, host->h_addr_list[0], host->h_length);

        // While we have a connection error
        while (::connect(socket_, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
        {
            // Get the error code
            int status = socket_error();

            // Depending on the status
            if (status != EINTR && status != EINPROGRESS && status != EWOULDBLOCK)
                // Throw en error or try again
                Utils::throw_error("Socket", "connect", Utils::string_format("Impossible to connect to host '%s:%d', error: %d", address.c_str(), port, status));
        }
    }

    void Socket::disconnect()
    {
        // If we have a socket
        if (socket_ != 0)
        {
            // Close the socket
            CLOSE_SOCKET(socket_);

            // Set the socket to 0
            socket_ = 0;
        }
    }

    void Socket::add_select_descriptors(fd_set* sockets, int* max_socket)
    {
        // Add the socket to the given set
        FD_SET(socket_, sockets);

        // If the socket file descriptor is higher than the current max
        if (*max_socket < socket_)
            // Then our socket is the max
            *max_socket = socket_;
    }

    std::string Socket::receive()
    {
        // The buffer
        char buffer[BUFFER_SIZE];

        // Socket operation status
        int status = 0;

        // Length retrieved
        int length = 0;

        // While we have an error when receiving
        while ((length = ::recv(socket_, buffer, BUFFER_SIZE, 0)) < 0)
        {
            // Get the error code
            int status = socket_error();

            // If the is EINTR or EAGAIN, just retry
            if (status != EINTR && status != EAGAIN)
                // If not, throw an exception
                Utils::throw_error("Socket", "receive", Utils::string_format("Error when receiving data: %d\n", status));
        }

        // Wrap the buffer in a string and return it
        return std::string(buffer, buffer + length);
    }

    void Socket::send(const std::string& message)
    {
        // Socket operation status
        int status = 0;

        // Copy the message
        std::string message_copy = message;

        // Append line endings to the message
        message_copy += 0x0D;
        message_copy += 0x0A;

        // While we have an error when trying to send a message
        while (::send(socket_, message_copy.c_str(), message_copy.length(), 0) < 0)
        {
            // Get the error code
            int status = socket_error();

            // If we have an error
            if (status != EINTR && status != EAGAIN)
                // Throw an exception
                Utils::throw_error("Socket", "receive", Utils::string_format("Error when receiving data: %d\n", status));
        }
    }
}
