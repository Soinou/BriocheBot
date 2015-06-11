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

#include "events/emitter.h"
#include "uv/socket.h"
#include "uv/timer.h"
#include "irc/options.h"
#include "irc/parser.h"

#include <map>
#include <queue>
#include <string>
#include <vector>
#include <mutex>

// Irc namespace
namespace Irc
{
    ///////////////////////////////////////////////////////////
    //
    // Enums/Structs/Typedefs
    //
    ///////////////////////////////////////////////////////////

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
        ///////////////////////////////////////////////////////////
        //
        // Members
        //
        ///////////////////////////////////////////////////////////

        // Mutex for the user list
        std::mutex mutex_;

        // The timer for write operations
        Uv::Timer timer_;

        // The socket used by the client
        Uv::Socket socket_;

        // The actual client state
        State state_;

        // The client options
        Options options_;

        // The connected users (Associates the channel name to a list of usernames)
        std::map<std::string, std::vector<std::string>> users_;

        // The output queue
        std::queue<std::string> output_;

        ///////////////////////////////////////////////////////////
        //
        // Libuv callbacks
        //
        ///////////////////////////////////////////////////////////

        // Called when the write timer is ready
        void on_write_timer(Uv::Timer* timer);

        // Called when the underlying socket has connected
        void on_socket_connect(Uv::Socket* socket, bool result);

        // Called when the underlying socket has data
        void on_socket_data(Uv::Socket* socket, std::string data);

        // Called when the underlying socket has written something
        void on_socket_write(Uv::Socket* socket, bool result);

        // Called when the underlying socket has closed
        void on_socket_close(Uv::Socket* socket);

        ///////////////////////////////////////////////////////////
        //
        // Irc handlers
        //
        ///////////////////////////////////////////////////////////

        // Handles a reply and calls the right handlers
        void handle(const Reply& reply);

        // Handles a name reply
        void handle_on_name(const Reply& reply);

        // Handles a join reply
        void handle_on_join(const Reply& reply);

        // Handles a part reply
        void handle_on_part(const Reply& reply);

        // Handles a message reply
        void handle_on_message(const Reply& reply);

    protected:
        ///////////////////////////////////////////////////////////
        //
        // Protected methods
        //
        ///////////////////////////////////////////////////////////

        // Adds a list of users to our list
        void add_users(const std::string& channel, const std::vector<std::string>& list);

    public:
        ///////////////////////////////////////////////////////////
        //
        // Constructors/Destructors
        //
        ///////////////////////////////////////////////////////////

        // Constructor
        Client();

        // Destructor
        ~Client();

        ///////////////////////////////////////////////////////////
        //
        // Getters/Setters
        //
        ///////////////////////////////////////////////////////////

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

        // Get the users list of a given channel
        inline const std::vector<std::string>& users_of(const std::string& channel)
        {
            return users_[channel];
        }

        ///////////////////////////////////////////////////////////
        //
        // Start/Stop
        //
        ///////////////////////////////////////////////////////////

        // Starts the client using the given options
        void start();

        // Stops the client
        void stop();

        ///////////////////////////////////////////////////////////
        //
        // Irc commands
        //
        ///////////////////////////////////////////////////////////

        // Sends a message to the given target
        void send(const std::string& target, const std::string& message);

        // Joins a given channel
        void join(const std::string& channel);

        // Joins a channel with a password
        void join(const std::string& channel, const std::string& password);

        // Leaves a channel
        void part(const std::string& channel);

        ///////////////////////////////////////////////////////////
        //
        // Emitters
        //
        ///////////////////////////////////////////////////////////

        // On connect emitter
        Emitter<> on_connect;

        // On join emitter
        Emitter<const std::string&, const std::string&> on_join;

        // On part emitter
        Emitter<const std::string&, const std::string&> on_part;

        // On message emitter
        Emitter<const std::string&, const std::string&, const std::string&> on_message;

        // On private message emitter
        Emitter<const std::string&, const std::string&> on_private_message;

        // On raw emitter
        Emitter<const Reply&> on_raw;
    };
}

#endif // IRC_CLIENT_H_
