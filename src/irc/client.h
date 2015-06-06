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

#ifndef IRC_CLIENT_H_
#define IRC_CLIENT_H_

#include "irc/socket.h"
#include "irc/options.h"
#include "irc/parser.h"

#include <string>
#include <queue>

// Irc namespace
namespace Irc
{
    // Represents all the possible irc states
    enum State
    {
        kInitializing,
        kDisconnecting,
        kDisconnected,
        kConnecting,
        kConnected,
        kError
    };

    // A structure holding irc related options
    struct Options
    {
        // The server
        std::string server;

        // The port
        unsigned int port;

        // The server password
        std::string password;

        // The nick
        std::string nick;

        // The username
        std::string username;

        // The real name
        std::string realname;
    };

    // Represents an irc client
    class Client
    {
    private:
        // The socket used by the client
        Socket socket_;

        // The actual client state
        State state_;

        // The client options
        Options options_;

        // The output queue
        std::queue<std::string> output_;

        // Handles a message and calls the right handlers
        void handle(const Message& message);

    public:
        // Constructor
        Client();

        // Destructor
        ~Client();

        // Socket getter
        inline const Socket& socket() const
        {
            return socket_;
        }

        // State getter
        inline const State& state() const
        {
            return state_;
        }

        // Options getter
        inline const Options& options() const
        {
            return options_;
        }

        // Options setter
        inline void set_options(const Options& options)
        {
            options_ = options;
        }

        // Checks if the client is correctly connected
        inline bool connected()
        {
            return socket_.is_valid();
        }

        // Connects the client using the given options
        void connect();

        // Add the session descriptors to the given sets
        void add_select_descriptors(fd_set* sockets_in, fd_set* sockets_out, int* max_socket);

        // Process the session descriptors
        void process_select_descriptors(fd_set* sockets_in, fd_set* sockets_out);

        // Forcefully disconnects the client
        void disconnect();

        // Sends a message to the given target
        void send(const std::string& target, const std::string& message);

        // Joins a given channel
        void join(const std::string& channel);

        // Joins a channel with a password
        void join(const std::string& channel, const std::string& password);

        // Called on connect
        virtual void on_connect();

        // Called on message
        virtual void on_message(const std::string& sender, const std::string& channel, const std::string& message);

        // Called on private message
        virtual void on_private_message(const std::string& sender, const std::string& message);

        // Called on raw message
        virtual void on_raw(const Message& message);
    };
}

#endif // IRC_CLIENT_H_
